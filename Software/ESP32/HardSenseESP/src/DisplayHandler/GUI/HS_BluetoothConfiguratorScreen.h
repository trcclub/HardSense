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

public:
	HS_BluetoothConfiguratorScreen(TFT_eSPI* newTFT);
	~HS_BluetoothConfiguratorScreen();
	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	char* GetSensorList();
};

