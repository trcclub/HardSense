#include "HS_GPU_TempAndFanChart_Panel.h"

HS_GPU_TempAndFanChart_Panel::HS_GPU_TempAndFanChart_Panel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme) : HS_PanelBase(newTFT, newCoords, newTheme)
{
	graphGridSprite = new TFT_eSprite(TFT);
	graphGridSprite->setColorDepth(16);

	gpuTempSprite = new TFT_eSprite(TFT);
	gpuTempSprite->setColorDepth(16);
	
	gpuFanSprite = new TFT_eSprite(TFT);
	gpuFanSprite->setColorDepth(16);


	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_14PT);
}

HS_GPU_TempAndFanChart_Panel::~HS_GPU_TempAndFanChart_Panel()
{
	graphGridSprite->unloadFont();
	graphGridSprite->deleteSprite();
	delete(graphGridSprite);

	gpuTempSprite->unloadFont();
	gpuTempSprite->deleteSprite();
	delete(gpuTempSprite);

	gpuFanSprite->unloadFont();
	gpuFanSprite->deleteSprite();
	delete(gpuFanSprite);
}

void HS_GPU_TempAndFanChart_Panel::DrawPanel()
{
	coords.w = 227;
	coords.h = 150;
	int textSpriteH = 10;

	DrawBoxWithBorderAndDropShadow(coords, theme);
	TFT->fillRect(coords.x + 3, coords.y + 3, coords.w - 6, textSpriteH + 4, theme.panelHeaderColor);

	textPrinter_Sprite->createSprite(coords.w - 6, 12);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelHeaderColor);
	//textPrinter_Sprite->drawString("GPU Core Temp & Fan", (coords.w - 6) / 2, 0);
	textPrinter_Sprite->drawString("EVGA NVIDIA GeForce GTX 1080 Ti", (coords.w - 8) / 2, 0);
	textPrinter_Sprite->pushSprite(coords.x + 3, coords.y + 4,TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(coords.x + 3, coords.y + (textSpriteH + 7), coords.w - 6, theme.panelBorderColor);
	TFT->drawFastHLine(coords.x + 3, coords.y + (textSpriteH + 8), coords.w - 6, theme.panelBorderColor);


	graphGridSprite->createSprite(220, 100);
	graphGridSprite->fillSprite(theme.panelBGColor);
	graphGridSprite->drawRect(19, 0, 180, 100, theme.panelBorderColor);

	//Draw horizontal grid lines
	for (int i = 10; i < 100; i += 10) {
		graphGridSprite->drawFastHLine(coords.x + 19, i, 180, theme.panelBorderColor);
	}

	//Draw vertical grid lines
	for (int i = 20; i < 180; i += 20) {
		graphGridSprite->drawFastVLine(i + 19, 0, 100, theme.panelBorderColor);
	}
	graphGridSprite->loadFont(AA_FONT_12PT);
	graphGridSprite->setTextColor(TFT_YELLOW, theme.panelBGColor);
	graphGridSprite->setTextDatum(TL_DATUM);
	graphGridSprite->drawString("100", 201, 0);
	graphGridSprite->setTextDatum(ML_DATUM);
	graphGridSprite->drawString("50", 201, 50);
	graphGridSprite->setTextDatum(BL_DATUM);
	graphGridSprite->drawString("0", 201, 100);

	graphGridSprite->setTextColor(TFT_GREEN, theme.panelBGColor);
	graphGridSprite->setTextDatum(TR_DATUM);
	graphGridSprite->drawString("100", 18, 0);
	graphGridSprite->setTextDatum(MR_DATUM);
	graphGridSprite->drawString("50", 18, 50);
	graphGridSprite->setTextDatum(BR_DATUM);
	graphGridSprite->drawString("0", 18, 100);

	

	DrawBoxWithBorderAndDropShadow(HS_Coords(coords.x + 22,coords.y + 125, 80, 21), theme);

	textPrinter_Sprite->createSprite(25, 10);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(TFT_GREEN, theme.panelBGColor);
	textPrinter_Sprite->drawString("Fan", 0,0);
	textPrinter_Sprite->pushSprite(coords.x + 25, coords.y + 131, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();


	DrawBoxWithBorderAndDropShadow(HS_Coords(coords.x + 111, coords.y + 125, 92, 21), theme);
	textPrinter_Sprite->createSprite(35, 12);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(TFT_YELLOW, theme.panelBGColor);
	textPrinter_Sprite->drawString("Temp", 0, 0);
	textPrinter_Sprite->pushSprite(coords.x + 115, coords.y + 130, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();


	gpuTempSprite->createSprite(178, 100);
	gpuTempSprite->fillScreen(TFT_TRANSPARENT);
}

void HS_GPU_TempAndFanChart_Panel::UpdateGPUTemp(double temp)
{
	temperature = temp;

	textPrinter_Sprite->createSprite(45, 10);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(TFT_YELLOW, theme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.1f %s", temp,degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 40, 0);
	textPrinter_Sprite->pushSprite(coords.x + 155, coords.y + 131, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}

void HS_GPU_TempAndFanChart_Panel::UpdateGPUFanLoad(double load)
{
	fanLoad = load;

	textPrinter_Sprite->createSprite(40, 10);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(TFT_GREEN, theme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.1f %%", load);
	textPrinter_Sprite->drawString(String(buf), 40, 0);
	textPrinter_Sprite->pushSprite(coords.x + 57, coords.y + 131, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}

void HS_GPU_TempAndFanChart_Panel::UpdateGraph()
{
	gpuTempSprite->scroll(-1, 0);
	gpuTempSprite->drawFastVLine(177, 0, 100, TFT_TRANSPARENT);
	
	gpuTempSprite->drawFastVLine(177, 100-temperature, 2, TFT_YELLOW);
	gpuTempSprite->drawFastVLine(177, 100-fanLoad, 2, TFT_GREEN);

	graphGridSprite->pushSprite(coords.x + 3, coords.y + 23);
	gpuTempSprite->pushSprite(coords.x + 23, coords.y + 23,TFT_TRANSPARENT);
}