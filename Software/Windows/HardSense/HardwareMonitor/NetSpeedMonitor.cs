using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.NetworkInformation;
using OpenHardwareMonitor.Hardware;
using System.Diagnostics;

namespace HardSense.HardwareMonitor
{
    class NetSpeedMonitor
    {
        public List<LocalNetworkInterface> nics { get; private set; }
        public List<LocalHardwareItem> localHardwareNics { get; private set; }
        public bool hasNics { get; private set; } = false;


        public void UpdateAllNics()
        {
            foreach(LocalNetworkInterface currNic in nics)
            {
                currNic.UpdateMonitor();
            }
        }
        public bool FindNICs(List<string> hardwareListToIgnore, List<string> sensorListToIgnore)
        {
            nics = new List<LocalNetworkInterface>();

            NetworkInterface[] localNics = NetworkInterface.GetAllNetworkInterfaces();
            if (localNics == null || localNics.Length < 1)
            {
                hasNics = false;
                return false;
            }
            foreach (NetworkInterface adapter in localNics)
            {
                if (!adapter.Supports(NetworkInterfaceComponent.IPv4) || adapter.NetworkInterfaceType == NetworkInterfaceType.Loopback)
                    continue;

                nics.Add(new LocalNetworkInterface(adapter));
            }

            if (nics.Count == 0)
            {
                hasNics = false;
                return false;
            }

            BuildListOfHardwareItems(hardwareListToIgnore, sensorListToIgnore);
            hasNics = true;
            return true;
        }

        private void BuildListOfHardwareItems(List<string> hardwareListToIgnore, List<string> sensorListToIgnore)
        {
            localHardwareNics = new List<LocalHardwareItem>();


            int ethernetCounter = 0;
            int bluetoothCounter = 0;
            foreach (LocalNetworkInterface currNic in nics)
            {
                LocalHardwareItem tmpHardwareItem = new LocalHardwareItem();
                tmpHardwareItem.Name = currNic.nic.Name;

                if (currNic.nic.Name.Contains("Bluetooth"))
                {
                    tmpHardwareItem.Id = "/Bluetooth/" + bluetoothCounter.ToString();
                    bluetoothCounter++;
                }
                else
                {
                    tmpHardwareItem.Id = "/Ethernet/" + ethernetCounter.ToString();
                    ethernetCounter++;
                }

                currNic.id = tmpHardwareItem.Id;
                if (hardwareListToIgnore.Contains(currNic.id))
                {
                    tmpHardwareItem.ignored = true;
                }
                
                tmpHardwareItem.NumberOfSensors = 2;

                string tmpRecvID = tmpHardwareItem.Id + Properties.Settings.Default.DefaultNicReceiveID;
                string tmpSendID = tmpHardwareItem.Id + Properties.Settings.Default.DefaultNicSendID;
                tmpHardwareItem.SensorList.Add(new LocalSensor(tmpRecvID, Properties.Settings.Default.DefaultNicReceiveSensorName, SensorType.KBPS, sensorListToIgnore.Contains(tmpRecvID)));
                tmpHardwareItem.SensorList.Add(new LocalSensor(tmpSendID, Properties.Settings.Default.DefaultNicSendSensorName, SensorType.KBPS, sensorListToIgnore.Contains(tmpSendID)));


                localHardwareNics.Add(tmpHardwareItem);
                currNic.StartMonitor();
            }
        }
    }

    public class LocalNetworkInterface
    {
        public NetworkInterface nic { get; set; }
        public string id { get; set; } = "";
        private long bytesReceived { get; set; } = 0;
        private long bytesSent { get; set; } = 0;
        public double sendSpeed { get; private set; } = 0;
        public double recvSpeed { get; private set; } = 0;

        private Stopwatch stopWatch = new Stopwatch();

        public LocalNetworkInterface(NetworkInterface inNic)
        {
            nic = inNic;
        }

        public void StartMonitor()
        {
            bytesReceived = nic.GetIPv4Statistics().BytesReceived;
            bytesSent = nic.GetIPv4Statistics().BytesSent;
            stopWatch.Start();
        }

        public void UpdateMonitor()
        {
            long origBR = bytesReceived;
            long origBS = bytesSent;

            bytesReceived = nic.GetIPv4Statistics().BytesReceived;
            bytesSent = nic.GetIPv4Statistics().BytesSent;

            stopWatch.Stop();
            double timeDelta = stopWatch.Elapsed.TotalMilliseconds;
            stopWatch.Restart();

            long sentDelta = bytesSent - origBS;
            long recvDelta = bytesReceived - origBR;

            sendSpeed = Convert.ToDouble((sentDelta / timeDelta).ToString("F3"));
            recvSpeed = Convert.ToDouble((recvDelta / timeDelta).ToString("F3"));
        }
    }
}
