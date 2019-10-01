namespace HardSense
{
    partial class HardSense
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HardSense));
            this.notifyIcon2 = new System.Windows.Forms.NotifyIcon(this.components);
            this.tempDisplayBox = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // notifyIcon2
            // 
            this.notifyIcon2.Text = "notifyIcon2";
            this.notifyIcon2.Visible = true;
            // 
            // tempDisplayBox
            // 
            this.tempDisplayBox.Location = new System.Drawing.Point(130, 107);
            this.tempDisplayBox.Name = "tempDisplayBox";
            this.tempDisplayBox.Size = new System.Drawing.Size(1180, 396);
            this.tempDisplayBox.TabIndex = 0;
            this.tempDisplayBox.Text = "";
            // 
            // HardSense
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1393, 684);
            this.Controls.Add(this.tempDisplayBox);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "HardSense";
            this.Text = "HardSense";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon notifyIcon2;
        private System.Windows.Forms.RichTextBox tempDisplayBox;
    }
}

