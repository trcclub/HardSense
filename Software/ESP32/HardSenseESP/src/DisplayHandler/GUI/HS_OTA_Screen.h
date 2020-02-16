#pragma once
#include "HS_ScreenBase.h"

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208


#define OTA_PANEL_X 20
#define OTA_PANEL_Y 40
#define OTA_PANEL_WIDTH 280
#define OAT_PANEL_HEIGHT 110

class HS_OTAScreen :
	public HS_ScreenBase
{
private:
	HS_Theme otaScreenTheme;

	unsigned long lastUpdate = 0;
	int updateScreenInterval = 100;
	int circleLoopCounter;	

/*
	//Busy icon
	unsigned long lastUpdate = 0;
	int updateScreenInterval = 15;
	void UpdateCircle();
	int circleLoopCounter;	
	int secondaryLoopCounter;
	uint32_t col[8];
*/

	bool Connected_To_Network;
	void Draw_OTA_Information_Panel();
	void Update_ConnectingToNetwork(String netID);
	void Update_ConnectedToNetwork(String netID);
	void Update_NetworkConnectionInfo(String netID, uint16_t circleColor);

	bool OTA_Update_In_Progress;
	void Starting_OTA_Update();
	void Draw_Waiting_Animation();
	void Update_In_Progress(int percentage);
	void Ending_OTA_Update();

public:
	HS_OTAScreen(Queues *newQueues, TFT_eSPI* newTFT);
	~HS_OTAScreen();
	void UpdateScreen(String value);
	void HandleTouch(int x, int y);
	void UpdateScreenOnInterval();
};

