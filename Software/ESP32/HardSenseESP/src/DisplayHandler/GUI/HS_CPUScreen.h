#pragma once
#include "HS_ScreenBase.h"

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208

#define TERTIARY_TEXT_COLOR TFT_GREEN

#define CPU_PACKAGE_PANEL_X 0
#define CPU_PACKAGE_PANEL_Y 0

#define CPU_PACKAGE_LOAD_GRID_X 186
#define CPU_PACKAGE_LOAD_GRID_Y 0

#define CPU_CORE_PANEL_START_X 0
#define CPU_CORE_PANEL_START_Y 60


#define CPU_CORE_PANEL_WIDTH 106
#define CPU_CORE_PANEL_HEIGHT 90
#define CPU_CORE_GRID_WIDTH (CPU_CORE_PANEL_WIDTH - 6)
#define CPU_CORE_GRID_HEIGHT (CPU_CORE_PANEL_HEIGHT - 6)

#define CPU_NUMBER_OF_CORES 6


struct CPU_CORE {
	HS_Coords localCoords;
	double load = 0.0;
	double temperature = 0.0;
	double clock = 0.0;
	TFT_eSprite* grid = NULL;

	CPU_CORE() {
	}

	CPU_CORE(HS_Coords newCoords) {
		localCoords = newCoords;
	}
	
	CPU_CORE(const CPU_CORE& orig)
	{
		localCoords = orig.localCoords;
		load = orig.load;
		temperature = orig.temperature;
		clock = orig.clock;
		grid = orig.grid;
	}
};


class HS_CPUScreen :
	public HS_ScreenBase
{
private:
	HS_Theme cpuScreenTheme;

	void DrawPackagePanel();
	void UpdatePackagePower(double power);
	void UpdatePackageCoresPower(double power);
	void UpdatePackageTemp(double temp);
	void UpdatePackageField(String str, int xIn, int yIn);

	TFT_eSprite* packageLoadGridSprite = NULL;
	TFT_eSprite* packageLoadSprite = NULL;
	double packageLoad = 0.0;
	void DrawPackageLoadGrid();
	void UpdatePackageLoadGraph();
	unsigned long lastUpdate = 0;
	int packageLoadGraphUpdateTime = 500;


	TFT_eSprite* cpuCorePanelGridSprite = NULL;
	void InitCPUCorePanels();
	void CreateCPUCorePanel(int core);
	CPU_CORE cores[CPU_NUMBER_OF_CORES];
	void UpdateCPUCorePanel(int core);
	
public:
	HS_CPUScreen(Queues *newQueues, TFT_eSPI* newTFT);
	~HS_CPUScreen();

	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	//void SendSensorList();
	void HandleTouch(int x, int y);
};




