#pragma once
#include "HS_SplashScreen.h"


HS_SplashScreen* HS_Current_SplashScreen = NULL;

void Create_SplashScreen(Queues *newQueues, TFT_eSPI* TFT) {
	HS_Current_SplashScreen = new HS_SplashScreen(newQueues, TFT);
}

void Destroy_SplashScreen()
{
	if (HS_Current_SplashScreen != NULL)
	{
		delete(HS_Current_SplashScreen);
		HS_Current_SplashScreen = NULL;
	}
}

void Update_SplashScreen(String value)
{
	HS_Current_SplashScreen->UpdateScreen(value);
}

void Update_SplashScreen_OnInterval()
{
	HS_Current_SplashScreen->UpdateScreenOnInterval();
}