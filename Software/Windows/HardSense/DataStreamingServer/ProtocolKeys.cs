using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HardSense.DataStreamingServer
{
    class ProtocolKeys
    {
        public static Dictionary<string, char> TRANSMISSION_KEYS = new Dictionary<string, char>();
        public static void init()
        {
            TRANSMISSION_KEYS.Add("TRANS_START", '\u0002');  // STX (start of text)
            TRANSMISSION_KEYS.Add("TRANS__ETX", '\u0003'); //ETX (end of text)
            TRANSMISSION_KEYS.Add("TRANS__DATA_SIZE_END", '\u001F'); // US (unit separator)
            TRANSMISSION_KEYS.Add("TRANS__PACKET_SIZE_END", '\u001E'); // RS (record separator)
            TRANSMISSION_KEYS.Add("TRANS__PACKET_END", '\u001D'); // GS (group separator)
            TRANSMISSION_KEYS.Add("TRANS__ACK", '\u0006'); // ACK (acknowledge)
            TRANSMISSION_KEYS.Add("TRANS__NACK", '\u0015'); // NAK (negative acknowledge)

            TRANSMISSION_KEYS.Add("TRANS__HEARTBEAT", '\u007D'); // }
            TRANSMISSION_KEYS.Add("TRANS__HEARTBEAT_ACK", '\u007E'); // ~

            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_REQUEST", '\u0005'); // ENQ (enquiry)
            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_ACK", '\u0001'); // SOH (start of heading)
            TRANSMISSION_KEYS.Add("TRANS__DISCONNECT", '\u0004'); // EOT (end of transmission)
            
            TRANSMISSION_KEYS.Add("TRANS__START_SENSOR_DATA_STREAM", '\u0011'); // DC1 (device control 1)
            TRANSMISSION_KEYS.Add("TRANS__STOP_SENSOR_DATA_STREAM", '\u0012'); // DC2 (device control 2)
            TRANSMISSION_KEYS.Add("TRANS__CLEAR_SENSOR_LIST", '\u0013'); // DC3 (device control 3)
            TRANSMISSION_KEYS.Add("TRANS__ADD_SENSORS_SENSOR_LIST", '\u0014'); // DC4 (device control 4)

            TRANSMISSION_KEYS.Add("TRANS__REQUESTING_FULL_SENSOR_LIST", '\u0021'); // !
            TRANSMISSION_KEYS.Add("TRANS__SENDING_FULL_SENSOR_LIST", '\u0022'); // "
            TRANSMISSION_KEYS.Add("TRANS__REQUEST_SENSORS_FOR_STREAMING", '\u0023'); // #
            TRANSMISSION_KEYS.Add("TRANS__UPDATE_SENSOR_VALUE", '\u0024'); // $
            TRANSMISSION_KEYS.Add("TRANS__TEST_KEY", '\u0025'); // %
            TRANSMISSION_KEYS.Add("TRANS__TEST_KEY_2", '\u0026'); // &

        }
    }

    enum TRANS__KEY : byte
    {
        TRANS__START = 0x02, // STX (start of text)
        TRANS__ETX = 0x03, //ETX (end of text)
        TRANS__DATA_SIZE_END = 0x1F, // US (unit separator)
        TRANS__PACKET_SIZE_END = 0x1E, // RS (record separator)
        TRANS__PACKET_END = 0X1D, // GS (group separator)
        TRANS__ACK = 0x06, // ACK (acknowledge)
        TRANS__NACK = 0x15, // NAK (negative acknowledge)

        TRANS__HEARTBEAT = 0x7D, // }
        TRANS__HEARTBEAT_ACK = 0x7E, // ~

        TRANS__CONNECTION_REQUEST = 0x05, // ENQ (enquiry)
        TRANS__CONNECTION_ACK = 0x01,   // SOH (start of heading)
        TRANS__DISCONNECT = 0x04,   // EOT (end of transmission)

        TRANS__START_SENSOR_DATA_STREAM = 0x11, // DC1 (device control 1)
        TRANS__STOP_SENSOR_DATA_STREAM = 0x12, // DC2 (device control 2)
        TRANS__CLEAR_SENSOR_LIST = 0x13, // DC3 (device control 3)
        TRANS__ADD_SENSORS_SENSOR_LIST = 0x14, // DC4 (device control 4)

        TRANS__REQUESTING_FULL_SENSOR_LIST = 0x21, // !
        TRANS__SENDING_FULL_SENSOR_LIST = 0x22, // "
        TRANS__REQUEST_SENSORS_FOR_STREAMING = 0x23, // #
        TRANS__UPDATE_SENSOR_VALUE = 0x24, // $
        TRANS__TEST_KEY = 0x25, // %
        TRANS__TEST_KEY_2 = 0x26, // &
    };

    
}
