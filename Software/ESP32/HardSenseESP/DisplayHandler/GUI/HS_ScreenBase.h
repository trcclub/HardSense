#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>

class HS_ScreenBase
{
public:
	TFT_eSPI *TFT;
	HS_ScreenBase(TFT_eSPI *newTFT);
	~HS_ScreenBase();
	virtual void UpdateScreen(char* value);
	virtual char* GetSensorList();

};

