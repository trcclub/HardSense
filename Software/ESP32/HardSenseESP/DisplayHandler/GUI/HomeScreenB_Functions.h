#pragma once
#include "HS_HomeScreen_B.h"

#include "../../QueueItem.h"


HS_HomeScreen_B* HS_Current_HomeScreenB = NULL;

void Create_HomeScreenB(TFT_eSPI* TFT) {
	HS_Current_HomeScreenB = new HS_HomeScreen_B(TFT);
}

void Destroy_HomeScreenB()
{
	if (HS_Current_HomeScreenB != NULL)
	{
		delete(HS_Current_HomeScreenB);
		HS_Current_HomeScreenB = NULL;
	}
}

void Update_HomeScreenB(String value)
{
	HS_Current_HomeScreenB->UpdateScreen(value);
}

char* Set_Home_ScreenB_SensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	HS_Current_HomeScreenB->SetSensorList(AddItemToOutputQueue_func);
	/*
	Serial.println("Home_Screen_SensorList():  Returning ret");
	return HS_Current_HomeScreen->GetSensorList();
	*/
}

bool Handle_HomeScreenB_Touch(int x, int y)
{
	HS_Current_HomeScreenB->HandleTouch(x, y);
}

void Set_HomeScreenB_DisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	HS_Current_HomeScreenB->SetDisplayQueue(AddItemToDisplayQueue_func);
}