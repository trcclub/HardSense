#pragma once
#include "HS_CPUScreen.h"
#include "../../QueueItem.h"

HS_CPUScreen * HS_Current_CPUScreen = NULL;


void Create_CpuScreen(TFT_eSPI* TFT) {
	HS_Current_CPUScreen = new HS_CPUScreen(TFT);
}

void Destroy_CpuScreen()
{
	if (HS_Current_CPUScreen != NULL)
	{
		delete(HS_Current_CPUScreen);
		HS_Current_CPUScreen = NULL;
	}
}

void Update_CpuScreen(String value)
{
	HS_Current_CPUScreen->UpdateScreen(value);
}

void Update_CpuScreen_OnInterval()
{
	HS_Current_CPUScreen->UpdateScreenOnInterval();
}

char* Set_CpuScreen_SensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	HS_Current_CPUScreen->SetSensorList(AddItemToOutputQueue_func);
}

bool Handle_CpuScreen_Touch(int x, int y)
{
	HS_Current_CPUScreen->HandleTouch(x, y);
}

void Set_CpuScreen_DisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	HS_Current_CPUScreen->SetDisplayQueue(AddItemToDisplayQueue_func);
}