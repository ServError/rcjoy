﻿namespace Tahorg.RCJoyGUI.ProgrammingPanels
{
    partial class FMPPMOutDesignPanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lnkButton = new Tahorg.RCJoyGUI.LinkPoint();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelHead
            // 
            this.labelHead.Size = new System.Drawing.Size(137, 13);
            this.labelHead.Text = "Mode PPM Output";
            // 
            // lnkButton
            // 
            this.lnkButton.DataMapIdx = ((short)(0));
            this.lnkButton.Direction = Tahorg.RCJoyGUI.enLinkDirection.Input;
            this.lnkButton.HolderPanel = null;
            this.lnkButton.IsSelected = false;
            this.lnkButton.LinkType = Tahorg.RCJoyGUI.enLinkType.Button;
            this.lnkButton.Location = new System.Drawing.Point(1, 20);
            this.lnkButton.MappedValueName = null;
            this.lnkButton.Name = "lnkButton";
            this.lnkButton.Size = new System.Drawing.Size(12, 12);
            this.lnkButton.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Select button";
            // 
            // FMPPMOutDesignPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lnkButton);
            this.Name = "FMPPMOutDesignPanel";
            this.Size = new System.Drawing.Size(137, 199);
            this.Title = "Mode PPM Output";
            this.Controls.SetChildIndex(this.labelHead, 0);
            this.Controls.SetChildIndex(this.lnkButton, 0);
            this.Controls.SetChildIndex(this.label1, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private LinkPoint lnkButton;
        private System.Windows.Forms.Label label1;
    }
}
