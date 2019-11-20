#include "HS_CPU_Core_Panel.h"

HS_CPU_Core_Panel::HS_CPU_Core_Panel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme) : HS_PanelBase(newTFT, newCoords, newTheme)
{
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);

	/*
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_10PT);
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->setTextColor(TERTIARY_TEXT_COLOR, TFT_TRANSPARENT);
	textPrinter_Sprite->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);

	chartSprite = new TFT_eSprite(textPrinter_Sprite);
	chartSprite->setColorDepth(8);
	chartSprite->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	chartSprite->fillSprite(TFT_TRANSPARENT);
	*/
}


HS_CPU_Core_Panel::~HS_CPU_Core_Panel()
{
	/*
	chartSprite->unloadFont();
	chartSprite->deleteSprite();
	delete(chartSprite);
	*/
}

void HS_CPU_Core_Panel::UpdateChart()
{
	char buf[12];
	int pixel = map(long(load), 0, 100, 1, CPU_CORE_GRID_WIDTH - 1);

	textPrinter_Sprite->scroll(-1, 0);
	textPrinter_Sprite->drawFastVLine(CPU_CORE_GRID_WIDTH - 1, 0, CPU_CORE_GRID_HEIGHT, TFT_TRANSPARENT);
	textPrinter_Sprite->drawFastVLine(CPU_CORE_GRID_WIDTH - 1, (CPU_CORE_GRID_HEIGHT - pixel), 2, TFT_YELLOW);
	textPrinter_Sprite->pushSprite(coords.x, coords.y, TFT_TRANSPARENT);
	
	/*
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	chartSprite->pushSprite(0, 0);

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(TERTIARY_TEXT_COLOR, TFT_TRANSPARENT);
	textPrinter_Sprite->createSprite(70, 42);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);

	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 70, 0);

	sprintf(buf, "%.1f %s ", temperature, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 70, 14);

	sprintf(buf, "%.1f %%", load);
	textPrinter_Sprite->drawString(String(buf), 70, 28);

	textPrinter_Sprite->pushSprite(0, 0);
	*/
}