namespace HardSense.GUI
{
    partial class SensorMonitor
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
            this.treeView_SensorList = new System.Windows.Forms.TreeView();
            this.SuspendLayout();
            // 
            // treeView_SensorList
            // 
            this.treeView_SensorList.Location = new System.Drawing.Point(12, 12);
            this.treeView_SensorList.Name = "treeView_SensorList";
            this.treeView_SensorList.Size = new System.Drawing.Size(526, 687);
            this.treeView_SensorList.TabIndex = 0;
            // 
            // SensorMonitor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(552, 708);
            this.Controls.Add(this.treeView_SensorList);
            this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::HardSense.Properties.Settings.Default, "SensorMonitor_WindowsLastPosition", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.Location = global::HardSense.Properties.Settings.Default.SensorMonitor_WindowsLastPosition;
            this.Name = "SensorMonitor";
            this.Text = "SensorMonitor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SensorMonitor_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeView_SensorList;
    }
}