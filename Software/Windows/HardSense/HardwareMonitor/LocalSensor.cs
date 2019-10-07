using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenHardwareMonitor.Hardware;

namespace HardSense.HardwareMonitor
{
    public class LocalSensor
    {
        public string Id { get; private set; } = "";
        public string Name { get; private set; } = "";
        public SensorType Type { get; private set; }
        public bool ignored { get; private set; } = false;

        public LocalSensor(LocalSensor origSensor)
        {
            Id = origSensor.Id;
            Name = origSensor.Name;
            Type = origSensor.Type;
            ignored = origSensor.ignored;
        }
            
        public LocalSensor(string newId, string newName, SensorType newType, bool isIgnored = false)
        {
            Id = newId;
            Name = newName;
            Type = newType;
            ignored = isIgnored;
        }

        public LocalSensor(ISensor currSensor, List<string> sensorListToIgnore)
        {
            Id = currSensor.Identifier.ToString();
            Name = currSensor.Name;
            Type = currSensor.SensorType;
            ignored = sensorListToIgnore.Contains(Id);
        }
        
    }

    
}
