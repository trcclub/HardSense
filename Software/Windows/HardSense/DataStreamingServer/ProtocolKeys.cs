using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HardSense.DataStreamingServer
{
    class ProtocolKeys
    {
        public static Dictionary<string, string> TRANSMISSION_KEYS = new Dictionary<string, string>();
        public static void init()
        {
            TRANSMISSION_KEYS.Add("TRANS_START", "\u0002");  // STX (start of text)
            TRANSMISSION_KEYS.Add("TRANS__ETX", "\u0003"); //ETX (end of text)
            TRANSMISSION_KEYS.Add("TRANS__DATA_SIZE_END", "\u001F"); // US (unit separator)
            TRANSMISSION_KEYS.Add("TRANS__PACKET_SIZE_END", "\u001E"); // RS (record separator)
            TRANSMISSION_KEYS.Add("TRANS__PACKET_END", "\u001D"); // GS (group separator)
            TRANSMISSION_KEYS.Add("TRANS__ACK", "\u0006"); // ACK (acknowledge)
            TRANSMISSION_KEYS.Add("TRANS__NACK", "\u0015"); // NAK (negative acknowledge)

            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_REQUEST", "\u0005"); // ENQ (enquiry)
            TRANSMISSION_KEYS.Add("TRANS__CONNECTION_ACK", "\u0001"); // SOH (start of heading)
            TRANSMISSION_KEYS.Add("TRANS__DISCONNECT", "\u0004"); // EOT (end of transmission)

            TRANSMISSION_KEYS.Add("TRANS__REQUESTING_FULL_SENSOR_LIST", "\u0021"); // !
            TRANSMISSION_KEYS.Add("TRANS__SENDING_FULL_SENSOR_LIST", "\u0022"); // "
            TRANSMISSION_KEYS.Add("TRANS__REQUEST_SENSORS_FOR_STREAMING", "\u0023"); // #
            TRANSMISSION_KEYS.Add("TRANS__UPDATE_SENSOR_VALUE", "\u0024"); // $

        }
    }
}
