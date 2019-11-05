#pragma once
#include "HS_ScreenBase.h"

#define TEXT_COLOR TFT_WHITE
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2

class HS_CPUScreen :
	public HS_ScreenBase
{
private:
	HS_Theme cpuScreenTheme;

public:
	HS_CPUScreen(TFT_eSPI* newTFT);
	~HS_CPUScreen();

	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);
};


