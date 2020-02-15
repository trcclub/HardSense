#pragma once
#include "HS_OTA_Screen.h"


HS_OTAScreen* HS_Current_OTAScreen = NULL;

void Create_OTAScreen(Queues *newQueues, TFT_eSPI* TFT) {
	HS_Current_OTAScreen = new HS_OTAScreen(newQueues, TFT);
}

void Destroy_OTAScreen()
{
	if (HS_Current_OTAScreen != NULL)
	{
		delete(HS_Current_OTAScreen);
		HS_Current_OTAScreen = NULL;
	}
}

void Update_OTAScreen(String value)
{
	HS_Current_OTAScreen->UpdateScreen(value);
}

bool Handle_OTAScreen_Touch(int x, int y)
{
	HS_Current_OTAScreen->HandleTouch(x, y);
}
