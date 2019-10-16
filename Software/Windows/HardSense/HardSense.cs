using HardSense.HardwareMonitor;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using HardSense.MemFile;
using HardSense.GUI;
using HardSense.GUI.Bluetooth;
using System.Threading;

namespace HardSense
{
    public partial class HardSense : Form
    {
        private NotifyIcon trayIcon;
        private ContextMenu trayMenu;
        private LocalHardwareMonitor computerMonitor = new LocalHardwareMonitor();
        


        //private Thread tempThread;
        //private int counter = 0;
        public HardSense()
        {
            InitializeComponent();
            SetupTrayIcon();

            tempDisplayBox.AppendText("Starting...\n");
            

            //tempThread = new Thread(ThreadProc);
            //tempThread.Start();

            

            
            List<string> tmpHardwareExludeList = new List<string>();
            //tmpHardwareExludeList.Add("/mainboard");
            //tmpHardwareExludeList.Add("/intelcpu/0");
            //tmpHardwareExludeList.Add("/ram");
            //tmpHardwareExludeList.Add("/hdd/0");
            //tmpHardwareExludeList.Add("/hdd/1");
            //tmpHardwareExludeList.Add("/hdd/2");
            //tmpHardwareExludeList.Add("/Bluetooth/0");

            List<string> tmpSensorExcludeList = new List<string>();
            //tmpSensorExcludeList.Add("/Bluetooth/0/send");
            //tmpSensorExcludeList.Add("/Bluetooth/0/recv");

            computerMonitor.init(tmpHardwareExludeList, tmpSensorExcludeList);
            computerMonitor.StartMonitor();
            DataStreamingServer.DataStreamingServer.StartServer();

            /* Temporarily disable the monitor and socket while working on the other bits.
            //----------------------------------------//
            //----------------------------------------//
            
            
           computerMonitor.init(tmpHardwareExludeList,tmpSensorExcludeList);
           computerMonitor.StartMonitor();
           DataStreamingServer.DataStreamingServer.StartServer();
           
           
             Be sure to remove this comment stuff to re-enable all the things.
            //----------------------------------------//
            //----------------------------------------//
            */

            //DisplayHardwareDataIntheBox();
            DisplaySensorsInTheBox();

        }

        /*
        private void ThreadProc()
        {
            while (true)
            {
                tempDisplayBox.Invoke((Action)delegate
                {
                    counter++;
                    String str = "Hello World! - " + counter.ToString() + "\n";
                    tempDisplayBox.AppendText(str);
                });
                Thread.Sleep(250);
            }
        }
        */
        private void DisplaySensorsInTheBox()
        {
            string numSensors = "Total available sensors: " + LocalHardwareMonitor.allAvailableSensors.Count.ToString() + "\n";// computerMonitor.allAvailableSensors.Count.ToString() + "\n";
            tempDisplayBox.AppendText(numSensors);

            foreach (LocalSensor currSensor in LocalHardwareMonitor.allAvailableSensors)// computerMonitor.allAvailableSensors)
            {
                string currSensorString = "Name: " + currSensor.Name + "\n";
                currSensorString += "ID: " + currSensor.Id + "\n";
                currSensorString += "Type: " + currSensor.Type + "\n";
                currSensorString += "Member of: " + currSensor.MemberOf + "\n";
                currSensorString += "Parent: " + currSensor.Parent + "\n";
                currSensorString += "Value: " + HardSenseMemFile.GetValueByKey(currSensor.Id) + "\n";
                tempDisplayBox.AppendText(currSensorString);
                tempDisplayBox.AppendText("---\n");
            }

        }

