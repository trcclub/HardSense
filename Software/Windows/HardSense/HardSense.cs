using HardSense.HardwareMonitor;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
//using System.Net.NetworkInformation;

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



            //tempThread = new Thread(ThreadProc);
            //tempThread.Start();

            //Motherboard motherBoard = computerMonitor.motherBoard;


            /*
            ListViewItem mainBoardRoot = new ListViewItem("Motherboard");
            mainBoardRoot.SubItems.Add(computerMonitor.motherBoardInfo.Name);
            mainBoardRoot.SubItems.Add(computerMonitor.motherBoardInfo.Id);
            listView1.Items.Add(mainBoardRoot);
            */

            tempDisplayBox.AppendText("Starting...\n");

            List<string> tmpHardwareList = new List<string>();
            tmpHardwareList.Add("/mainboard");

            List<string> tmpSensorList = new List<string>();
            tmpSensorList.Add("/Bluetooth/0/send");
            tmpSensorList.Add("/Bluetooth/0/recv");
            computerMonitor.init(tmpHardwareList,tmpSensorList);


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
            foreach(LocalHardwareItem currItem in computerMonitor.cpuInfo)
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
            } else
            {
                tempDisplayBox.AppendText("No network connections found\n");
            }



            computerMonitor.StartMonitor();

            //ListViewItem cpuRoot = new ListViewItem("CPU's");



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
            //TreeNode rootNode = new TreeNode();
            //rootNode.Text = "MotherBoard";




            //sesnsorSelector_TreeView.Nodes.Add(rootNode);

            //computerMonitor.StartMonitor();
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


        private void DisplaySensorInfo(LocalSensor currSensor)
        {
            String s = "Name: " + currSensor.Name + "\n";
            s += "Id: " + currSensor.Id + "\n";
            s += "Type: " + currSensor.Type + "\n";
            s += "Ignored: " + currSensor.ignored.ToString() + "\n";
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
            computerMonitor.StopMonitor();
            Application.Exit();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                this.WindowState = FormWindowState.Minimized;
            }
        }
    }
}
