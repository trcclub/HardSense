#pragma once
#include "RTClib.h"
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"
#include "Panels/HS_NetPanel.h"
#include "Panels/HS_MemPanel.h"

#define SCREEN_HOME_LOAD_DIAL_MIN -120
#define SCREEN_HOME_LOAD_DIAL_MAX 120

#define SCREEN_HOME_CPU_LOAD_DIAL_X 22
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 2

#define SCREEN_HOME_GPU_LOAD_DIAL_X 22
#define SCREEN_HOME_GPU_LOAD_DIAL_Y 97

#define CPU_PANEL_X 0
#define CPU_PANEL_Y 0

#define GPU_PANEL_X 0
#define GPU_PANEL_Y 95

#define NET_PANEL_X 207
#define NET_PANEL_Y 53

#define RAM_PANEL_X 207
#define RAM_PANEL_Y 122

#define RAM_PANEL_X 207
#define RAM_PANEL_Y 122

#define HDD_PANEL_X 0
#define HDD_PANEL_Y 190

#define TIME_PANEL_X 207
#define TIME_PANEL_Y 0

/* Light Blue theme
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2
*/


#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2
#define TEXT_COLOR TFT_WHITE

// * Dark
/*
#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208
*/

#define CPU_PANEL_LOW_X 0
#define CPU_PANEL_HIGH_X 208
#define CPU_PANEL_LOW_Y 0
#define CPU_PANEL_HIGH_Y 94

#define GPU_PANEL_LOW_X GPU_PANEL_X
#define GPU_PANEL_HIGH_X (GPU_PANEL_X + 207)
#define GPU_PANEL_LOW_Y GPU_PANEL_Y
#define GPU_PANEL_HIGH_Y (GPU_PANEL_Y + 95)



class HS_HomeScreen :
	public HS_ScreenBase
{
private:
	HS_Dial_Widget* cpuLoadWidget;
	uint16_t Home_Screen_cpuLoadDial_CurrentRingColor;

	HS_Dial_Widget* gpuLoadWidget;
	uint16_t Home_Screen_gpuLoadDial_CurrentRingColor;

	HS_Theme homeScreenTheme;

	bool CPU_Panel_Touched(int x, int y);
	bool GPU_Panel_Touched(int x, int y);

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


	HS_NetPanel *netPanel;
	void Draw_Net_Panel();

	HS_MemPanel* memPanel;
	void Draw_Mem_Panel();
	//void Update_Mem_Useage(double dPercent);
	//void Update_Mem_Used(double used);
	//void Update_Mem_Free(double free);

	void Draw_HDD_Panel();
	void Update_HDD_Useage(char key, double percent);

	RTC_DS1307 rtc;
	unsigned long lastTimeUpate = 0;
	int updateTimeInterval = 1000;
	char daysOfTheWeek[7][5] = { "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat" };
	char monthsOfTheYear[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	void Draw_Time_Panel();
	void Update_Time();

public:
	HS_HomeScreen(TFT_eSPI *newTFT);
	~HS_HomeScreen();
	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);
};
