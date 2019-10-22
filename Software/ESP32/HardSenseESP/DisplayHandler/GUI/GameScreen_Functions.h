#pragma once
#include "HS_GameScreen.h"

#include "../../QueueItem.h"


HS_GameScreen *HS_Current_GameScreen = NULL;

void Create_GameScreen(TFT_eSPI* TFT) {
	HS_Current_GameScreen = new HS_GameScreen(TFT);
}

void Destroy_GameScreen()
{
	if (HS_Current_GameScreen != NULL)
	{
		delete(HS_Current_GameScreen);
		HS_Current_GameScreen = NULL;
	}
}

void Update_GameScreen(String value)
{
	HS_Current_GameScreen->UpdateScreen(value);
}

void Update_GameScreen_OnInterval()
{
	HS_Current_GameScreen->UpdateScreenOnInterval();
}

char* Set_GameScreen_SensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	HS_Current_GameScreen->SetSensorList(AddItemToOutputQueue_func);
}

bool Handle_GameScreen_Touch(int x, int y)
{
	HS_Current_GameScreen->HandleTouch(x, y);
}

void Set_GameScreen_DisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	HS_Current_GameScreen->SetDisplayQueue(AddItemToDisplayQueue_func);
}