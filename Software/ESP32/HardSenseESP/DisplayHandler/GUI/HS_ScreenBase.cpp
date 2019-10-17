#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(TFT_eSPI *newTFT)
{
	TFT = newTFT;
	HS_Load_Fonts();

	/*
	smallTextPrinter = new TFT_eSprite(TFT);
	largeTextPrinter = new TFT_eSprite(TFT);

	smallTextPrinter->setColorDepth(8);
	smallTextPrinter->loadFont(AA_FONT_SMALL);

	largeTextPrinter->setColorDepth(8);
	largeTextPrinter->loadFont(AA_FONT_LARGE);
	*/
}

HS_ScreenBase::~HS_ScreenBase()
{
	//delete(smallTextPrinter);
	//delete(largeTextPrinter);
}

void HS_ScreenBase::UpdateScreen(char* value)
{

}

void HS_ScreenBase::SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value))
{

}

void HS_ScreenBase::HS_Load_Fonts()
{
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
	else Serial.println("\r\HS_ScreenBase::HS_Load_Fonts():: Fonts found OK.");

}