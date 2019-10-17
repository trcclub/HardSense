#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../GlobalDefinitions.h"
#include "../../HSSerial/SerialInterface.h"


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

	String degreesC = "";

	virtual void UpdateScreen(char* value);
	virtual void SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value));

	void DrawBoxWithBorderAndDropShadow(int32_t x, int32_t, int32_t w, int32_t h, int32_t borderColor, int32_t boxColor, int32_t dropShadowColor);
	void DrawBoxDropShadow(int32_t x, int32_t, int32_t w, int32_t h, int32_t boxColor);
};

