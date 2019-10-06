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
        public string Name { get; set; } = "";
        public string Id { get; set; } = "";
        public bool ignored { get; set; } = false;

        public List<LocalSensor> SensorList = new List<LocalSensor>();
        public int NumberOfSensors { get; set; } = 0;
        
        public LocalHardwareItem()
        {

        }

        public LocalHardwareItem(IHardware currHardware, List<string> hardwareListToIgnore, List<string> sensorListToIgnore)
        {
            Name = currHardware.Name;
            Id = currHardware.Identifier.ToString();
            ignored = hardwareListToIgnore.Contains(Id);

            SensorList.AddRange(CollectSensorInfoFromOpenHardware(currHardware, sensorListToIgnore));
            NumberOfSensors = SensorList.Count;
        }



        private List<LocalSensor> CollectSensorInfoFromOpenHardware(IHardware currHardware, List<string> sensorListToIgnore)
        {
            List<LocalSensor> tempSensorList = new List<LocalSensor>();
            foreach (var sensorItem in currHardware.Sensors)
            {
                tempSensorList.Add(new LocalSensor(sensorItem, sensorListToIgnore));
            }

            foreach (var subHardwareItem in currHardware.SubHardware)
                tempSensorList.AddRange(CollectSensorInfoFromOpenHardware(subHardwareItem, sensorListToIgnore));

            return tempSensorList;
        }
    }
}
