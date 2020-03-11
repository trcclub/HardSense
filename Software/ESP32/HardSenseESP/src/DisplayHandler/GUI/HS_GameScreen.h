#pragma once
#include "HS_ScreenBase.h"
#include "Panels/HS_MemPanel.h"
#include "Panels/HS_GPU_TempAndFanChart_Panel.h"
#include "Widgets/HS_Dial_Widget.h"
#include "Panels/HS_NetPanel.h"

/*  Light Blue
#define TEXT_COLOR TFT_WHITE
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2
*/

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208


#define LOAD_DIAL_MIN -120
#define LOAD_DIAL_MAX 120

#define GPU_CORE_LOAD_PANEL_X 227
#define GPU_CORE_LOAD_PANEL_Y 51	
#define GPU_CORE_LOAD_DIAL_X 228
#define GPU_CORE_LOAD_DIAL_Y 56

#define GPU_RAM_PANEL_X 227
#define GPU_RAM_PANEL_Y 0

#define GPU_TEMP_PANEL_X 0
#define GPU_TEMP_PANEL_Y 0

#define GPU_CLOCKS_PANEL_X 0
#define GPU_CLOCKS_PANEL_Y 150
#define GPU_CLOCKS_PANEL_CORE_CLOCK_YOFFSET 11
#define GPU_CLOCKS_PANEL_MEMORY_CLOCK_YOFFSET 40
#define GPU_CLOCKS_PANEL_SHADER_CLOCK_YOFFSET 69

#define FPS_PANEL_X 162
#define FPS_PANEL_Y 150

#define CPU_LOAD_PANEL_X 162
#define CPU_LOAD_PANEL_Y 195

#define NET_PANEL_X 209
#define NET_PANEL_Y 150

class HS_GameScreen :
	public HS_ScreenBase
{
private:
	HS_Theme gameScreenTheme;

	unsigned long lastUpdate = 0;
	int gpuTempAndFanGraphUpdateTime = 500;

	HS_GPU_TempAndFanChart_Panel* GPU_TempAndFanChart;
	void DrawTempPanel();

	HS_MemPanel* memPanel;
	void DrawMemPanel();

	HS_Dial_Widget* gpuCoreLoadWidget;
	uint16_t gpuCoreLoadDial_CurrentRingColor = TFT_GREEN;
	void DrawGPUCoreLoadPanel();
	void UpdateGPUCoreLoad(double load);

	void DrawClockSpeedPanel();
	void UpdateGPUCoreClock(double clock);
	void UpdateGPUMemoryClock(double clock);
	void UpdateGPUShaderClock(double clock);
	void UpdateGPUClockField(double clock, int yOffset);

	void DrawFPSPanel();
	void UpdateFPS(double fps);

	void DrawCPULoadPanel();
	void UpdateCPULoad(double load);

	HS_NetPanel* netPanel;
	void Draw_Net_Panel();
public:
	HS_GameScreen(Queues *newQueues, TFT_eSPI* newTFT);
	~HS_GameScreen();
	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	//void SendSensorList();
	void HandleTouch(int x, int y);
};

