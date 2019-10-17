#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../../GlobalDefinitions.h"


class HS_WidgetBase
{
private:
	void HS_Load_Fonts();

public:
	HS_WidgetBase();
	~HS_WidgetBase();
};

