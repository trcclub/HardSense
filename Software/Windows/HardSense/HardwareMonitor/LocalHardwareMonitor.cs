using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using OpenHardwareMonitor.Hardware;
using HardSense.MemFile;

namespace HardSense.HardwareMonitor
{    
    public class LocalHardwareMonitor
    {
        public bool running = false;

        private bool continueMonitoring = false;
        private Thread monitorThread;

        private UpdateVisitor updateVisitor = new UpdateVisitor();
        private Computer computer = new Computer();
        private NetSpeedMonitor netSpeedMonitor = new NetSpeedMonitor();
        
        
        private MMFile mmFile;

        public List<LocalHardwareItem> motherBoardInfo;
        public List<LocalHardwareItem> cpuInfo;
        public List<LocalHardwareItem> gpuInfo;
        public List<LocalHardwareItem> ramInfo;
        public List<LocalHardwareItem> hddInfo;
        public List<LocalHardwareItem> nicInfo;
        //public List<LocalHardwareItem> fanControllerInfo;

        public LocalHardwareMonitor()
        {
            computer.Open();
            computer.Accept(updateVisitor);

            UpdateAllHardwareInfo();          
        }

        ~LocalHardwareMonitor()
        {
            computer.Close();
        }

        private void ThreadProc()
        {
            EnableAllHardwareItems();
            while (continueMonitoring)
            {

                Thread.Sleep(250);
            }
        }

        public bool StartMonitor()
        {
            continueMonitoring = true;
            monitorThread = new Thread(ThreadProc);
            monitorThread.Start();
            running = true;
            return true;
        }

        public bool StopMonitor()
        {
            if (!running) return true;
            continueMonitoring = false;
            monitorThread.Join();
            running = false;
            return true;
        }

        private void EnableAllHardwareItems()
        {
            computer.CPUEnabled = true;
            computer.GPUEnabled = true;
            computer.RAMEnabled = true;
            computer.MainboardEnabled = true;
            computer.HDDEnabled = true;
            computer.FanControllerEnabled = false;
        }

        private void DisableAllHardwareItems()
        {
            computer.CPUEnabled = false;
            computer.GPUEnabled = false;
            computer.RAMEnabled = false;
            computer.MainboardEnabled = false;
            computer.HDDEnabled = false;
            computer.FanControllerEnabled = false;

        }

        public void UpdateAllHardwareInfo()
        {
            bool wasMonitorRuning = running;
            if (running)
                StopMonitor();

            UpdateMainboardInfo();
            UpdateCPUInfo();
            UpdateGPUInfo();
            UpdateRAMInfo();
            UpdateHDDInfo();
            //UpdateFanControllerInfo();
            UpdateNicInfo();

            if (wasMonitorRuning)
                StartMonitor();
        }

        private List<LocalHardwareItem> FetchHardwareInfo()
        {
            List<LocalHardwareItem> ret = new List<LocalHardwareItem>();

            computer.Traverse(updateVisitor);
            if (computer.Hardware.Length == 0)
            {
                throw new Exception("Failed to find any hardware.  Which is really weird to say the least.");
            }

            foreach (IHardware currHardware in computer.Hardware)
            {
                ret.Add(new LocalHardwareItem(currHardware));
            }

            return ret;
        }

        public void UpdateHDDInfo()
        {
            DisableAllHardwareItems();
            computer.HDDEnabled = true;

            hddInfo = FetchHardwareInfo();
            foreach(LocalHardwareItem currHDD in hddInfo)
            {
                foreach(IHardware hdd in computer.Hardware)
                {
                    if(currHDD.Name.Equals(hdd.Name))
                    {
                        string report = hdd.GetReport();
                        int x = report.IndexOf("Logical drive name: ");
                        string tmp = report.Substring(x + 20, 2);
                        currHDD.Name = currHDD.Name + " (" + tmp + ")";
                    }
                }
            }
        }

        public void UpdateRAMInfo()
        {
            DisableAllHardwareItems();
            computer.RAMEnabled = true;
            ramInfo = FetchHardwareInfo();
        }

        public void UpdateGPUInfo()
        {
            DisableAllHardwareItems();
            computer.GPUEnabled = true;
            gpuInfo = FetchHardwareInfo();
        }
        public void UpdateCPUInfo()
        {
            DisableAllHardwareItems();
            computer.CPUEnabled = true;
            cpuInfo = FetchHardwareInfo();
        }

        public void UpdateMainboardInfo()
        {
            DisableAllHardwareItems();
            computer.MainboardEnabled = true;
            motherBoardInfo = FetchHardwareInfo();
        }

        public void UpdateNicInfo()
        {
            nicInfo = new List<LocalHardwareItem>();

            if (!netSpeedMonitor.hasNics)
                return;

            nicInfo = netSpeedMonitor.localHardwareNics ;

            /*
            int ethernetCounter = 0;
            int bluetoothCounter = 0;
            foreach(NetworkInterface currNic in nics)
            {
                LocalHardwareItem tmpHardwareItem = new LocalHardwareItem();
                tmpHardwareItem.Name = currNic.Name;

                if (currNic.Name.Contains("Bluetooth"))
                {
                    tmpHardwareItem.Id = "/Bluetooth/" + bluetoothCounter.ToString();
                    bluetoothCounter++;
                }
                else
                {
                    tmpHardwareItem.Id = "/Ethernet/" + ethernetCounter.ToString();
                    ethernetCounter++;
                }
                tmpHardwareItem.NumberOfSensors = 2;

                tmpHardwareItem.SensorList.Add(new LocalSensor((tmpHardwareItem.Id + "/recv"),"Receive Speed", SensorType.SmallData));
                tmpHardwareItem.SensorList.Add(new LocalSensor((tmpHardwareItem.Id + "/send"), "Send Speed", SensorType.SmallData));


                nicInfo.Add(tmpHardwareItem);
            }
            */
        }

        /*
        public void UpdateFanControllerInfo()
        {
            computer.MainboardEnabled = false;
            computer.CPUEnabled = false;
            computer.GPUEnabled = false;
            computer.RAMEnabled = false;
            computer.FanControllerEnabled = true;
            computer.HDDEnabled = false;

            fanControllerInfo = FetchHardwareInfo();
        }
        */


    }



    public class UpdateVisitor : IVisitor
    {
        public void VisitComputer(IComputer computer)
        {
            computer.Traverse(this);
        }
        public void VisitHardware(IHardware hardware)
        {
            hardware.Update();
            foreach (IHardware subHardware in hardware.SubHardware) subHardware.Accept(this);
        }
        public void VisitSensor(ISensor sensor) { }
        public void VisitParameter(IParameter parameter) { }
    }
}
