#pragma once
#include "HS_ScreenBase.h"
class HS_SplashScreen :
	public HS_ScreenBase
{
private:

public:
	HS_SplashScreen(TFT_eSPI* newTFT);
	~HS_SplashScreen();
	void UpdateScreen(String value);
	char* GetSensorList();
};

