#pragma once
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"

#define SCREEN_HOME_LOAD_DIAL_MIN -120
#define SCREEN_HOME_LOAD_DIAL_MAX 120

#define SCREEN_HOME_CPU_LOAD_DIAL_X 22
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 2

#define SCREEN_HOME_GPU_LOAD_DIAL_X 22
#define SCREEN_HOME_GPU_LOAD_DIAL_Y 97

#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2

#define CPU_PANEL_LOW_X 0
#define CPU_PANEL_HIGH_X 208
#define CPU_PANEL_LOW_Y 0
#define CPU_PANEL_HIGH_Y 94

class HS_HomeScreen :
	public HS_ScreenBase
{
private:
	HS_Dial_Widget* cpuLoadWidget;
	uint16_t Home_Screen_cpuLoadDial_CurrentRingColor;

	HS_Dial_Widget* gpuLoadWidget;
	uint16_t Home_Screen_gpuLoadDial_CurrentRingColor;

	bool CPU_Panel_Touched(int x, int y);

	void Draw_CPU_Panel();
	void Update_CPU_Panel_Load(double percentage);
	void Update_CPU_Panel_Temperature(double temp);
	void Update_CPU_Panel_ClockSpeed(double clock);
	void Update_CPU_Panel_Power(double power);

	void Draw_GPU_Panel();
	void Update_GPU_Panel_Load(double percentage);
	void Update_GPU_Panel_Temperature(double temp);
	void Update_GPU_Panel_ClockSpeed(double clock);
	void Update_GPU_Panel_FanControl(double load);

	void Draw_Net_Panel();
	void Update_Net_DownloadSpeed(double dSpeed);
	void Update_Net_UpLoadSpeed(double uSpeed);
	String GetSpeedString(double speed);

	void Draw_Ram_Panel();
	void Update_Ram_Useage(double dPercent);

public:
	HS_HomeScreen(TFT_eSPI *newTFT);
	~HS_HomeScreen();
	void UpdateScreen(String value);
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);
};
