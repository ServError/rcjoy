#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>

#include "SysTimer.h"
#include "PWMReader.h"
#include "PPMGenerator.h"

int main()
{
  StartSysTimer();
  
  initCapture();
  startCapture();
  
  initPPMGenerator(8);
  startPPMGenerator();

  for (;;)
  {
  }
}
