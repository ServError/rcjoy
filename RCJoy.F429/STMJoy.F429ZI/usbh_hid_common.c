#include "usbh_hid_common.h"
#include "inttypes.h"
#include "usbh_def.h"



uint8_t usb_input_buffer[1024];
uint8_t usb_buffer[256];
uint8_t usb_repd_buffer[2048];
uint16_t usb_rep_desc_len;
uint16_t usb_rep_len;
volatile uint8_t usb_data_valid = FALSE;

uint16_t USB_Poll_Time = 4;

uint8_t Report_Total_Length;
uint8_t IsMultiReport;
volatile uint8_t CurrentReportShift = 0;
uint8_t HasReports = 0;

uint16_t VendorID;
uint16_t ProductID;

uint8_t data_pid_stored;

extern void ParseReportDescriptor(uint8_t *report, uint16_t len, uint16_t* bitlen, uint8_t *mreport);
extern USBH_HandleTypeDef hUsbHostHS;


USBH_StatusTypeDef USBH_HID_CommonInit(USBH_HandleTypeDef *phost)
{
	HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;

	HID_Handle->pData = usb_buffer;
	fifo_init(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * HID_Handle->length);

	VendorID = phost->device.DevDesc.idVendor;
	ProductID = phost->device.DevDesc.idProduct;
	USB_Poll_Time = HID_Handle->poll;
	HasReports = 0;
	IsMultiReport = 0;
	CurrentReportShift = 0;
	data_pid_stored = 0xFF;
	
	return USBH_OK;
}


void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
	HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;
	uint16_t i;

	HasReports = 1;
	
	usb_rep_len = HID_Handle->length;

	if (HID_Handle->length == 0)
	{
		usb_data_valid = FALSE;
		CurrentReportShift = 0;
		return;
	}

	usb_data_valid = (fifo_read(&HID_Handle->fifo, usb_buffer, HID_Handle->length) == HID_Handle->length);

	if (!usb_data_valid)
	{
		CurrentReportShift = 0;
		return;
	}

	if (Report_Total_Length > HID_Handle->length)
	{
		// check seq here

		uint8_t data_pid = ((((HCD_HandleTypeDef*)phost->pData)->hc)[HID_Handle->InPipe]).data_pid;
		
		if (data_pid == data_pid_stored)
		{
			return; // reinit interface ???
		}

		data_pid_stored = data_pid;

#ifdef MULTIREP11
		if (IsMultiReport && CurrentReportShift == 0)
		{
			for (i = 0; i < HID_Handle->length - 1; i++)
				usb_input_buffer[i + CurrentReportShift] = usb_buffer[i + 1];

			CurrentReportShift += HID_Handle->length - 1;
		}
		else
		{
			for (i = 0; i < HID_Handle->length; i++)
				usb_input_buffer[i + CurrentReportShift] = usb_buffer[i];

			CurrentReportShift += HID_Handle->length;
		}
#else
		for (i = 0; i < HID_Handle->length; i++)
			usb_input_buffer[i + CurrentReportShift] = usb_buffer[i];

		CurrentReportShift += HID_Handle->length;
#endif
	}
	else
	{
		if (IsMultiReport)
		{
			if (usb_buffer[0] != 1) return;

			for (i = 0; i < HID_Handle->length - 1; i++)
				usb_input_buffer[i] = usb_buffer[i + 1];
		}
		else
		{
			for (i = 0; i < HID_Handle->length; i++)
				usb_input_buffer[i] = usb_buffer[i];
		}
	}

	if (CurrentReportShift >= Report_Total_Length)
		CurrentReportShift = 0;
}

void USB_HID_DataTimeoutCallBack(USBH_HandleTypeDef *phost)
{
	//CurrentReportShift = 0;
}

void USB_HID_ReportReadCallback(USBH_HandleTypeDef *phost)
{
	HID_HandleTypeDef *HID_Handle = phost->pActiveClass->pData;
	
	usb_rep_desc_len = HID_Handle->HID_Desc.wItemLength;
	uint16_t i;

	for (i = 0; i < usb_rep_desc_len; i++)
		*(usb_repd_buffer +i) = *(phost->device.Data + i);

	uint16_t replen_bits;

	ParseReportDescriptor(usb_repd_buffer, usb_rep_desc_len, &replen_bits, &IsMultiReport);
	Report_Total_Length = replen_bits >> 3;
}

uint8_t *USB_HID_GetLastReport()
{
	if (usb_data_valid)
		return usb_input_buffer;
	else
		return NULL;
}

void USB_HID_SetDebuggerReport(uint8_t *buffer, uint16_t length)
{
	uint16_t i;
	for (i = 0; i < length; i++)
	{
		usb_input_buffer[i] = buffer[i];
	}
	usb_rep_len = length;
	usb_data_valid = 1;
}

uint16_t USB_HID_GetReportLength()
{
	return Report_Total_Length ? Report_Total_Length : usb_rep_len;
}

void USB_GetReportDescriptor(uint16_t *l, uint8_t **b)
{
	if (usb_rep_desc_len)
	{
		*l = usb_rep_desc_len;
		*b = usb_repd_buffer;
	}
	else
		*l = 0;
}

uint8_t CheckJoystick(uint16_t vendor, uint16_t product)
{
	return (VendorID == vendor) && (ProductID == product);
}

void GetJoyInfo(uint16_t *pVendor, uint16_t *pProduct)
{
	*pVendor = VendorID;
	*pProduct = ProductID;
}

void USBGetStatuses(uint8_t *b)
{
	b[0] = hUsbHostHS.gState;
	b[1] = hUsbHostHS.EnumState;
	b[2] = hUsbHostHS.RequestState;
	b[3] = hUsbHostHS.Control.state;
	b[4] = hUsbHostHS.device.speed;

	if (hUsbHostHS.pActiveClass)
	{
		HID_HandleTypeDef *HID_Handle = hUsbHostHS.pActiveClass->pData;
		b[5] = HID_Handle->state;
		b[6] = HID_Handle->ctl_state;
		b[7] = Report_Total_Length;
		b[8] = IsMultiReport;
		b[9] = HasReports;
	}
	else
	{
		b[5] = 0;
		b[6] = 0;
		b[7] = 0;
		b[8] = 0;
		b[9] = 0;
	}
}
