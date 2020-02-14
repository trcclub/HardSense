#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(TFT_eSPI *newTFT)
{
	TFT = newTFT;
	SPIFFS.begin();


	textPrinter_Sprite = new TFT_eSprite(TFT);
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->setTextDatum(TR_DATUM);

	char d = 0xB0;
	degreesC = String(d);
	degreesC += "C";
	sprintf(degreesC_char, "%cC", d);
}

HS_ScreenBase::~HS_ScreenBase()
{
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->deleteSprite();
	delete(textPrinter_Sprite);
	AddItemToDisplayQueue = NULL;
	TFT = NULL;
}

void HS_ScreenBase::UpdateScreen(String value)
{

}

void HS_ScreenBase::SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value))
{

}


void HS_ScreenBase::DrawBoxWithBorderAndDropShadow(HS_Coords hs_coords, HS_Theme hs_theme)
{
	TFT->fillRect(hs_coords.x, hs_coords.y, hs_coords.w - 1, hs_coords.h - 1, hs_theme.panelBorderColor);
	TFT->fillRect(hs_coords.x + 2, hs_coords.y + 2, hs_coords.w - 5, hs_coords.h - 5, hs_theme.panelBGColor);

	TFT->drawFastHLine(hs_coords.x + 3, hs_coords.y + 3, hs_coords.w - 6, hs_theme.panelDropShadowColor);
	TFT->drawFastVLine(hs_coords.x + 2, hs_coords.y + 3, hs_coords.h - 6, hs_theme.panelDropShadowColor);
	TFT->drawFastHLine(hs_coords.x, hs_coords.y + hs_coords.h - 1, hs_coords.w, hs_theme.panelDropShadowColor);
	TFT->drawFastVLine(hs_coords.x + hs_coords.w - 1, hs_coords.y, hs_coords.h, hs_theme.panelDropShadowColor);
}
/*
void HS_ScreenBase::DrawBoxWithBorderAndDropShadow(int32_t x, int32_t y, int32_t w, int32_t h, int32_t borderColor, int32_t boxColor, int32_t dropShadowColor)
{			   	//0, 0, 200, 94
	TFT->fillRect(x, y, w-1, h-1, borderColor);
	TFT->fillRect(x+2, y+2, w-5, h-5, boxColor);

	TFT->drawFastHLine(x+3, y+3, w-6, dropShadowColor);
	TFT->drawFastVLine(x+2, y+3, h-6, dropShadowColor);
	TFT->drawFastHLine(x, y+h-1, w, dropShadowColor);
	TFT->drawFastVLine(x+w-1, y, h, dropShadowColor);
}
*/
void HS_ScreenBase::HandleTouch(int x, int y)
{

}

bool HS_ScreenBase::HiddenHomeScreen_Touched(int x, int y)
{
	if ((x >= HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_X && x <= HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_X) && (y >= HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_Y && y <= HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_Y))
	{
		return true;
	}
	return false;
}

void HS_ScreenBase::HS_Load_Fonts()
{
	if (!SPIFFS.begin()) {
		Serial.println("HS_ScreenBase::HS_Load_Fonts():: SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	// ESP32 will crash if any of the fonts are missing
	bool font_missing = false;
	//if (SPIFFS.exists("/SegoeUI-10.vlw") == false) font_missing = true;
	//if (SPIFFS.exists("/SegoeUI-12.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_missing = true;

	if (font_missing)
	{
		Serial.println("\r\HS_ScreenBase::HS_Load_Fonts():: Font missing in SPIFFS, did you upload it?");
		while (1) yield();
	}
}

void HS_ScreenBase::SetDisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	AddItemToDisplayQueue = AddItemToDisplayQueue_func;
}

void HS_ScreenBase::DisplayFreeHeap()
{
	Serial.println(ESP.getFreeHeap());
}
