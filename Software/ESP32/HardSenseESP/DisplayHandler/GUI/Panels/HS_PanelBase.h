#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../../GlobalDefinitions.h"

class HS_PanelBase
{
private:

public:
	HS_PanelBase(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme);
	~HS_PanelBase();

	TFT_eSPI* TFT;
	TFT_eSprite* textPrinter_Sprite;
	HS_Theme theme;
	HS_Coords coords;

	void DrawBoxWithBorderAndDropShadow(HS_Coords hs_coords, HS_Theme hs_theme);
};

