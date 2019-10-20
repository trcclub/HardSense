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
        public static bool initiated = false;
        public static void init()
        {
            if (initiated)
            {
                return;
            }
            initiated = true;

            TRANSMISSION_KEYS.Add("STX", '\u0002');  // STX (start of text)
            TRANSMISSION_KEYS.Add("ETX", '\u0003'); //ETX (end of text)
            TRANSMISSION_KEYS.Add("REQUEST_NEW_CONNECTION", '\u001F'); // US (unit separator)
            TRANSMISSION_KEYS.Add("NEW_CONNECTION_APPROVED", '\u001E'); // RS (record separator)
            TRANSMISSION_KEYS.Add("PACKET_END", '\u001D'); // GS (group separator)
            TRANSMISSION_KEYS.Add("ACK", '\u0006'); // ACK (acknowledge)
            TRANSMISSION_KEYS.Add("NACK", '\u0015'); // NAK (negative acknowledge)

            TRANSMISSION_KEYS.Add("HEARTBEAT", '\u007D'); // }
            TRANSMISSION_KEYS.Add("HEARTBEAT_ACK", '\u007E'); // ~

            TRANSMISSION_KEYS.Add("CONNECTION_REQUEST", '\u0005'); // ENQ (enquiry)
            TRANSMISSION_KEYS.Add("CONNECTION_ACK", '\u0001'); // SOH (start of heading)
            TRANSMISSION_KEYS.Add("DISCONNECT", '\u0004'); // EOT (end of transmission)

            TRANSMISSION_KEYS.Add("START_SENSOR_DATA_STREAM", '\u0011'); // DC1 (device control 1)
            TRANSMISSION_KEYS.Add("STOP_SENSOR_DATA_STREAM", '\u0012'); // DC2 (device control 2)
            TRANSMISSION_KEYS.Add("CLEAR_SENSOR_LIST", '\u0013'); // DC3 (device control 3)
            TRANSMISSION_KEYS.Add("ADD_SENSORS_TO_SENSOR_LIST", '\u0014'); // DC4 (device control 4)

            TRANSMISSION_KEYS.Add("REQUESTING_FULL_SENSOR_LIST", '\u0021'); // !
            TRANSMISSION_KEYS.Add("SENDING_FULL_SENSOR_LIST", '\u0022'); // "
            TRANSMISSION_KEYS.Add("UPDATE_SENSOR_VALUE", '\u0023'); // #

            TRANSMISSION_KEYS.Add("CONFIG_REQUEST_SSID", '\u0024'); // $
            TRANSMISSION_KEYS.Add("CONFIG_SET_SSID", '\u0025'); // %
            TRANSMISSION_KEYS.Add("CONFIG_SSID_UPDATE_SUCCESS", '\u0032'); // 2
            TRANSMISSION_KEYS.Add("CURRENT_SSID", '\u0026'); // &
            
            TRANSMISSION_KEYS.Add("CONFIG_REQUEST_IS_PASSWORD_SET", '\u0028'); // (
            TRANSMISSION_KEYS.Add("CONFIG_SET_PASSWORD", '\u0029'); // )
            TRANSMISSION_KEYS.Add("CONFIG_PASSWORD_UPDATE_SUCCESS", '\u0033'); // 3
            TRANSMISSION_KEYS.Add("CONFIG_CURRENT_PASSWORD_IS_SET", '\u002A'); // )
            
            TRANSMISSION_KEYS.Add("CONFIG_REQUEST_SERVER_HOSTNAME", '\u002B'); // +
            TRANSMISSION_KEYS.Add("CONFIG_CURRENT_SERVER_HOSTNAME", '\u002C'); // ,
            TRANSMISSION_KEYS.Add("CONFIG_SET_SERVER_HOSTNAME", '\u002D'); // -
            TRANSMISSION_KEYS.Add("CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS", '\u0034'); // 4

            TRANSMISSION_KEYS.Add("CONFIG_REQUEST_SERVER_PORT", '\u002E'); // 
            TRANSMISSION_KEYS.Add("CONFIG_CURRENT_SERVER_PORT", '\u0030'); // 
            TRANSMISSION_KEYS.Add("CONFIG_SET_SERVER_PORT", '\u0031'); // 
            TRANSMISSION_KEYS.Add("CONFIG_SERVER_PORT_UPDATE_SUCCESS", '\u0035'); // 5
            TRANSMISSION_KEYS.Add("CONFIG_DELETE_TOUCH_CALIBRATION_FILE", '\u0036'); // 6

        }
    }

    enum TRANS__KEY : byte
    {
        STX = 0x02, // STX (start of text)
        ETX = 0x03, //ETX (end of text)
        REQUEST_NEW_CONNECTION = 0x1F, // US (unit separator)
        NEW_CONNECTION_APPROVED = 0x1E, // RS (record separator)
        PACKET_END = 0X1D, // GS (group separator)
        ACK = 0x06, // ACK (acknowledge)
        NACK = 0x15, // NAK (negative acknowledge)

        HEARTBEAT = 0x7D, // }
        HEARTBEAT_ACK = 0x7E, // ~

        CONNECTION_REQUEST = 0x05, // ENQ (enquiry)
        CONNECTION_ACK = 0x01,   // SOH (start of heading)
        DISCONNECT = 0x04,   // EOT (end of transmission)

        START_SENSOR_DATA_STREAM = 0x11, // DC1 (device control 1)
        STOP_SENSOR_DATA_STREAM = 0x12, // DC2 (device control 2)
        CLEAR_SENSOR_LIST = 0x13, // DC3 (device control 3)
        ADD_SENSORS_TO_SENSOR_LIST = 0x14, // DC4 (device control 4)

        REQUESTING_FULL_SENSOR_LIST = 0x21, // !
        SENDING_FULL_SENSOR_LIST = 0x22, // "
        UPDATE_SENSOR_VALUE = 0x23, // #

        CONFIG_REQUEST_SSID = 0x24, // $
        CONFIG_SET_SSID = 0X25, // %
        CONFIG_SSID_UPDATE_SUCCESS = 0X32, // 2
        CURRENT_SSID = 0X26, // &

        CONFIG_REQUEST_IS_PASSWORD_SET = 0X28, // (
        CONFIG_SET_PASSWORD = 0X29, // )
        CONFIG_PASSWORD_UPDATE_SUCCESS = 0X33, // 3
        CONFIG_CURRENT_PASSWORD_IS_SET = 0X2A, // *
        
        CONFIG_REQUEST_SERVER_HOSTNAME = 0X2B, // +
        CONFIG_CURRENT_SERVER_HOSTNAME = 0X2C, // ,
        CONFIG_SET_SERVER_HOSTNAME = 0X2D, // -
        CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS = 0X34, // 4

        CONFIG_REQUEST_SERVER_PORT = 0X2E, // .
        CONFIG_CURRENT_SERVER_PORT = 0X30, // 0
        CONFIG_SET_SERVER_PORT = 0X31, // 1
        CONFIG_SERVER_PORT_UPDATE_SUCCESS = 0X35, // 5
        CONFIG_DELETE_TOUCH_CALIBRATION_FILE = 0X36, // 6
    };

    
}
