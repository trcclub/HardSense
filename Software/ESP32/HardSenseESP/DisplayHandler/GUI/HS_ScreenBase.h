#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../GlobalDefinitions.h"
#include "../../HSSerial/SerialInterface.h"

#define CPU_AREA_BGCOLOR TFT_NAVY

class HS_ScreenBase
{
private:
	void HS_Load_Fonts();
public:
	HS_ScreenBase(TFT_eSPI *newTFT);
	~HS_ScreenBase();

	TFT_eSPI *TFT;
	//TFT_eSprite *smallTextPrinter;
	//TFT_eSprite* largeTextPrinter;

	virtual void UpdateScreen(char* value);
	virtual void SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value));

};

