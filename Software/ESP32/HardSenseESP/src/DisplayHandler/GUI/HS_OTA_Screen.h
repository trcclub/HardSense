#pragma once
#include "HS_ScreenBase.h"

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208


#define NET_PANEL_X 0
#define NET_PANEL_Y 40

class HS_OTAScreen :
	public HS_ScreenBase
{
private:
	HS_Theme otaScreenTheme;

	void Draw_NetworkConnectionPanel();
	void Update_ConnectingToNetwork(String netID);
	void Update_ConnectedToNetwork(String netID);
	void Update_NetworkConnectionInfo(String netID, uint16_t circleColor);

public:
	HS_OTAScreen(Queues *newQueues, TFT_eSPI* newTFT);
	~HS_OTAScreen();
	void UpdateScreen(String value);
	void HandleTouch(int x, int y);

	//void UpdateScreenOnInterval();
};

