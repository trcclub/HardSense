#pragma once
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"

#define SCREEN_HOME_LOAD_DIAL_MIN -120
#define SCREEN_HOME_LOAD_DIAL_MAX 120

#define SCREEN_HOME_CPU_LOAD_DIAL_X 22
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 2

#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2

#define CPU_PANEL_LOW_X 0
#define CPU_PANEL_HIGH_X 208
#define CPU_PANEL_LOW_Y 0
#define CPU_PANEL_HIGH_Y 94


class HS_GameScreen :
	public HS_ScreenBase
{
private:


public:
	HS_GameScreen(TFT_eSPI* newTFT);
	~HS_GameScreen();
	void UpdateScreen(String value);
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);

};

