using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.NetworkInformation;
using OpenHardwareMonitor.Hardware;

namespace HardSense.HardwareMonitor
{
    class NetSpeedMonitor
    {
        private List<NetworkInterface> nics;
        public List<LocalHardwareItem> localHardwareNics { get; private set; }
        public bool hasNics { get; private set; } = false;


        public NetSpeedMonitor()
        {
            hasNics = FindNICs();
                
        }

        private bool FindNICs()
        {
            nics = new List<NetworkInterface>();

            NetworkInterface[] localNics = NetworkInterface.GetAllNetworkInterfaces();
            if (localNics == null || localNics.Length < 1)
            {
                return false;
            }
            foreach (NetworkInterface adapter in localNics)
            {
                if (!adapter.Supports(NetworkInterfaceComponent.IPv4) || adapter.NetworkInterfaceType == NetworkInterfaceType.Loopback)
                    continue;

                nics.Add(adapter);

            }

            if (nics.Count == 0)
                return false;

            BuildListOfHardwareItems();
            return true;
        }

        private void BuildListOfHardwareItems()
        {
            localHardwareNics = new List<LocalHardwareItem>();


            int ethernetCounter = 0;
            int bluetoothCounter = 0;
            foreach (NetworkInterface currNic in nics)
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

                tmpHardwareItem.SensorList.Add(new LocalSensor((tmpHardwareItem.Id + "/recv"), "Receive Speed", SensorType.BPS));
                tmpHardwareItem.SensorList.Add(new LocalSensor((tmpHardwareItem.Id + "/send"), "Send Speed", SensorType.BPS));


                localHardwareNics.Add(tmpHardwareItem);
            }
        }
    }
}
