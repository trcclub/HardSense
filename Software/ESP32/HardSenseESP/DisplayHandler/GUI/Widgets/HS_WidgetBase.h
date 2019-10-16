#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>

#define AA_FONT_SMALL "SegoeUI-14"
#define AA_FONT_LARGE "SegoeUI-18"

class HS_WidgetBase
{
private:
	void HS_Load_Fonts();

public:
	HS_WidgetBase();
	~HS_WidgetBase();
};

