#pragma once
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"

#define SCREEN_HOME_CPU_LOAD_DIAL_MIN -120
#define SCREEN_HOME_CPU_LOAD_DIAL_MAX 120
#define SCREEN_HOME_CPU_LOAD_DIAL_X 0
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 0


class HS_HomeScreen :
	public HS_ScreenBase
{
private:
	HS_Dial_Widget* cpuLoadWidget;
	uint16_t Home_Screen_cpuLoadDial_CurrentRingColor;
	void Update_CPU_Total_Load(double percentage);

public:
	HS_HomeScreen(TFT_eSPI *newTFT);
	~HS_HomeScreen();
	void UpdateScreen(char* value);
	char* GetSensorList();
};