        private void DisplayHardwareDataIntheBox()
        {

            tempDisplayBox.AppendText("\n---\nMotherboard Info\n");
            string s = "Found " + computerMonitor.motherBoardInfo.Count.ToString() + " Motherboards(s)\n";
            tempDisplayBox.AppendText(s);
            int count = 0;
            foreach (LocalHardwareItem currItem in computerMonitor.motherBoardInfo)
            {
                count++;
                s = "\n";
                s += "Motherboard #" + count.ToString() + "\n";
                s += "Name: " + currItem.Name + "\n";
                s += "Id: " + currItem.Id + "\n";
                s += "Ignored: " + currItem.ignored.ToString() + "\n";
                s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                tempDisplayBox.AppendText(s);
                foreach (LocalSensor currSensor in currItem.SensorList)
                {
                    DisplaySensorInfo(currSensor);
                    tempDisplayBox.AppendText("\n");
                }
            }



            tempDisplayBox.AppendText("\n---\nCPU Info\n");
            s = "Found " + computerMonitor.cpuInfo.Count.ToString() + " CPU(s)\n";
            tempDisplayBox.AppendText(s);
            count = 0;
            foreach (LocalHardwareItem currItem in computerMonitor.cpuInfo)
            {
                count++;
                s = "\n";
                s += "Cpu #" + count.ToString() + "\n";
                s += "Name: " + currItem.Name + "\n";
                s += "Id: " + currItem.Id + "\n";
                s += "Ignored: " + currItem.ignored.ToString() + "\n";
                s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                tempDisplayBox.AppendText(s);
                foreach (LocalSensor currSensor in currItem.SensorList)
                {
                    DisplaySensorInfo(currSensor);
                    tempDisplayBox.AppendText("\n");
                }
            }

            tempDisplayBox.AppendText("\n---\nGPU Info\n");
            s = "Found " + computerMonitor.gpuInfo.Count.ToString() + " GPU(s)\n";
            tempDisplayBox.AppendText(s);
            count = 0;
            foreach (LocalHardwareItem currItem in computerMonitor.gpuInfo)
            {
                count++;
                s = "\n";
                s += "GPU #" + count.ToString() + "\n";
                s += "Name: " + currItem.Name + "\n";
                s += "Id: " + currItem.Id + "\n";
                s += "Ignored: " + currItem.ignored.ToString() + "\n";
                s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                tempDisplayBox.AppendText(s);
                foreach (LocalSensor currSensor in currItem.SensorList)
                {
                    DisplaySensorInfo(currSensor);
                    tempDisplayBox.AppendText("\n");
                }
            }

            tempDisplayBox.AppendText("\n---\nRAM Info\n");
            s = "Found " + computerMonitor.ramInfo.Count.ToString() + " RAM Unit(s)\n";
            tempDisplayBox.AppendText(s);
            count = 0;
            foreach (LocalHardwareItem currItem in computerMonitor.ramInfo)
            {
                count++;
                s = "\n";
                s += "Ram #" + count.ToString() + "\n";
                s += "Name: " + currItem.Name + "\n";
                s += "Id: " + currItem.Id + "\n";
                s += "Ignored: " + currItem.ignored.ToString() + "\n";
                s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                tempDisplayBox.AppendText(s);
                foreach (LocalSensor currSensor in currItem.SensorList)
                {
                    DisplaySensorInfo(currSensor);
                    tempDisplayBox.AppendText("\n");
                }
            }

            tempDisplayBox.AppendText("\n---\nHDD Info\n");
            s = "Found " + computerMonitor.hddInfo.Count.ToString() + " HDD(s)\n";
            tempDisplayBox.AppendText(s);
            count = 0;
            foreach (LocalHardwareItem currItem in computerMonitor.hddInfo)
            {
                count++;
                s = "\n";
                s += "HDD #" + count.ToString() + "\n";
                s += "Name: " + currItem.Name + "\n";
                s += "Id: " + currItem.Id + "\n";
                s += "Ignored: " + currItem.ignored.ToString() + "\n";
                s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                tempDisplayBox.AppendText(s);
                foreach (LocalSensor currSensor in currItem.SensorList)
                {
                    DisplaySensorInfo(currSensor);
                    tempDisplayBox.AppendText("\n");
                }
            }


            if (computerMonitor.nicInfo.Count != 0)
            {
                tempDisplayBox.AppendText("\n---\nNic Info\n");
                s = "Found " + computerMonitor.nicInfo.Count.ToString() + " nic(s)\n";
                tempDisplayBox.AppendText(s);
                count = 0;
                foreach (LocalHardwareItem currItem in computerMonitor.nicInfo)
                {
                    count++;
                    s = "\n";
                    s += "nic #" + count.ToString() + "\n";
                    s += "Name: " + currItem.Name + "\n";
                    s += "Id: " + currItem.Id + "\n";
                    s += "Ignored: " + currItem.ignored.ToString() + "\n";
                    s += "Number of sensors: " + currItem.NumberOfSensors.ToString() + "\n\nSensor List:\n";
                    tempDisplayBox.AppendText(s);
                    foreach (LocalSensor currSensor in currItem.SensorList)
                    {
                        DisplaySensorInfo(currSensor);
                        tempDisplayBox.AppendText("\n");
                    }
                }
            }
            else
            {
                tempDisplayBox.AppendText("No network connections found\n");
            }
            // NETWORK INTERFACE STUFF //
            /*
            tempDisplayBox.AppendText("\n-------------\n");


            IPGlobalProperties properties = IPGlobalProperties.GetIPGlobalProperties();
            s = "Hostname: " + properties.HostName + "\n";
            s += "Domainname: " + properties.DomainName + "\n";

            tempDisplayBox.AppendText(s);
            tempDisplayBox.AppendText("\n*******\n");

            NetworkInterface[] adapters = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface adapter in adapters)
            {
                if (!adapter.Supports(NetworkInterfaceComponent.IPv4))
                    continue;

                s = "ID: " + adapter.Id + "\n";
                s += "Description: " + adapter.Description + "\n";
                s += "Name: " + adapter.Name + "\n";
                s += "Speed: " + adapter.Speed.ToString()+ "\n";

                //adapter.GetIPv4Statistics.
                tempDisplayBox.AppendText(s);
                tempDisplayBox.AppendText("\n---\n");
            }

            */
        }


