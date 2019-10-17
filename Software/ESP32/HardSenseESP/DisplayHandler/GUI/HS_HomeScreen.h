#pragma once
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"

#define SCREEN_HOME_CPU_LOAD_DIAL_MIN -120
#define SCREEN_HOME_CPU_LOAD_DIAL_MAX 120
#define SCREEN_HOME_CPU_LOAD_DIAL_X 22
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 2

#define CPU_AREA_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2

class HS_HomeScreen :
	public HS_ScreenBase
{
private:
	HS_Dial_Widget* cpuLoadWidget;
	uint16_t Home_Screen_cpuLoadDial_CurrentRingColor;

	void Draw_CPU_Panel();
	void Update_CPU_Panel_Load(double percentage);
	void Update_CPU_Panel_Tempererature(double temp);
	void Update_CPU_Panel_ClockSpeed(double temp);

public:
	HS_HomeScreen(TFT_eSPI *newTFT);
	~HS_HomeScreen();
	void UpdateScreen(char* value);
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value));
};
