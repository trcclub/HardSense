#include "HS_PanelBase.h"
#include "../../../GlobalDefinitions.h"

HS_PanelBase::HS_PanelBase()
{
}

HS_PanelBase::~HS_PanelBase()
{
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->deleteSprite();
	delete(textPrinter_Sprite);
}

void HS_PanelBase::init(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme)
{
	TFT = newTFT;
	SPIFFS.begin();

	coords = newCoords;
	theme = newTheme;

	textPrinter_Sprite = new TFT_eSprite(TFT);
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->loadFont(AA_FONT_18PT);
}

void HS_PanelBase::DrawBoxWithBorderAndDropShadow(TFT_eSPI TFT, HS_Coords hs_coords, HS_Theme hs_theme)
{
	TFT.fillRect(hs_coords.x, hs_coords.y, hs_coords.w - 1, hs_coords.h - 1, hs_theme.panelBorderColor);
	TFT.fillRect(hs_coords.x + 2, hs_coords.y + 2, hs_coords.w - 5, hs_coords.h - 5, hs_theme.panelBGColor);

	TFT.drawFastHLine(hs_coords.x + 3, hs_coords.y + 3, hs_coords.w - 6, hs_theme.panelDropShadowColor);
	TFT.drawFastVLine(hs_coords.x + 2, hs_coords.y + 3, hs_coords.h - 6, hs_theme.panelDropShadowColor);
	TFT.drawFastHLine(hs_coords.x, hs_coords.y + hs_coords.h - 1, hs_coords.w, hs_theme.panelDropShadowColor);
	TFT.drawFastVLine(hs_coords.x + hs_coords.w - 1, hs_coords.y, hs_coords.h, hs_theme.panelDropShadowColor);
}