#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../../GlobalDefinitions.h"


class HS_WidgetBase
{
private:

public:
	void HS_Load_Fonts();
	HS_WidgetBase();
	~HS_WidgetBase();
};

