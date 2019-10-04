using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenHardwareMonitor.Hardware;

namespace HardSense.HardwareMonitor
{
    public class LocalHardwareItem
    {
        public string Name { get; set; }
        public string Id { get; set; }
        
        public List<LocalSensor> SensorList = new List<LocalSensor>();
        public int NumberOfSensors { get; set; }
        
        public LocalHardwareItem()
        {

        }

        public LocalHardwareItem(IHardware currHardware)
        {
            Name = currHardware.Name;
            Id = currHardware.Identifier.ToString();

            SensorList.AddRange(CollectSensorInfoFromOpenHardware(currHardware));
            NumberOfSensors = SensorList.Count;
        }



        private List<LocalSensor> CollectSensorInfoFromOpenHardware(IHardware currHardware)
        {
            List<LocalSensor> tempSensorList = new List<LocalSensor>();
            foreach (var sensorItem in currHardware.Sensors)
            {
                tempSensorList.Add(new LocalSensor(sensorItem));
            }

            foreach (var subHardwareItem in currHardware.SubHardware)
                tempSensorList.AddRange(CollectSensorInfoFromOpenHardware(subHardwareItem));

            return tempSensorList;
        }
    }
}
