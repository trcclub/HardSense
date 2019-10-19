#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(TFT_eSPI *newTFT)
{
	TFT = newTFT;
	HS_Load_Fonts();

	/*
	smallTextPrinter = new TFT_eSprite(TFT);

	smallTextPrinter->setColorDepth(8);
	smallTextPrinter->loadFont(AA_FONT_SMALL);

	largeTextPrinter = new TFT_eSprite(TFT);
	largeTextPrinter->setColorDepth(8);
	largeTextPrinter->loadFont(AA_FONT_LARGE);
	*/

	char d = 0xB0;
	degreesC = String(d);
	degreesC += "C";
}

HS_ScreenBase::~HS_ScreenBase()
{
	TFT->unloadFont();
	//delete(smallTextPrinter);
	//delete(largeTextPrinter);
}

void HS_ScreenBase::UpdateScreen(String value)
{

}

void HS_ScreenBase::SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value))
{

}

void HS_ScreenBase::DrawBoxWithBorderAndDropShadow(int32_t x, int32_t y, int32_t w, int32_t h, int32_t borderColor, int32_t boxColor, int32_t dropShadowColor)
{			   	//0, 0, 200, 94
	TFT->fillRect(x, y, w, h, borderColor);
	TFT->fillRect(x+2, y+2, w-4, h-4, boxColor);

	TFT->drawFastHLine(x+3, y+3, w-5, dropShadowColor);
	TFT->drawFastVLine(x+2, y+3, h-5, dropShadowColor);
	TFT->drawFastHLine(x, y+h, w, dropShadowColor);
	TFT->drawFastVLine(x+w, y, h, dropShadowColor);
}

void HS_ScreenBase::HandleTouch(int x, int y)
{

}

void HS_ScreenBase::HS_Load_Fonts()
{
	if (!TFT->fontsLoaded())
	{
		Serial.println("HS_ScreenBase::HS_Load_Fonts():  NOT LOADED");
	}

	//if (!TFT->fontsLoaded()) 
	//{
		if (!SPIFFS.begin()) {
			Serial.println("HS_ScreenBase::HS_Load_Fonts():: SPIFFS initialisation failed!");
			while (1) yield(); // Stay here twiddling thumbs waiting
		}

		// ESP32 will crash if any of the fonts are missing
		bool font_missing = false;
		if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_missing = true;
		if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_missing = true;

		if (font_missing)
		{
			Serial.println("\r\HS_ScreenBase::HS_Load_Fonts():: Font missing in SPIFFS, did you upload it?");
			while (1) yield();
		}
	//}

}

void HS_ScreenBase::SetDisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	AddItemToDisplayQueue = AddItemToDisplayQueue_func;
}