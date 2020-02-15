#pragma once
#include "HS_GameScreen.h"

#include "../../QueueItem.h"


HS_GameScreen *HS_Current_GameScreen = NULL;

void Create_GameScreen(Queues *newQueues, TFT_eSPI* TFT) {
	HS_Current_GameScreen = new HS_GameScreen(newQueues, TFT);
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

bool Handle_GameScreen_Touch(int x, int y)
{
	HS_Current_GameScreen->HandleTouch(x, y);
}
