#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../GlobalDefinitions.h"
#include "../../HSSerial/SerialInterface.h"

#define HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_X 270
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_X 320
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_Y 190
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_Y 240

class HS_ScreenBase
{
private:
	void HS_Load_Fonts();


public:
	HS_ScreenBase(TFT_eSPI *newTFT);
	~HS_ScreenBase();

	String degreesC = "";
	char degreesC_char[3];

	TFT_eSPI *TFT;
	TFT_eSprite* textPrinter_Sprite;

	void(*AddItemToDisplayQueue)(char key, String value);

	virtual void UpdateScreen(String value);
	virtual void SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value));
	virtual void HandleTouch(int x, int y);
	bool HiddenHomeScreen_Touched(int x, int y);
	void SetDisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value));

	void DrawBoxWithBorderAndDropShadow(int32_t x, int32_t, int32_t w, int32_t h, int32_t borderColor, int32_t boxColor, int32_t dropShadowColor);
	
};

