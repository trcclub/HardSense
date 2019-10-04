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

        public LocalSensor()
        {

        }
            
        public LocalSensor(string newId, string newName, SensorType newType)
        {
            Id = newId;
            Name = newName;
            Type = newType;
        }

        public LocalSensor(ISensor currSensor)
        {
            Id = currSensor.Identifier.ToString();
            Name = currSensor.Name;
            Type = currSensor.SensorType;
        }
        
    }

    
}
