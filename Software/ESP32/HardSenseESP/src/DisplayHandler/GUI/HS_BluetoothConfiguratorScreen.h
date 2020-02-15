#pragma once
#include "HS_ScreenBase.h"
#include "RTClib.h"


#define TEXT_COLOR 0xF3CF
#define PANEL_HCOLOR 0x9800
#define PANEL_BGCOLOR 0xD0C3
#define BOX_BORDER_COLOR 0xC34D
#define BOX_DROP_SHADOW 0xB841

#define TIME_PANEL_X 207
#define TIME_PANEL_Y 0

#define NETWORK_PANEL_X 0
#define NETWORK_PANEL_Y 0

#define SERVER_PANEL_X 0
#define SERVER_PANEL_Y 64

#define DEVICEID_PANEL_X 0
#define DEVICEID_PANEL_Y 128

#define OTA_PANEL_X 207
#define OTA_PANEL_Y 52

#define OTA_PANEL_LOW_X 207
#define OTA_PANEL_HIGH_X 320
#define OTA_PANEL_LOW_Y 52
#define OTA_PANEL_HIGH_Y 179

class HS_BluetoothConfiguratorScreen :
	public HS_ScreenBase
{
private:
	HS_Theme bluetoothConfiguratorScreenTheme;

	RTC_DS1307 rtc;
	unsigned long lastTimeUpate = 0;
	int updateTimeInterval = 1000;
	void Draw_Time_Panel();
	void Update_Time();

	void Draw_Network_Panel();
	void UpdateSSID(String ssid);
	void UpdatePassword(String password);

	void Draw_Server_Panel();
	void Update_Server_Name(String serverName);
	void Update_Server_Port(int port);

	void Draw_DeviceID_Panel();
	void Update_WiFi_DeviceID(String wifiDID);
	void Update_BT_DeviceID(String btDID);

	void Draw_OTA_Panel();
	bool OTA_Panel_Touched(int x, int y);
public:
	HS_BluetoothConfiguratorScreen(TFT_eSPI* newTFT);
	~HS_BluetoothConfiguratorScreen();
	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	void HandleTouch(int x, int y);
	char* GetSensorList();
};

