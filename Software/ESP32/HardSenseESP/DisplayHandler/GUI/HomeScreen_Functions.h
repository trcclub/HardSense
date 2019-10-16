#pragma once
#include "HS_HomeScreen.h"

HS_HomeScreen* HS_Current_HomeScreen = NULL;
void Create_HomeScreen(TFT_eSPI *TFT) {
	HS_Current_HomeScreen = new HS_HomeScreen(TFT);
}

void Destroy_HomeScreen()
{
	if (HS_Current_HomeScreen != NULL)
	{
		delete(HS_Current_HomeScreen);
		HS_Current_HomeScreen = NULL;
	}
}

void Update_HomeScreen(char* value)
{
	HS_Current_HomeScreen->UpdateScreen(value);
}

char* Home_Screen_SensorList()
{
	return HS_Current_HomeScreen->GetSensorList();
}
