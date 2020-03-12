namespace HardSense.GUI.Bluetooth
{
    partial class BluetoothConfigurator
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
            this.label_CommPort = new System.Windows.Forms.Label();
            this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
            this.comboBox_PortList = new System.Windows.Forms.ComboBox();
            this.groupBox_Bluetooth = new System.Windows.Forms.GroupBox();
            this.button_ReScanPorts = new System.Windows.Forms.Button();
            this.button_ConnectToPort = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.groupBox_HardSenseESPCurrentInfo = new System.Windows.Forms.GroupBox();
            this.textBox_CurrentServerPort = new System.Windows.Forms.TextBox();
            this.textBox_CurrentServerName = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox_CurrentSSID = new System.Windows.Forms.TextBox();
            this.textBox_CurrentPassword = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox_UpdateSettings = new System.Windows.Forms.GroupBox();
            this.button_UpdateServerInfoToESP = new System.Windows.Forms.Button();
            this.textBox_ServerPort = new System.Windows.Forms.TextBox();
            this.textBox_ServerName = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.button_UpdateNetworkInfoToESP = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_NewPasswordConfirm = new System.Windows.Forms.TextBox();
            this.textBox_NewPassword = new System.Windows.Forms.TextBox();
            this.textBox_NewSSID = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.button_Exit = new System.Windows.Forms.Button();
            this.button_DeleteTouchCalibrationFile = new System.Windows.Forms.Button();
            this.button_UpdateTime = new System.Windows.Forms.Button();
            this.backgroundWorker2 = new System.ComponentModel.BackgroundWorker();
            this.groupBox_DeviceIDs = new System.Windows.Forms.GroupBox();
            this.button_UpdateDeviceIDInfoToESP = new System.Windows.Forms.Button();
            this.textBox_NewBluetoothDeviceID = new System.Windows.Forms.TextBox();
            this.textBox_NewWifiDeviceID = new System.Windows.Forms.TextBox();
            this.textBox_CurrentBluetoothDeviceID = new System.Windows.Forms.TextBox();
            this.textBox_CurrentWiFIDeviceID = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox_HardSenseESP_ScreenLayouts = new System.Windows.Forms.GroupBox();
            this.groupBox_HomeScreenLayout = new System.Windows.Forms.GroupBox();
            this.button_Edit_HomeScreen_Layout = new System.Windows.Forms.Button();
            this.button_Upload_HomeScreen_Layout = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
            this.groupBox_Bluetooth.SuspendLayout();
            this.groupBox_HardSenseESPCurrentInfo.SuspendLayout();
            this.groupBox_UpdateSettings.SuspendLayout();
            this.groupBox_DeviceIDs.SuspendLayout();
            this.groupBox_HardSenseESP_ScreenLayouts.SuspendLayout();
            this.groupBox_HomeScreenLayout.SuspendLayout();
            this.SuspendLayout();
            // 
            // label_CommPort
            // 
            this.label_CommPort.AutoSize = true;
            this.label_CommPort.Location = new System.Drawing.Point(6, 22);
            this.label_CommPort.Name = "label_CommPort";
            this.label_CommPort.Size = new System.Drawing.Size(26, 13);
            this.label_CommPort.TabIndex = 0;
            this.label_CommPort.Text = "Port";
            // 
            // comboBox_PortList
            // 
            this.comboBox_PortList.FormattingEnabled = true;
            this.comboBox_PortList.Location = new System.Drawing.Point(38, 19);
            this.comboBox_PortList.Name = "comboBox_PortList";
            this.comboBox_PortList.Size = new System.Drawing.Size(128, 21);
            this.comboBox_PortList.TabIndex = 1;
            // 
            // groupBox_Bluetooth
            // 
            this.groupBox_Bluetooth.Controls.Add(this.button_ReScanPorts);
            this.groupBox_Bluetooth.Controls.Add(this.button_ConnectToPort);
            this.groupBox_Bluetooth.Controls.Add(this.comboBox_PortList);
            this.groupBox_Bluetooth.Controls.Add(this.label_CommPort);
            this.groupBox_Bluetooth.Location = new System.Drawing.Point(12, 12);
            this.groupBox_Bluetooth.Name = "groupBox_Bluetooth";
            this.groupBox_Bluetooth.Size = new System.Drawing.Size(177, 79);
            this.groupBox_Bluetooth.TabIndex = 2;
            this.groupBox_Bluetooth.TabStop = false;
            this.groupBox_Bluetooth.Text = "Bluetooth";
            // 
            // button_ReScanPorts
            // 
            this.button_ReScanPorts.Location = new System.Drawing.Point(91, 45);
            this.button_ReScanPorts.Name = "button_ReScanPorts";
            this.button_ReScanPorts.Size = new System.Drawing.Size(75, 23);
            this.button_ReScanPorts.TabIndex = 3;
            this.button_ReScanPorts.Text = "Re-Scan";
            this.button_ReScanPorts.UseVisualStyleBackColor = true;
            this.button_ReScanPorts.Click += new System.EventHandler(this.button_ReScanPorts_Click);
            // 
            // button_ConnectToPort
            // 
            this.button_ConnectToPort.Location = new System.Drawing.Point(9, 46);
            this.button_ConnectToPort.Name = "button_ConnectToPort";
            this.button_ConnectToPort.Size = new System.Drawing.Size(75, 23);
            this.button_ConnectToPort.TabIndex = 2;
            this.button_ConnectToPort.Text = "Connect";
            this.button_ConnectToPort.UseVisualStyleBackColor = true;
            this.button_ConnectToPort.Click += new System.EventHandler(this.button_ConnectToPort_Click);
            // 
            // groupBox_HardSenseESPCurrentInfo
            // 
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.textBox_CurrentServerPort);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.textBox_CurrentServerName);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.label8);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.label9);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.textBox_CurrentSSID);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.textBox_CurrentPassword);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.label2);
            this.groupBox_HardSenseESPCurrentInfo.Controls.Add(this.label1);
            this.groupBox_HardSenseESPCurrentInfo.Location = new System.Drawing.Point(12, 97);
            this.groupBox_HardSenseESPCurrentInfo.Name = "groupBox_HardSenseESPCurrentInfo";
            this.groupBox_HardSenseESPCurrentInfo.Size = new System.Drawing.Size(177, 135);
            this.groupBox_HardSenseESPCurrentInfo.TabIndex = 3;
            this.groupBox_HardSenseESPCurrentInfo.TabStop = false;
            this.groupBox_HardSenseESPCurrentInfo.Text = "HardSenseESP";
            // 
            // textBox_CurrentServerPort
            // 
            this.textBox_CurrentServerPort.Location = new System.Drawing.Point(71, 105);
            this.textBox_CurrentServerPort.Name = "textBox_CurrentServerPort";
            this.textBox_CurrentServerPort.ReadOnly = true;
            this.textBox_CurrentServerPort.Size = new System.Drawing.Size(100, 20);
            this.textBox_CurrentServerPort.TabIndex = 14;
            // 
            // textBox_CurrentServerName
            // 
            this.textBox_CurrentServerName.Location = new System.Drawing.Point(72, 79);
            this.textBox_CurrentServerName.Name = "textBox_CurrentServerName";
            this.textBox_CurrentServerName.ReadOnly = true;
            this.textBox_CurrentServerName.Size = new System.Drawing.Size(100, 20);
            this.textBox_CurrentServerName.TabIndex = 13;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 108);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(26, 13);
            this.label8.TabIndex = 12;
            this.label8.Text = "Port";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 82);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(38, 13);
            this.label9.TabIndex = 11;
            this.label9.Text = "Server";
            // 
            // textBox_CurrentSSID
            // 
            this.textBox_CurrentSSID.Location = new System.Drawing.Point(72, 17);
            this.textBox_CurrentSSID.Name = "textBox_CurrentSSID";
            this.textBox_CurrentSSID.ReadOnly = true;
            this.textBox_CurrentSSID.Size = new System.Drawing.Size(100, 20);
            this.textBox_CurrentSSID.TabIndex = 3;
            // 
            // textBox_CurrentPassword
            // 
            this.textBox_CurrentPassword.Location = new System.Drawing.Point(72, 43);
            this.textBox_CurrentPassword.Name = "textBox_CurrentPassword";
            this.textBox_CurrentPassword.ReadOnly = true;
            this.textBox_CurrentPassword.Size = new System.Drawing.Size(100, 20);
            this.textBox_CurrentPassword.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Password";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "SSID";
            // 
            // groupBox_UpdateSettings
            // 
            this.groupBox_UpdateSettings.Controls.Add(this.button_UpdateServerInfoToESP);
            this.groupBox_UpdateSettings.Controls.Add(this.textBox_ServerPort);
            this.groupBox_UpdateSettings.Controls.Add(this.textBox_ServerName);
            this.groupBox_UpdateSettings.Controls.Add(this.label7);
            this.groupBox_UpdateSettings.Controls.Add(this.label6);
            this.groupBox_UpdateSettings.Controls.Add(this.button_UpdateNetworkInfoToESP);
            this.groupBox_UpdateSettings.Controls.Add(this.label5);
            this.groupBox_UpdateSettings.Controls.Add(this.textBox_NewPasswordConfirm);
            this.groupBox_UpdateSettings.Controls.Add(this.textBox_NewPassword);
            this.groupBox_UpdateSettings.Controls.Add(this.textBox_NewSSID);
            this.groupBox_UpdateSettings.Controls.Add(this.label3);
            this.groupBox_UpdateSettings.Controls.Add(this.label4);
            this.groupBox_UpdateSettings.Location = new System.Drawing.Point(195, 12);
            this.groupBox_UpdateSettings.Name = "groupBox_UpdateSettings";
            this.groupBox_UpdateSettings.Size = new System.Drawing.Size(177, 220);
            this.groupBox_UpdateSettings.TabIndex = 4;
            this.groupBox_UpdateSettings.TabStop = false;
            this.groupBox_UpdateSettings.Text = "Update Settings";
            // 
            // button_UpdateServerInfoToESP
            // 
            this.button_UpdateServerInfoToESP.Location = new System.Drawing.Point(9, 188);
            this.button_UpdateServerInfoToESP.Name = "button_UpdateServerInfoToESP";
            this.button_UpdateServerInfoToESP.Size = new System.Drawing.Size(157, 23);
            this.button_UpdateServerInfoToESP.TabIndex = 13;
            this.button_UpdateServerInfoToESP.Text = "Update Server Info";
            this.button_UpdateServerInfoToESP.UseVisualStyleBackColor = true;
            this.button_UpdateServerInfoToESP.Click += new System.EventHandler(this.button_UpdateServerInfoToESP_Click);
            // 
            // textBox_ServerPort
            // 
            this.textBox_ServerPort.Location = new System.Drawing.Point(66, 163);
            this.textBox_ServerPort.Name = "textBox_ServerPort";
            this.textBox_ServerPort.Size = new System.Drawing.Size(100, 20);
            this.textBox_ServerPort.TabIndex = 12;
            // 
            // textBox_ServerName
            // 
            this.textBox_ServerName.Location = new System.Drawing.Point(66, 137);
            this.textBox_ServerName.Name = "textBox_ServerName";
            this.textBox_ServerName.Size = new System.Drawing.Size(100, 20);
            this.textBox_ServerName.TabIndex = 11;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 165);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(26, 13);
            this.label7.TabIndex = 10;
            this.label7.Text = "Port";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 140);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Server";
            // 
            // button_UpdateNetworkInfoToESP
            // 
            this.button_UpdateNetworkInfoToESP.Location = new System.Drawing.Point(9, 101);
            this.button_UpdateNetworkInfoToESP.Name = "button_UpdateNetworkInfoToESP";
            this.button_UpdateNetworkInfoToESP.Size = new System.Drawing.Size(157, 23);
            this.button_UpdateNetworkInfoToESP.TabIndex = 8;
            this.button_UpdateNetworkInfoToESP.Text = "Update Network Info";
            this.button_UpdateNetworkInfoToESP.UseVisualStyleBackColor = true;
            this.button_UpdateNetworkInfoToESP.Click += new System.EventHandler(this.button_UpdateNetworkInfoToESP_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 78);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(42, 13);
            this.label5.TabIndex = 7;
            this.label5.Text = "Confirm";
            // 
            // textBox_NewPasswordConfirm
            // 
            this.textBox_NewPasswordConfirm.Location = new System.Drawing.Point(66, 75);
            this.textBox_NewPasswordConfirm.Name = "textBox_NewPasswordConfirm";
            this.textBox_NewPasswordConfirm.Size = new System.Drawing.Size(100, 20);
            this.textBox_NewPasswordConfirm.TabIndex = 6;
            this.textBox_NewPasswordConfirm.UseSystemPasswordChar = true;
            // 
            // textBox_NewPassword
            // 
            this.textBox_NewPassword.Location = new System.Drawing.Point(66, 48);
            this.textBox_NewPassword.Name = "textBox_NewPassword";
            this.textBox_NewPassword.Size = new System.Drawing.Size(100, 20);
            this.textBox_NewPassword.TabIndex = 5;
            this.textBox_NewPassword.UseSystemPasswordChar = true;
            // 
            // textBox_NewSSID
            // 
            this.textBox_NewSSID.Location = new System.Drawing.Point(66, 20);
            this.textBox_NewSSID.Name = "textBox_NewSSID";
            this.textBox_NewSSID.Size = new System.Drawing.Size(100, 20);
            this.textBox_NewSSID.TabIndex = 4;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 51);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Password";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 25);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(32, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "SSID";
            // 
            // button_Exit
            // 
            this.button_Exit.Location = new System.Drawing.Point(297, 347);
            this.button_Exit.Name = "button_Exit";
            this.button_Exit.Size = new System.Drawing.Size(75, 23);
            this.button_Exit.TabIndex = 5;
            this.button_Exit.Text = "Exit";
            this.button_Exit.UseVisualStyleBackColor = true;
            this.button_Exit.Click += new System.EventHandler(this.button_Exit_Click);
            // 
            // button_DeleteTouchCalibrationFile
            // 
            this.button_DeleteTouchCalibrationFile.Location = new System.Drawing.Point(12, 346);
            this.button_DeleteTouchCalibrationFile.Name = "button_DeleteTouchCalibrationFile";
            this.button_DeleteTouchCalibrationFile.Size = new System.Drawing.Size(103, 23);
            this.button_DeleteTouchCalibrationFile.TabIndex = 6;
            this.button_DeleteTouchCalibrationFile.Text = "Delete Calibration";
            this.button_DeleteTouchCalibrationFile.UseVisualStyleBackColor = true;
            this.button_DeleteTouchCalibrationFile.Click += new System.EventHandler(this.button_DeleteTouchCalibrationFile_Click);
            // 
            // button_UpdateTime
            // 
            this.button_UpdateTime.Location = new System.Drawing.Point(122, 346);
            this.button_UpdateTime.Name = "button_UpdateTime";
            this.button_UpdateTime.Size = new System.Drawing.Size(83, 23);
            this.button_UpdateTime.TabIndex = 7;
            this.button_UpdateTime.Text = "Update Time";
            this.button_UpdateTime.UseVisualStyleBackColor = true;
            this.button_UpdateTime.Click += new System.EventHandler(this.button_UpdateTime_Click);
            // 
            // groupBox_DeviceIDs
            // 
            this.groupBox_DeviceIDs.Controls.Add(this.button_UpdateDeviceIDInfoToESP);
            this.groupBox_DeviceIDs.Controls.Add(this.textBox_NewBluetoothDeviceID);
            this.groupBox_DeviceIDs.Controls.Add(this.textBox_NewWifiDeviceID);
            this.groupBox_DeviceIDs.Controls.Add(this.textBox_CurrentBluetoothDeviceID);
            this.groupBox_DeviceIDs.Controls.Add(this.textBox_CurrentWiFIDeviceID);
            this.groupBox_DeviceIDs.Controls.Add(this.label11);
            this.groupBox_DeviceIDs.Controls.Add(this.label10);
            this.groupBox_DeviceIDs.Location = new System.Drawing.Point(12, 239);
            this.groupBox_DeviceIDs.Name = "groupBox_DeviceIDs";
            this.groupBox_DeviceIDs.Size = new System.Drawing.Size(359, 101);
            this.groupBox_DeviceIDs.TabIndex = 8;
            this.groupBox_DeviceIDs.TabStop = false;
            this.groupBox_DeviceIDs.Text = "Device IDs";
            // 
            // button_UpdateDeviceIDInfoToESP
            // 
            this.button_UpdateDeviceIDInfoToESP.Location = new System.Drawing.Point(215, 72);
            this.button_UpdateDeviceIDInfoToESP.Name = "button_UpdateDeviceIDInfoToESP";
            this.button_UpdateDeviceIDInfoToESP.Size = new System.Drawing.Size(128, 23);
            this.button_UpdateDeviceIDInfoToESP.TabIndex = 6;
            this.button_UpdateDeviceIDInfoToESP.Text = "Update Device IDs";
            this.button_UpdateDeviceIDInfoToESP.UseVisualStyleBackColor = true;
            this.button_UpdateDeviceIDInfoToESP.Click += new System.EventHandler(this.button_UpdateDeviceIDInfoToESP_Click);
            // 
            // textBox_NewBluetoothDeviceID
            // 
            this.textBox_NewBluetoothDeviceID.Location = new System.Drawing.Point(215, 45);
            this.textBox_NewBluetoothDeviceID.Name = "textBox_NewBluetoothDeviceID";
            this.textBox_NewBluetoothDeviceID.Size = new System.Drawing.Size(128, 20);
            this.textBox_NewBluetoothDeviceID.TabIndex = 5;
            // 
            // textBox_NewWifiDeviceID
            // 
            this.textBox_NewWifiDeviceID.Location = new System.Drawing.Point(215, 19);
            this.textBox_NewWifiDeviceID.Name = "textBox_NewWifiDeviceID";
            this.textBox_NewWifiDeviceID.Size = new System.Drawing.Size(128, 20);
            this.textBox_NewWifiDeviceID.TabIndex = 4;
            // 
            // textBox_CurrentBluetoothDeviceID
            // 
            this.textBox_CurrentBluetoothDeviceID.Location = new System.Drawing.Point(66, 46);
            this.textBox_CurrentBluetoothDeviceID.Name = "textBox_CurrentBluetoothDeviceID";
            this.textBox_CurrentBluetoothDeviceID.ReadOnly = true;
            this.textBox_CurrentBluetoothDeviceID.Size = new System.Drawing.Size(143, 20);
            this.textBox_CurrentBluetoothDeviceID.TabIndex = 3;
            // 
            // textBox_CurrentWiFIDeviceID
            // 
            this.textBox_CurrentWiFIDeviceID.Location = new System.Drawing.Point(66, 20);
            this.textBox_CurrentWiFIDeviceID.Name = "textBox_CurrentWiFIDeviceID";
            this.textBox_CurrentWiFIDeviceID.ReadOnly = true;
            this.textBox_CurrentWiFIDeviceID.Size = new System.Drawing.Size(143, 20);
            this.textBox_CurrentWiFIDeviceID.TabIndex = 2;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(9, 53);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(52, 13);
            this.label11.TabIndex = 1;
            this.label11.Text = "Bluetooth";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 20);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(25, 13);
            this.label10.TabIndex = 0;
            this.label10.Text = "Wifi";
            // 
            // groupBox_HardSenseESP_ScreenLayouts
            // 
            this.groupBox_HardSenseESP_ScreenLayouts.Controls.Add(this.groupBox_HomeScreenLayout);
            this.groupBox_HardSenseESP_ScreenLayouts.Location = new System.Drawing.Point(378, 12);
            this.groupBox_HardSenseESP_ScreenLayouts.Name = "groupBox_HardSenseESP_ScreenLayouts";
            this.groupBox_HardSenseESP_ScreenLayouts.Size = new System.Drawing.Size(587, 328);
            this.groupBox_HardSenseESP_ScreenLayouts.TabIndex = 9;
            this.groupBox_HardSenseESP_ScreenLayouts.TabStop = false;
            this.groupBox_HardSenseESP_ScreenLayouts.Text = "HardSenseESP Screen Layouts";
            // 
            // groupBox_HomeScreenLayout
            // 
            this.groupBox_HomeScreenLayout.Controls.Add(this.button_Upload_HomeScreen_Layout);
            this.groupBox_HomeScreenLayout.Controls.Add(this.button_Edit_HomeScreen_Layout);
            this.groupBox_HomeScreenLayout.Location = new System.Drawing.Point(6, 24);
            this.groupBox_HomeScreenLayout.Name = "groupBox_HomeScreenLayout";
            this.groupBox_HomeScreenLayout.Size = new System.Drawing.Size(172, 55);
            this.groupBox_HomeScreenLayout.TabIndex = 0;
            this.groupBox_HomeScreenLayout.TabStop = false;
            this.groupBox_HomeScreenLayout.Text = "Home Screen";
            // 
            // button_Edit_HomeScreen_Layout
            // 
            this.button_Edit_HomeScreen_Layout.Location = new System.Drawing.Point(6, 22);
            this.button_Edit_HomeScreen_Layout.Name = "button_Edit_HomeScreen_Layout";
            this.button_Edit_HomeScreen_Layout.Size = new System.Drawing.Size(75, 23);
            this.button_Edit_HomeScreen_Layout.TabIndex = 0;
            this.button_Edit_HomeScreen_Layout.Text = "Edit";
            this.button_Edit_HomeScreen_Layout.UseVisualStyleBackColor = true;
            this.button_Edit_HomeScreen_Layout.Click += new System.EventHandler(this.button_Edit_HomeScreen_Layout_Click);
            // 
            // button_Upload_HomeScreen_Layout
            // 
            this.button_Upload_HomeScreen_Layout.Location = new System.Drawing.Point(87, 22);
            this.button_Upload_HomeScreen_Layout.Name = "button_Upload_HomeScreen_Layout";
            this.button_Upload_HomeScreen_Layout.Size = new System.Drawing.Size(75, 23);
            this.button_Upload_HomeScreen_Layout.TabIndex = 1;
            this.button_Upload_HomeScreen_Layout.Text = "Upload";
            this.button_Upload_HomeScreen_Layout.UseVisualStyleBackColor = true;
            // 
            // BluetoothConfigurator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(996, 380);
            this.Controls.Add(this.groupBox_HardSenseESP_ScreenLayouts);
            this.Controls.Add(this.groupBox_DeviceIDs);
            this.Controls.Add(this.button_UpdateTime);
            this.Controls.Add(this.button_DeleteTouchCalibrationFile);
            this.Controls.Add(this.button_Exit);
            this.Controls.Add(this.groupBox_UpdateSettings);
            this.Controls.Add(this.groupBox_HardSenseESPCurrentInfo);
            this.Controls.Add(this.groupBox_Bluetooth);
            this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::HardSense.Properties.Settings.Default, "BluetoothConfiguratorFormLastPosition", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.Location = global::HardSense.Properties.Settings.Default.BluetoothConfiguratorFormLastPosition;
            this.Name = "BluetoothConfigurator";
            this.Text = "Configurator";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.BluetoothConfigurator_FormClosed);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
            this.groupBox_Bluetooth.ResumeLayout(false);
            this.groupBox_Bluetooth.PerformLayout();
            this.groupBox_HardSenseESPCurrentInfo.ResumeLayout(false);
            this.groupBox_HardSenseESPCurrentInfo.PerformLayout();
            this.groupBox_UpdateSettings.ResumeLayout(false);
            this.groupBox_UpdateSettings.PerformLayout();
            this.groupBox_DeviceIDs.ResumeLayout(false);
            this.groupBox_DeviceIDs.PerformLayout();
            this.groupBox_HardSenseESP_ScreenLayouts.ResumeLayout(false);
            this.groupBox_HomeScreenLayout.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label_CommPort;
        private System.Windows.Forms.BindingSource bindingSource1;
        private System.Windows.Forms.ComboBox comboBox_PortList;
        private System.Windows.Forms.GroupBox groupBox_Bluetooth;
        private System.Windows.Forms.Button button_ReScanPorts;
        private System.Windows.Forms.Button button_ConnectToPort;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.GroupBox groupBox_HardSenseESPCurrentInfo;
        private System.Windows.Forms.TextBox textBox_CurrentSSID;
        private System.Windows.Forms.TextBox textBox_CurrentPassword;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox_UpdateSettings;
        private System.Windows.Forms.Button button_UpdateNetworkInfoToESP;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_NewPasswordConfirm;
        private System.Windows.Forms.TextBox textBox_NewPassword;
        private System.Windows.Forms.TextBox textBox_NewSSID;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button_Exit;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox_CurrentServerPort;
        private System.Windows.Forms.TextBox textBox_CurrentServerName;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button button_UpdateServerInfoToESP;
        private System.Windows.Forms.TextBox textBox_ServerPort;
        private System.Windows.Forms.TextBox textBox_ServerName;
        private System.Windows.Forms.Button button_DeleteTouchCalibrationFile;
        private System.Windows.Forms.Button button_UpdateTime;
        private System.ComponentModel.BackgroundWorker backgroundWorker2;
        private System.Windows.Forms.GroupBox groupBox_DeviceIDs;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button button_UpdateDeviceIDInfoToESP;
        private System.Windows.Forms.TextBox textBox_NewBluetoothDeviceID;
        private System.Windows.Forms.TextBox textBox_NewWifiDeviceID;
        private System.Windows.Forms.TextBox textBox_CurrentBluetoothDeviceID;
        private System.Windows.Forms.TextBox textBox_CurrentWiFIDeviceID;
        private System.Windows.Forms.GroupBox groupBox_HardSenseESP_ScreenLayouts;
        private System.Windows.Forms.GroupBox groupBox_HomeScreenLayout;
        private System.Windows.Forms.Button button_Upload_HomeScreen_Layout;
        private System.Windows.Forms.Button button_Edit_HomeScreen_Layout;
    }
}