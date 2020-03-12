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
    public static class LocalHardwareMonitor
    {
        public static bool running = false;
        private static bool continueMonitoring = false;
        private static Thread monitorThread;

        private static UpdateVisitor updateVisitor = new UpdateVisitor();
        private static Computer computer = new Computer();
        private static NetSpeedMonitor netSpeedMonitor = new NetSpeedMonitor();
        private static FPSMonitor fpsMonitor = new FPSMonitor();
        

        private static MemFile.HardSenseMemFile mmFile = new MemFile.HardSenseMemFile(true);

        private static bool initiationComplete = false;

        public static List<LocalHardwareItem> motherBoardInfo { get; private set; }
        public static List<LocalHardwareItem> cpuInfo { get; private set; }
        public static List<LocalHardwareItem> gpuInfo { get; private set; }
        public static List<LocalHardwareItem> ramInfo { get; private set; }
        public static List<LocalHardwareItem> hddInfo { get; private set; }
        public static List<LocalHardwareItem> fanInfo { get; private set; }
        public static List<LocalHardwareItem> nicInfo { get; private set; }
        public static List<LocalHardwareItem> fpsInfo { get; private set; }

        public static List<string> listOfHardwareIDsToIgnore { get; set; } = new List<string>();
        public static List<string> listOfSensorIDsToIgnore { get; set; } = new List<string>();
        public static List<LocalSensor> allAvailableSensors { get; private set; } = new List<LocalSensor>();

        /*
        public LocalHardwareMonitor()
        { 
            computer.Open();
            computer.Accept(updateVisitor);
        }

        ~LocalHardwareMonitor()
        {
            computer.Close();
        }
        */

        public static void init(List<string> newListOfHardwareIDsToIgnore, List<string> newListOfSensorIDsToIgnore)
        {
            computer.Open();
            computer.Accept(updateVisitor);

            listOfHardwareIDsToIgnore = newListOfHardwareIDsToIgnore;
            listOfSensorIDsToIgnore = newListOfSensorIDsToIgnore;

            FindAllHardwareItemsAndSensors();
            EnableAllHardwareItems();
            UpdateComputersSensorValues();
            initiationComplete = true;
        }
        
        public static void Close()
        {
            computer.Close();
        }

        private static void MonitorThreadFunction()
        {
            if (!initiationComplete)
                throw new Exception("ThreadPrc: Initiation has not been completed.  You must init().");
                        
            while (continueMonitoring)
            {
                UpdateComputersSensorValues();
                Thread.Sleep(Properties.Settings.Default.DefaultHardwareMonitorPollTime);
            }
        }

        private static void UpdateComputersSensorValues()
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
            mmFile.UpdateKeyWithValue(Properties.Settings.Default.FPSSensorName, fpsMonitor.averageFPS);
        }

        private static void UpdateHardwareItemsSensorValues(IHardware hardwareItem)
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

        private static void UpdateSensorValue(ISensor currSensor)
        {
            if (listOfSensorIDsToIgnore.Contains(currSensor.Identifier.ToString()))
            {
                return;
            }

            mmFile.UpdateKeyWithValue(currSensor.Identifier.ToString(), currSensor.Value.Value);

            return;
        }
        public static void StartMonitor()
        {
            fpsMonitor.StartMonitor();
            continueMonitoring = true;
            monitorThread = new Thread(MonitorThreadFunction);
            monitorThread.Start();
            running = true;
        }

        public static void StopMonitor()
        {
            if (!running)
            {
                return;
            }
            fpsMonitor.StopMonitor();
            continueMonitoring = false;
            monitorThread.Join();
            running = false;
        }

        private static void EnableAllHardwareItems()
        {
            computer.CPUEnabled = true;
            computer.GPUEnabled = true;
            computer.RAMEnabled = true;
            computer.MainboardEnabled = true;
            computer.HDDEnabled = true;
            computer.FanControllerEnabled = false;
        }

        private static void DisableAllHardwareItems()
        {
            computer.CPUEnabled = false;
            computer.GPUEnabled = false;
            computer.RAMEnabled = false;
            computer.MainboardEnabled = false;
            computer.HDDEnabled = false;
            computer.FanControllerEnabled = false;

        }

        public static void FindAllHardwareItemsAndSensors()
        {
            bool wasMonitorRuning = running;
            if (running)
            {
                StopMonitor();
            }

            mmFile.Clear();
            allAvailableSensors.Clear();

            BuildMainboardInfo();
            BuildCPUInfo();
            BuildGPUInfo();
            BuildRAMInfo();
            BuildHDDInfo();
            BuildFanInfo();
            BuildNicInfo();
            BuildFPSInfo();

            mmFile.InitializeMemoryMappedFileWithData();

            if (wasMonitorRuning)
            {
                StartMonitor();
            }
        }

        private static List<LocalHardwareItem> FetchHardwareInfo()
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
                ret.Add(tmpHardwareItem);
            }

            return ret;
        }

        private static void BuildMainboardInfo()
        {
            DisableAllHardwareItems();
            computer.MainboardEnabled = true;
            motherBoardInfo = FetchHardwareInfo();
            AddSensorsToAllAvailableList(motherBoardInfo);
            AddHardwareListToMMFileMap(motherBoardInfo);
        }
        private static void BuildCPUInfo()
        {
            DisableAllHardwareItems();
            computer.CPUEnabled = true;
            cpuInfo = FetchHardwareInfo();
            AddSensorsToAllAvailableList(cpuInfo);
            AddHardwareListToMMFileMap(cpuInfo);
        }

        private static void BuildGPUInfo()
        {
            DisableAllHardwareItems();
            computer.GPUEnabled = true;
            gpuInfo = FetchHardwareInfo();
            AddSensorsToAllAvailableList(gpuInfo);
            AddHardwareListToMMFileMap(gpuInfo);
        }

        private static void BuildRAMInfo()
        {
            DisableAllHardwareItems();
            computer.RAMEnabled = true;
            ramInfo = FetchHardwareInfo();
            AddSensorsToAllAvailableList(ramInfo);
            AddHardwareListToMMFileMap(ramInfo);
        }

        private static void BuildFanInfo()
        {
            DisableAllHardwareItems();
            computer.FanControllerEnabled = true;
            fanInfo = FetchHardwareInfo();
            AddSensorsToAllAvailableList(fanInfo);
            AddHardwareListToMMFileMap(fanInfo);
        }

        private static void BuildHDDInfo()
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
                        string newName = currHDD.Name + " (" + tmp + ")";
                        currHDD.SetNewName(newName);
                    }
                }
            }
            AddSensorsToAllAvailableList(hddInfo);
            AddHardwareListToMMFileMap(hddInfo);
        }

        private static void BuildNicInfo()
        {
            netSpeedMonitor.FindNICs(listOfHardwareIDsToIgnore, listOfSensorIDsToIgnore);
            nicInfo = new List<LocalHardwareItem>();

            if (!netSpeedMonitor.hasNics)
                return;

            nicInfo = netSpeedMonitor.localHardwareNics;
            AddHardwareListToMMFileMap(nicInfo);
            AddSensorsToAllAvailableList(nicInfo);
        }

        private static void BuildFPSInfo()
        {
            fpsInfo = new List<LocalHardwareItem>();
            LocalHardwareItem fpsItem = new LocalHardwareItem();
            fpsItem.Name = "FPS Counter";
            fpsItem.Id = "/fps/0";
            LocalSensor fpsSensor = new LocalSensor(Properties.Settings.Default.FPSSensorName, "FPS Counter", "FPS", "FPS Counter", SensorType.FPS);
            fpsItem.SensorList.Add(fpsSensor);
            fpsInfo.Add(fpsItem);
            AddHardwareListToMMFileMap(fpsInfo);
            AddSensorsToAllAvailableList(fpsInfo);

        }
        
        private static void AddSensorsToAllAvailableList(List<LocalHardwareItem> hardwareItemList)
        {
            foreach (LocalHardwareItem currHardwareItem in hardwareItemList)
            {
                if (!currHardwareItem.ignored)
                {
                    foreach (LocalSensor currSensor in currHardwareItem.SensorList)
                    {
                        if (!currSensor.ignored)
                        {
                            allAvailableSensors.Add(new LocalSensor(currSensor));
                        }
                    }
                }
            }
        }

        private static void AddHardwareListToMMFileMap(List<LocalHardwareItem> currHardwareItemList)
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
