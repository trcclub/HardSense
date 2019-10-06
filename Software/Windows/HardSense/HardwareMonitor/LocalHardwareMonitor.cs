using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using HardSense.MemFile;
using OpenHardwareMonitor.Hardware;

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
                
        private MemFile.HardSenseMemFile mmFile = new MemFile.HardSenseMemFile(true);

        private bool initiationComplete = false;

        public List<LocalHardwareItem> motherBoardInfo { get; private set; }
        public List<LocalHardwareItem> cpuInfo { get; private set; }
        public List<LocalHardwareItem> gpuInfo { get; private set; }
        public List<LocalHardwareItem> ramInfo { get; private set; }
        public List<LocalHardwareItem> hddInfo { get; private set; }
        public List<LocalHardwareItem> fanInfo { get; private set; }
        public List<LocalHardwareItem> nicInfo { get; private set; }
        
        public List<string> listOfHardwareIDsToIgnore { get; set; } = new List<string>();
        public List<string> listOfSensorIDsToIgnore { get; set; } = new List<string>();
        public List<LocalSensor> allAvailableSensors { get; private set; } = new List<LocalSensor>();

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
            EnableAllHardwareItems();
            UpdateComputersSensorValues();
            initiationComplete = true;
        }
        
        private void ThreadProc()
        {
            if (!initiationComplete)
                throw new Exception("ThreadPrc: Initiation has not been completed.  You must init().");
                        
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

            netSpeedMonitor.UpdateAllNics();
            foreach(LocalNetworkInterface currNic in netSpeedMonitor.nics)
            {
                if (listOfHardwareIDsToIgnore.Contains(currNic.id))
                {
                    continue;
                }

                string recvID = currNic.id + Properties.Settings.Default.DefaultNicReceiveID;
                string sendID = currNic.id + Properties.Settings.Default.DefaultNicSendID;

                if(!listOfSensorIDsToIgnore.Contains(recvID))
                {
                    mmFile.UpdateKeyWithValue(recvID, currNic.recvSpeed);
                }
                if (!listOfSensorIDsToIgnore.Contains(sendID))
                {
                    mmFile.UpdateKeyWithValue(sendID, currNic.sendSpeed);
                }
            }
            
        }

        private void UpdateHardwareItemsSensorValues(IHardware hardwareItem)
        {
            if (listOfHardwareIDsToIgnore.Contains(hardwareItem.Identifier.ToString()))
                return;

            hardwareItem.Update();
            foreach (ISensor currSensor in hardwareItem.Sensors)
            {
                UpdateSensorValue(currSensor);
            }

            foreach (IHardware subHardwareItem in hardwareItem.SubHardware)
            {
                UpdateHardwareItemsSensorValues(subHardwareItem);
            }

        }

        private void UpdateSensorValue(ISensor currSensor)
        {
            if (listOfSensorIDsToIgnore.Contains(currSensor.Identifier.ToString()))
            {
                return;
            }

            mmFile.UpdateKeyWithValue(currSensor.Identifier.ToString(), currSensor.Value.Value);

            return;
        }
        public void StartMonitor()
        {
            continueMonitoring = true;
            monitorThread = new Thread(ThreadProc);
            monitorThread.Start();
            running = true;
        }

        public void StopMonitor()
        {
            if (!running)
            {
                return;
            }
            continueMonitoring = false;
            monitorThread.Join();
            running = false;
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
            {
                StopMonitor();
            }

            mmFile.Clear();
            allAvailableSensors.Clear();

            UpdateMainboardInfo();
            UpdateCPUInfo();
            UpdateGPUInfo();
            UpdateRAMInfo();
            UpdateHDDInfo();
            UpdateFanInfo();
            UpdateNicInfo();

            mmFile.InitializeMemoryMappedFileWithData();

            if (wasMonitorRuning)
            {
                StartMonitor();
            }
        }

        private List<LocalHardwareItem> FetchHardwareInfo()
        {
            List<LocalHardwareItem> ret = new List<LocalHardwareItem>();

            computer.Traverse(updateVisitor);
            if (computer.Hardware.Length == 0)
            {
                return ret;
            }
            

            foreach (IHardware currHardware in computer.Hardware)
            {
                LocalHardwareItem tmpHardwareItem = new LocalHardwareItem(currHardware, listOfHardwareIDsToIgnore, listOfSensorIDsToIgnore);
                AddSensorsToAllAvailableList(tmpHardwareItem);
                ret.Add(tmpHardwareItem);
            }

            return ret;
        }

        private void UpdateMainboardInfo()
        {
            DisableAllHardwareItems();
            computer.MainboardEnabled = true;
            motherBoardInfo = FetchHardwareInfo();
            AddHardwareListToMMFileMap(motherBoardInfo);
        }
        private void UpdateCPUInfo()
        {
            DisableAllHardwareItems();
            computer.CPUEnabled = true;
            cpuInfo = FetchHardwareInfo();
            AddHardwareListToMMFileMap(cpuInfo);
        }

        private void UpdateGPUInfo()
        {
            DisableAllHardwareItems();
            computer.GPUEnabled = true;
            gpuInfo = FetchHardwareInfo();
            AddHardwareListToMMFileMap(gpuInfo);
        }

        private void UpdateRAMInfo()
        {
            DisableAllHardwareItems();
            computer.RAMEnabled = true;
            ramInfo = FetchHardwareInfo();
            AddHardwareListToMMFileMap(ramInfo);
        }

        private void UpdateFanInfo()
        {
            DisableAllHardwareItems();
            computer.FanControllerEnabled = true;
            fanInfo = FetchHardwareInfo();
            AddHardwareListToMMFileMap(fanInfo);
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
            AddHardwareListToMMFileMap(hddInfo);
        }

        private void UpdateNicInfo()
        {
            netSpeedMonitor.FindNICs(listOfHardwareIDsToIgnore, listOfSensorIDsToIgnore);
            nicInfo = new List<LocalHardwareItem>();

            if (!netSpeedMonitor.hasNics)
                return;

            nicInfo = netSpeedMonitor.localHardwareNics;
            foreach(LocalHardwareItem currHardwareItem in nicInfo)
            {
                AddSensorsToAllAvailableList(currHardwareItem);
            }
            AddHardwareListToMMFileMap(nicInfo);
        }

        private void AddSensorsToAllAvailableList(LocalHardwareItem currHardwareItem)
        {
            if (!currHardwareItem.ignored)
            {
                foreach (LocalSensor currSensor in currHardwareItem.SensorList)
                {
                    if (currSensor.ignored)
                        continue;
                    allAvailableSensors.Add(currSensor);
                }
            }
        }

        private void AddHardwareListToMMFileMap(List<LocalHardwareItem> currHardwareItemList)
        {
            foreach(LocalHardwareItem currHardwareItem in currHardwareItemList)
            {
                if (currHardwareItem.ignored)
                    continue;

                foreach (LocalSensor currSensor in currHardwareItem.SensorList)
                {
                    if (currSensor.ignored)
                        continue;
                    mmFile.AddNewDataItem(currSensor.Id, Properties.Settings.Default.DefaultMemoryMappedFieldSize);
                }
            }
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
