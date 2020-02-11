#pragma once
#include "HS_HomeScreen.h"
#include "../../QueueItem.h"

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

void Update_HomeScreen(String value)
{
	HS_Current_HomeScreen->UpdateScreen(value);
}

void Update_HomeScreen_OnInterval()
{
	HS_Current_HomeScreen->UpdateScreenOnInterval();
}

char* Set_HomeScreen_SensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	HS_Current_HomeScreen->SetSensorList(AddItemToOutputQueue_func);
}

bool Handle_HomeScreen_Touch(int x, int y)
{
	HS_Current_HomeScreen->HandleTouch(x, y);
}

void Set_HomeScreen_DisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	HS_Current_HomeScreen->SetDisplayQueue(AddItemToDisplayQueue_func);
}

