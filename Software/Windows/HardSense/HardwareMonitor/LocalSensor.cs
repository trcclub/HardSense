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
        public string MemberOf { get; private set; } = "";
        public string Parent { get; set; } = "";
        public SensorType Type { get; private set; }
        public bool ignored { get; private set; } = false;

        public LocalSensor(LocalSensor origSensor)
        {
            Id = origSensor.Id;
            Name = origSensor.Name;
            MemberOf = origSensor.MemberOf;
            Parent = origSensor.Parent;
            Type = origSensor.Type;
            ignored = origSensor.ignored;
        }
            
        public LocalSensor(string newId, string newName, string parentType, string parentName, SensorType newType, bool isIgnored = false)
        {
            Id = newId;
            Name = newName;
            MemberOf = NormalizeMemberOf(parentType);
            Parent = parentName;
            Type = newType;
            ignored = isIgnored;

        }

        public LocalSensor(ISensor currSensor, string parentType, string parentName, List<string> sensorListToIgnore)
        {
            Id = currSensor.Identifier.ToString();
            Name = currSensor.Name;
            MemberOf = NormalizeMemberOf(parentType); ;
            Parent = parentName;
            Type = currSensor.SensorType;
            ignored = sensorListToIgnore.Contains(Id);
        }

        private string NormalizeMemberOf(string parentType)
        {
            if (parentType.Equals("SuperIO"))
            {
                return "Mainboard";
            }
            return parentType;
        }

        
    }

    
}
