using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace HardSense.GUI.Bluetooth
{
    public class BluetoothSerial
    {

        public BluetoothSerial()
        {
            
        }

        public string[] GetAvailablePorts()
        {
            return SerialPort.GetPortNames();
        }
    }
}
