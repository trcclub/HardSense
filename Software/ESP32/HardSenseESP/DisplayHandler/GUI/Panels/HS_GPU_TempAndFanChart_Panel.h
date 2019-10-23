#pragma once
#include "HS_PanelBase.h"
class HS_GPU_TempAndFanChart_Panel :
	public HS_PanelBase
{
private:
	TFT_eSprite* graphGridSprite = NULL;
	TFT_eSprite* gpuTempSprite = NULL;
	TFT_eSprite* gpuFanSprite = NULL;
	double temperature = 0;
	double fanLoad = 0;

public:
	HS_GPU_TempAndFanChart_Panel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme);
	~HS_GPU_TempAndFanChart_Panel();

	void UpdateGPUTemp(double temp);
	void UpdateGPUFanLoad(double load);
	void UpdateGraph();
	void DrawPanel();
};

