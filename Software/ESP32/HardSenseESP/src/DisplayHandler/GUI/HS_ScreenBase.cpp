#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(Queues *newQueues, TFT_eSPI *newTFT)
{
	TFT = newTFT;
	allQueues = newQueues;
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
	TFT = NULL;
}

void HS_ScreenBase::SendSensorList(String fileName)
{
	File file = SPIFFS.open(fileName);
	if (!file) {
		return;
	}
	String line = "";
	while (file.available()) {
		line = file.readStringUntil('\n');
		if (line.length() > 0)
		{
			allQueues->AddItemToOutputQueue(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, line);
		}
	}
	file.close();
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
	if (SPIFFS.exists("/SegoeUI-10.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_missing = true;

	if (font_missing)
	{
		Serial.println("\r\HS_ScreenBase::HS_Load_Fonts():: Font missing in SPIFFS, did you upload it?");
		while (1) yield();
	}
}

/*
void HS_ScreenBase::DisplayFreeHeap()
{
	Serial.println(ESP.getFreeHeap());
}
*/