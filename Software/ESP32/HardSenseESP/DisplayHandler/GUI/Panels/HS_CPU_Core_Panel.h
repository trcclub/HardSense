#pragma once
#include "HS_PanelBase.h"


#define CPU_CORE_PANEL_WIDTH 106
#define CPU_CORE_PANEL_HEIGHT 90
#define CPU_CORE_GRID_WIDTH (CPU_CORE_PANEL_WIDTH - 6)
#define CPU_CORE_GRID_HEIGHT (CPU_CORE_PANEL_HEIGHT - 6)

#define TERTIARY_TEXT_COLOR 0x65D6

class HS_CPU_Core_Panel : public HS_PanelBase
{
private:
	//TFT_eSprite* chartSprite = NULL;

public:
	double load = 0.0;
	double temperature = 0.0;
	double clock = 0.0;

	HS_CPU_Core_Panel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme);
	~HS_CPU_Core_Panel();

	void UpdateChart();
};

