#include "HS_WidgetBase.h"

HS_WidgetBase::HS_WidgetBase()
{
	SPIFFS.begin();
	//HS_Load_Fonts();
}

HS_WidgetBase::~HS_WidgetBase()
{

}

void HS_WidgetBase::HS_Load_Fonts()
{
	if (!SPIFFS.begin()) {
		Serial.println("HS_WidgetBase::HS_Load_Fonts:: SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	// ESP32 will crash if any of the fonts are missing
	bool font_missing = false;
	if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_missing = true;

	if (font_missing)
	{
		Serial.println("\r\HS_WidgetBase::HS_Load_Fonts:: Font missing in SPIFFS, did you upload it?");
		while (1) yield();
	}
}