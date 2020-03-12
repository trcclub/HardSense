using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;
using HardSense.DataStreamingServer;

namespace HardSense.GUI.Bluetooth
{
    public partial class BluetoothConfigurator : Form
    {
        SerialPort btPort = null;
        bool connected = false;
        bool running = false;
        private Thread readThread;
        private Thread writeThread;

        private StringBuilder message = new StringBuilder();
        private Mutex dataLock = new Mutex();
        public BluetoothConfigurator()
        {
            InitializeComponent();
            readThread = new Thread(readThreadProc);
            writeThread = new Thread(writeThreadProc);
            groupBox_UpdateSettings.Enabled = false;
            groupBox_HardSenseESPCurrentInfo.Enabled = false;
            groupBox_DeviceIDs.Enabled = false;
            button_DeleteTouchCalibrationFile.Enabled = false;
            button_UpdateTime.Enabled = false;

            GetAvailablePorts();
        }
        
        private void BluetoothConfigurator_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.Save();
            Stop();
        }

        private void button_Exit_Click(object sender, EventArgs e)
        {
            this.Close();

        }

        private void button_ConnectToPort_Click(object sender, EventArgs e)
        {
            if(comboBox_PortList.Text == "")
            {
                string message = "You must select a port";
                string caption = "Missing port";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
                return;
            }

            Properties.Settings.Default.DefaultBTSerialPort = comboBox_PortList.Text;

            ConnectToHardSenseESP(comboBox_PortList.Text);
        }
        private void Start()
        {
            running = true;
            try
            {
                readThread.Start();
            }
            catch (Exception e)
            {
                string message = "ReadThread Exception: " + e.Message;
                string caption = "ReadThread Exception";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
            }

            writeThread.Start();
        }

        public void Stop()
        {
            if (running)
            {
                if (connected)
                {
                    AddKeyToMessage((char)TRANS__KEY.DISCONNECT);
                    Thread.Sleep(50); //Give the write thread enough time to send.
                }

                running = false;
                if (readThread.IsAlive)
                {
                    readThread.Join();
                }
                if (writeThread.IsAlive)
                {
                    writeThread.Join();
                }
            }

            if (btPort != null && btPort.IsOpen)
            {
                btPort.Close();
            }
            connected = false;
            message.Clear();
        }

        private void GetAvailablePorts()
        {
            comboBox_PortList.Items.Clear();
            string[] availablePorts = SerialPort.GetPortNames(); ;
            foreach (string currPort in availablePorts)
            {
                comboBox_PortList.Items.Add(currPort);
                comboBox_PortList.SelectedItem = Properties.Settings.Default.DefaultBTSerialPort;
            }
        }


        public bool ConnectToHardSenseESP(string portID)
        {
            btPort = new SerialPort(portID);
            btPort.BaudRate = 115200;
            btPort.Parity = Parity.None;
            btPort.StopBits = StopBits.One;
            btPort.DataBits = 8;
            btPort.Handshake = Handshake.None;

            btPort.ReadTimeout = 500;
            btPort.WriteTimeout = 500;

            int failCounter = 0;
            
            while (failCounter < Properties.Settings.Default.DefaultBTConnectMaxFailures)
            {
                try
                {
                    btPort.Open();
                }
                catch (Exception e)
                {
                    if (btPort.IsOpen)
                    {
                        break;
                    }
                    failCounter++;
                    if (Properties.Settings.Default.DebugOutput)
                    {
                        string message = "Connection Failed(" + failCounter.ToString() + "): " + e.Message;
                        string caption = "Bluetooth connect failed";
                        MessageBoxButtons buttons = MessageBoxButtons.OK;
                        MessageBox.Show(message, caption, buttons);
                    }
                }
            }

            connected = btPort.IsOpen;
            if (connected)
            {
                Start();
                Thread.Sleep(100);
                AddKeyToMessage((char)TRANS__KEY.CONNECTION_REQUEST);
                
                groupBox_Bluetooth.Enabled = false;
                groupBox_UpdateSettings.Enabled = true;
                groupBox_HardSenseESPCurrentInfo.Enabled = true;
                groupBox_DeviceIDs.Enabled = true;
                button_DeleteTouchCalibrationFile.Enabled = true;
                button_UpdateTime.Enabled = true;

                if (Properties.Settings.Default.DebugOutput)
                {
                    string message = "Successfully connected to HardSenseESP.";
                    string caption = "Bluetooth connect sucess";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                }
            } else
            {
                string message = "Failed to connect bluetooth to HardSenseESP.";
                string caption = "Bluetooth connect failed";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
            }
            return connected;
        }
        private void writeThreadProc()
        {
            try
            {
                while (running)
                {
                    SendData();
                    Thread.Sleep(50);
                }
            }
            catch (Exception e)
            {
            }
        }
        private void readThreadProc()
        {
                while (running)
                {
                try
                {
                    if (btPort.ReadByte() == (int)TRANS__KEY.STX)
                    {
                        string textinput = btPort.ReadTo(((char)TRANS__KEY.ETX).ToString());
                        parseInput(textinput);
                    }
                    Thread.Sleep(20);
                }

                catch (Exception e)
                {
                    if (e is InvalidOperationException || e is TimeoutException)
                    { 
                        continue;
                    }
                    string message = "ERROR: " + e.Message;
                    string caption = "ERROR";
                    MessageBoxButtons buttons = MessageBoxButtons.OK;
                    MessageBox.Show(message, caption, buttons);
                }
            }

        }