        private void DisplaySensorInfo(LocalSensor currSensor)
        {
            String s = "Name: " + currSensor.Name + "\n";
            //s += "Id: " + currSensor.Id + "\n";
            s += "Type: " + currSensor.Type + "\n";
            //s += "Value: " + currSensor.value + "\n";
            //s += "Ignored: " + currSensor.ignored.ToString() + "\n";
            tempDisplayBox.AppendText(s);
        }

        private void SetupTrayIcon()
        {
            trayMenu = new ContextMenu();
            trayMenu.MenuItems.Add("Exit", OnExit);
            trayMenu.MenuItems.Add("Open", trayIcon_DoubleClick);

            trayIcon = new NotifyIcon();
            trayIcon.Text = "HardSense";
            trayIcon.Icon = Properties.Resources.AppIcon;
            trayIcon.DoubleClick += new System.EventHandler(this.trayIcon_DoubleClick);


            trayIcon.ContextMenu = trayMenu;
            trayIcon.Visible = true;
        }

        protected override void OnLoad(EventArgs e)
        {
            Visible = false; // Hide form window.
            //ShowInTaskbar = false; // Remove from taskbar.

            base.OnLoad(e);
        }

        private void trayIcon_DoubleClick(object Sender, EventArgs e)
        {
            // Show the form when the user double clicks on the notify icon.

            // Set the WindowState to normal if the form is minimized.
            if (this.WindowState == FormWindowState.Minimized)
                this.WindowState = FormWindowState.Normal;

            // Activate the form.
            this.Show();
        }

        private void OnExit(object sender, EventArgs e)
        {
            DataStreamingServer.DataStreamingServer.StopServer();
            computerMonitor.StopMonitor();
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                Hide();
                //this.WindowState = FormWindowState.Minimized;
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox ab = new AboutBox();
            ab.ShowDialog(this);
        }

        private void bluetoothConfiguratorToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            BluetoothConfigurator btConfig = new BluetoothConfigurator();
            btConfig.ShowDialog(this);
        }
    }
}
