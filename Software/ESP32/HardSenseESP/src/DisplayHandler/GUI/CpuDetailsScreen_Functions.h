#pragma once
#include "HS_CPUScreen.h"
#include "../../QueueItem.h"

HS_CPUScreen * HS_Current_CPUScreen = NULL;


void Create_CpuScreen(Queues *newQueues, TFT_eSPI* TFT) {
	HS_Current_CPUScreen = new HS_CPUScreen(newQueues, TFT);
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

bool Handle_CpuScreen_Touch(int x, int y)
{
	HS_Current_CPUScreen->HandleTouch(x, y);
}