        private void parseInput(string inputData)
        {
            string strippedInput = inputData.Substring(0, inputData.Length - 1);
            string[] splitter = { ((char)TRANS__KEY.PACKET_END).ToString() };
            string[] tokens = inputData.Split(splitter, StringSplitOptions.RemoveEmptyEntries);
            {
                foreach (string currItem in tokens)
                {
                    char key = currItem.ToCharArray()[0];
                    string value = currItem.Substring(1);
                    DispatchRequest(key, value);
                }
            }
        }

        private void DispatchRequest(char key, string value)
        {
            switch (key)
            {
                case (char)TRANS__KEY.CONNECTION_ACK:
                    Trans_Accept_New_Connect();
                    break;
                case (char)TRANS__KEY.CURRENT_SSID:
                    Trans_Receive_SSID(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_PASSWORD_IS_SET:
                    Trans_Receive_Is_Password_Set(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_SERVER_HOSTNAME:
                    Trans_Receive_ServerName(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_SERVER_PORT:
                    Trans_Receive_ServerPort(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_WIFI_DEVICE_ID:
                    Trans_Receive_WifiDeviceId(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_BT_DEVICE_ID:
                    Trans_Receive_BtDeviceId(value);
                    break;
                case (char)TRANS__KEY.CONFIG_SSID_UPDATE_SUCCESS:
                    Trans_Receive_SSID_Update_Success(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_PASSWORD_UPDATE_SUCCESS:
                    Trans_Receive_Password_Update_Success(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS:
                    Trans_Receive_Server_Hostname_Update_Success(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_SERVER_PORT_UPDATE_SUCCESS:
                    Trans_Receive_Server_Port_Update_Success(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_WIFI_DEVICE_ID_UPDATE_SUCCESS:
                    CONFIG_WIFI_DEVICE_ID_UPDATE_SUCCESS(Boolean.Parse(value));
                    break;
                case (char)TRANS__KEY.CONFIG_BT_DEVICE_ID_UPDATE_SUCCESS:
                    CONFIG_BT_DEVICE_ID_UPDATE_SUCCESS(Boolean.Parse(value));
                    break;
                default:
                    break;
            }
            
            
        }

        private void Trans_Accept_New_Connect()
        {
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SSID);
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_IS_PASSWORD_SET);
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SERVER_HOSTNAME);
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SERVER_PORT);
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_WIFI_DEVICE_ID);
            AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_BT_DEVICE_ID);
        }
        private void Trans_Receive_SSID(string newSSID)
        {
            if (newSSID == "")
            {
                newSSID = "N/A";
            } 

            textBox_CurrentSSID.Invoke((Action)delegate
            {
                textBox_CurrentSSID.Text = newSSID;
            });

        }
        private void Trans_Receive_Is_Password_Set(bool val)
        {
            string value = "N/A";
            if(val)
            {
                value = "******";
            }

            textBox_CurrentPassword.Invoke((Action)delegate
            {
                textBox_CurrentPassword.Text = value;
            });
        }
        private void Trans_Receive_ServerName(string newServerName)
        {
            if (newServerName == "")
            {
                newServerName = "N/A";
            }

            textBox_CurrentServerName.Invoke((Action)delegate
            {
                textBox_CurrentServerName.Text = newServerName;
            });

        }
        private void Trans_Receive_ServerPort(string newServerPort)
        {
            if (newServerPort == "0")
            {
                newServerPort = "N/A";
            }

            textBox_CurrentServerPort.Invoke((Action)delegate
            {
                textBox_CurrentServerPort.Text = newServerPort;
            });

        }
        private void Trans_Receive_WifiDeviceId(string newWifiDeviceID)
        {
            if (newWifiDeviceID == "")
            {
                newWifiDeviceID = "N/A";
            }

            textBox_CurrentWiFIDeviceID.Invoke((Action)delegate
            {
                textBox_CurrentWiFIDeviceID.Text = newWifiDeviceID;
            });
        }
        private void Trans_Receive_BtDeviceId(string newBtDeviceID)
        {
            if (newBtDeviceID == "")
            {
                newBtDeviceID = "N/A";
            }

            textBox_CurrentBluetoothDeviceID.Invoke((Action)delegate
            {
                textBox_CurrentBluetoothDeviceID.Text = newBtDeviceID;
            });
        }
        private void Trans_Receive_SSID_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SSID);
            } else
            {
                MessageBoxPopUp("HardSenseESP failed to update the SSID", "HardSenseESP Failure");
            }
        }
        private void Trans_Receive_Password_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_IS_PASSWORD_SET);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the Password", "HardSenseESP Failure");
            }
        }
        private void Trans_Receive_Server_Hostname_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SERVER_HOSTNAME);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the Server Hostname", "HardSenseESP Failure");
            }
        }
        private void Trans_Receive_Server_Port_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_SERVER_PORT);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the Server Port", "HardSenseESP Failure");
            }
        }
        private void CONFIG_WIFI_DEVICE_ID_UPDATE_SUCCESS(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_WIFI_DEVICE_ID);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the WiFi Device ID", "HardSenseESP Failure");
            }
        }
        private void CONFIG_BT_DEVICE_ID_UPDATE_SUCCESS(bool result)
        {
            if (result)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_REQUEST_BT_DEVICE_ID);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the BT Device ID", "HardSenseESP Failure");
            }
        }

        private void MessageBoxPopUp(string message, string caption)
        {
            MessageBoxButtons buttons = MessageBoxButtons.OK;
            MessageBox.Show(message, caption, buttons);
        }

        public void AddKeyToMessage(char key)
        {
            AddStringToMessage(key, "");
        }
        public void AddStringToMessage(char key, string value)
        {
            int dataPacketLength = 2 + value.Length;

            if (dataPacketLength + message.Length > Properties.Settings.Default.DefaultMaxLengthDataToSend)
            {
                SendData();
            }

            StringBuilder tmpSB = new StringBuilder();
            tmpSB.Append(key);
            tmpSB.Append(value);
            tmpSB.Append((char)TRANS__KEY.PACKET_END);

            dataLock.WaitOne();
            message.Append(tmpSB.ToString());
            dataLock.ReleaseMutex();
        }
        public void SendData()
        {
            if (message.Length == 0)
            {
                return;
            }

            dataLock.WaitOne();
            StringBuilder tmpSB = new StringBuilder(message.ToString());
            message.Clear();
            dataLock.ReleaseMutex();

            tmpSB.Insert(0, (char)TRANS__KEY.STX);
            tmpSB.Append((char)TRANS__KEY.ETX);

            byte[] byteData = Encoding.ASCII.GetBytes(tmpSB.ToString());

            btPort.Write(byteData, 0, byteData.Length);
        }

        private void button_ReScanPorts_Click(object sender, EventArgs e)
        {
            GetAvailablePorts();
        }

        private void button_UpdateNetworkInfoToESP_Click(object sender, EventArgs e)
        {
            string newSSID = textBox_NewSSID.Text;
            string newPassword = textBox_NewPassword.Text;
            string newPasswordConfirm = textBox_NewPasswordConfirm.Text;
            if(!newPassword.Equals(newPasswordConfirm))
            {
                string message = "The passwords do not match.\nPlease try again.";
                string caption = "Password Mis-Match";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show(message, caption, buttons);
                return;
            }
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_SSID, newSSID);
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_PASSWORD, newPassword);
            textBox_NewSSID.Clear();
            textBox_NewPassword.Clear();
            textBox_NewPasswordConfirm.Clear();
        }

        private void button_UpdateServerInfoToESP_Click(object sender, EventArgs e)
        {
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_SERVER_HOSTNAME, textBox_ServerName.Text);
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_SERVER_PORT, textBox_ServerPort.Text);
            textBox_ServerName.Clear();
            textBox_ServerPort.Clear();
        }

        private void button_DeleteTouchCalibrationFile_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to delete the touch calibration file?", "Confirm Delete", MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                AddKeyToMessage((char)TRANS__KEY.CONFIG_DELETE_TOUCH_CALIBRATION_FILE);
            }
        }

        private void button_UpdateTime_Click(object sender, EventArgs e)
        {
            DateTime localDate = DateTime.Now;
            String dt = localDate.ToString("MMM dd yyyy,HH:mm:ss");
            AddStringToMessage((char)TRANS__KEY.CONFIG_UPDATE_TIME, dt);
        }

        private void button_UpdateDeviceIDInfoToESP_Click(object sender, EventArgs e)
        {
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_WIFI_DEVICED_ID, textBox_NewWifiDeviceID.Text);
            AddStringToMessage((char)TRANS__KEY.CONFIG_SET_BT_DEVICE_ID, textBox_NewBluetoothDeviceID.Text);
            textBox_NewWifiDeviceID.Clear();
            textBox_NewBluetoothDeviceID.Clear();
        }
    }
}

