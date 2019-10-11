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
        Thread readThread;
        private Thread writeThread;

        private StringBuilder message = new StringBuilder();
        private Mutex dataLock = new Mutex();
        public BluetoothConfigurator()
        {
            InitializeComponent();
            ProtocolKeys.init();
            readThread = new Thread(readThreadProc);
            writeThread = new Thread(writeThreadProc);
            groupBox_UpdateSettings.Enabled = false;
            groupBox_HardSenseESPCurrentInfo.Enabled = false;

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
            readThread.Start();
            writeThread.Start();
        }

        public void Stop()
        {
            if (running)
            {
                if (connected)
                {
                    AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["DISCONNECT"]);
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
                AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONNECTION_REQUEST"]);
                
                groupBox_Bluetooth.Enabled = false;
                groupBox_UpdateSettings.Enabled = true;
                groupBox_HardSenseESPCurrentInfo.Enabled = true;

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
                        string textinput = btPort.ReadTo(ProtocolKeys.TRANSMISSION_KEYS["ETX"].ToString());
                        parseInput(textinput);
                    }
                    Thread.Sleep(20);
                }

                catch (Exception e)
                {
                    if (e is InvalidOperationException || e is TimeoutException)
                    { 
                        return;
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
            string[] splitter = { ProtocolKeys.TRANSMISSION_KEYS["PACKET_END"].ToString() };
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
                    Trans_Receive_Is_Password_Set(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_SERVER_HOSTNAME:
                    Trans_Receive_ServerName(value);
                    break;
                case (char)TRANS__KEY.CONFIG_CURRENT_SERVER_PORT:
                    Trans_Receive_ServerPort(value);
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
                default:
                    break;
            }
            
            
        }

        private void Trans_Accept_New_Connect()
        {
            AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SSID"]);
            AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_IS_PASSWORD_SET"]);
            AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SERVER_HOSTNAME"]);
            AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SERVER_PORT"]);
        }
        private void Trans_Receive_SSID(string newSSID)
        {
            if (newSSID == "")
            {
                newSSID = "N/A";
            } 

            textBox_CurrentSSID.Invoke((Action)delegate
            {
                textBox_CurrentSSID.AppendText(newSSID);
            });

        }
        private void Trans_Receive_Is_Password_Set(string val)
        {
            string value = "N/A";
            if(val == "1")
            {
                value = "******";
            }

            textBox_CurrentPassword.Invoke((Action)delegate
            {
                textBox_CurrentPassword.AppendText(value);
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
                textBox_CurrentServerName.AppendText(newServerName);
            });

        }
        private void Trans_Receive_ServerPort(string newServerPort)
        {
            if (newServerPort == "")
            {
                newServerPort = "N/A";
            }

            textBox_CurrentServerPort.Invoke((Action)delegate
            {
                textBox_CurrentServerPort.AppendText(newServerPort);
            });

        }
        private void Trans_Receive_SSID_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SSID"]);
            } else
            {
                MessageBoxPopUp("HardSenseESP failed to update the SSID", "HardSenseESP Failure");
            }
        }
        private void Trans_Receive_Password_Update_Success(bool result)
        {
            if (result)
            {
                AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_CURRENT_PASSWORD_IS_SET"]);
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
                AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SERVER_HOSTNAME"]);
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
                AddKeyToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_REQUEST_SERVER_PORT"]);
            }
            else
            {
                MessageBoxPopUp("HardSenseESP failed to update the Server Port", "HardSenseESP Failure");
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
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["PACKET_END"]);

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

            tmpSB.Insert(0, ProtocolKeys.TRANSMISSION_KEYS["STX"]);
            tmpSB.Append(ProtocolKeys.TRANSMISSION_KEYS["ETX"]);

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
            AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_SET_SSID"], newSSID);
            AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_SET_PASSWORD"], newPassword);
            textBox_NewSSID.Clear();
            textBox_NewPassword.Clear();
            textBox_NewPasswordConfirm.Clear();
        }

        private void button_UpdateServerInfoToESP_Click(object sender, EventArgs e)
        {
            AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_SET_SERVER_HOSTNAME"], textBox_ServerName.Text);
            AddStringToMessage(ProtocolKeys.TRANSMISSION_KEYS["CONFIG_CURRENT_SERVER_PORT"], textBox_ServerPort.Text);
            textBox_ServerName.Clear();
            textBox_ServerPort.Clear();
        }
    }
}

