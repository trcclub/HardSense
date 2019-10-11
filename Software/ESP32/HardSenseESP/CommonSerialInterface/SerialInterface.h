#pragma once
#define FIRMWARE_NAME "Template"
#define FIRMWARE_VERSION "0.1"

#define SERIAL_READ_TIMEOUT 1000 //milliseconds

enum TRANS__KEY : byte {
        SOT = 0x02, // STX (start of text)
        ETX = 0x03, //ETX (end of text)
        DATA_SIZE_END = 0x1F, // US (unit separator)
        PACKET_SIZE_END = 0x1E, // RS (record separator)
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
        CURRENT_SSID = 0X26, // &
        CONFIG_REQUEST_IS_PASSWORD_SET = 0X28, // {
        CONFIG_SET_PASSWORD = 0X29, // )
        CONFIG_CURRENT_PASSWORD_IS_SET = 0X2A, // *
	//TRANS__CONFIG_ 0X27, // '
	//TRANS__CONFIG_ 0X28, // (
	//TRANS__CONFIG_ 0X29, // )
	//TRANS__CONFIG_ 0X2a, // *

};
