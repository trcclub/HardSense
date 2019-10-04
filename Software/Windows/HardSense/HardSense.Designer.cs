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
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.hardwareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mainboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.listView1 = new System.Windows.Forms.ListView();
            this.Component_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Name_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ID_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Sensor_Column = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // notifyIcon2
            // 
            this.notifyIcon2.Text = "notifyIcon2";
            this.notifyIcon2.Visible = true;
            // 
            // tempDisplayBox
            // 
            this.tempDisplayBox.Location = new System.Drawing.Point(845, 107);
            this.tempDisplayBox.Name = "tempDisplayBox";
            this.tempDisplayBox.Size = new System.Drawing.Size(465, 398);
            this.tempDisplayBox.TabIndex = 0;
            this.tempDisplayBox.Text = "";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.hardwareToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1393, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // hardwareToolStripMenuItem
            // 
            this.hardwareToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainboardToolStripMenuItem});
            this.hardwareToolStripMenuItem.Name = "hardwareToolStripMenuItem";
            this.hardwareToolStripMenuItem.Size = new System.Drawing.Size(70, 20);
            this.hardwareToolStripMenuItem.Text = "Hardware";
            // 
            // mainboardToolStripMenuItem
            // 
            this.mainboardToolStripMenuItem.Name = "mainboardToolStripMenuItem";
            this.mainboardToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.mainboardToolStripMenuItem.Text = "Mainboard";
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Component_Column,
            this.Name_Column,
            this.ID_Column,
            this.Sensor_Column});
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(29, 69);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(768, 242);
            this.listView1.TabIndex = 2;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // Component_Column
            // 
            this.Component_Column.Text = "Component";
            this.Component_Column.Width = 91;
            // 
            // Name_Column
            // 
            this.Name_Column.Text = "Name";
            this.Name_Column.Width = 123;
            // 
            // ID_Column
            // 
            this.ID_Column.Text = "ID";
            // 
            // Sensor_Column
            // 
            this.Sensor_Column.Text = "Sensor";
            // 
            // HardSense
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1393, 684);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.tempDisplayBox);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "HardSense";
            this.Text = "HardSense";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NotifyIcon notifyIcon2;
        private System.Windows.Forms.RichTextBox tempDisplayBox;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem hardwareToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mainboardToolStripMenuItem;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader Name_Column;
        private System.Windows.Forms.ColumnHeader Component_Column;
        private System.Windows.Forms.ColumnHeader ID_Column;
        private System.Windows.Forms.ColumnHeader Sensor_Column;
    }
}

