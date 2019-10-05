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

        private bool initiationComplete = false;

        public List<LocalHardwareItem> motherBoardInfo { get; private set; }
        public List<LocalHardwareItem> cpuInfo { get; private set; }
        public List<LocalHardwareItem> gpuInfo { get; private set; }
        public List<LocalHardwareItem> ramInfo { get; private set; }
        public List<LocalHardwareItem> hddInfo { get; private set; }
        public List<LocalHardwareItem> nicInfo { get; private set; }
        public List<string> listOfHardwareIDsToIgnore { get; set; } = new List<string>();
        public List<string> listOfSensorIDsToIgnore { get; set; } = new List<string>();

        public LocalHardwareMonitor()
        { 
            computer.Open();
            computer.Accept(updateVisitor);
        }

        ~LocalHardwareMonitor()
        {
            computer.Close();
        }

        public void init(List<string> newListOfHardwareIDsToIgnore, List<string> newListOfSensorIDsToIgnore)
        {
            listOfHardwareIDsToIgnore = newListOfHardwareIDsToIgnore;
            listOfSensorIDsToIgnore = newListOfSensorIDsToIgnore;

            UpdateAllHardwareInfo();
            initiationComplete = true;
        }
        
        private void ThreadProc()
        {
            if (!initiationComplete)
                throw new Exception("ThreadPrc: Initiation has not been completed.  You must init().");

            EnableAllHardwareItems();
            
            while (continueMonitoring)
            {
                UpdateComputersSensorValues();
                Thread.Sleep(250);
            }
        }

        private void UpdateComputersSensorValues()
        {
            foreach (IHardware hardwareItem in computer.Hardware)
            {
                UpdateHardwareItemsSensorValues(hardwareItem);
            }
        }

        private void UpdateHardwareItemsSensorValues(IHardware hardwareItem)
        {
            if (listOfHardwareIDsToIgnore.Contains(hardwareItem.Identifier.ToString()))
                return;

            hardwareItem.Update();
            foreach (ISensor currSensor in hardwareItem.Sensors)
                UpdateSensorValue(currSensor);

            foreach (IHardware subHardwareItem in hardwareItem.SubHardware)
                UpdateHardwareItemsSensorValues(subHardwareItem);

        }

        private void UpdateSensorValue(ISensor currSensor)
        {
            if (listOfSensorIDsToIgnore.Contains(currSensor.Identifier.ToString()))
                return;

            string n = currSensor.Name;
            float v = currSensor.Value.Value;
            return;
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
                ret.Add(new LocalHardwareItem(currHardware, listOfHardwareIDsToIgnore, listOfSensorIDsToIgnore));
            }

            return ret;
        }

        private void UpdateMainboardInfo()
        {
            DisableAllHardwareItems();
            computer.MainboardEnabled = true;
            motherBoardInfo = FetchHardwareInfo();
        }
        private void UpdateCPUInfo()
        {
            DisableAllHardwareItems();
            computer.CPUEnabled = true;
            cpuInfo = FetchHardwareInfo();
        }

        private void UpdateGPUInfo()
        {
            DisableAllHardwareItems();
            computer.GPUEnabled = true;
            gpuInfo = FetchHardwareInfo();
        }

        private void UpdateRAMInfo()
        {
            DisableAllHardwareItems();
            computer.RAMEnabled = true;
            ramInfo = FetchHardwareInfo();
        }

        private void UpdateHDDInfo()
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

        private void UpdateNicInfo()
        {
            netSpeedMonitor.FindNICs(listOfSensorIDsToIgnore);
            nicInfo = new List<LocalHardwareItem>();

            if (!netSpeedMonitor.hasNics)
                return;

            nicInfo = netSpeedMonitor.localHardwareNics;
        }

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
