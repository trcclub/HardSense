#pragma once
#define FIRMWARE_NAME "HARDSENSE_ESP"
#define FIRMWARE_VERSION "0.1"

#define SERIAL_READ_TIMEOUT 500 //milliseconds

enum TRANS__KEY : byte {
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
		SENDING_FULL_SENSOR_LIST = 0x22, // "BTConfigurator.h
		UPDATE_SENSOR_VALUE = 0x23, // #

		CONFIG_REQUEST_SSID = 0x24, // $
		CONFIG_SET_SSID = 0X25, // %
		CONFIG_SSID_UPDATE_SUCCESS = 0X32, // 2
		CURRENT_SSID = 0X26, // &

		CONFIG_REQUEST_IS_PASSWORD_SET = 0X28, // {
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
};
