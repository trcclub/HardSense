#pragma once
#include "HS_ConnectToNetworkScreen.h"


HS_ConnectToNetworkScreen* HS_Current_ConnectToNetworkScreen = NULL;

void Create_ConnectToNetworkScreen(TFT_eSPI* TFT) {
	HS_Current_ConnectToNetworkScreen = new HS_ConnectToNetworkScreen(TFT);
}

void Destroy_ConnectToNetworkScreen()
{
	if (HS_Current_ConnectToNetworkScreen != NULL)
	{
		delete(HS_Current_ConnectToNetworkScreen);
		HS_Current_ConnectToNetworkScreen = NULL;
	}
}

void Update_ConnectToNetworkScreen(char* value)
{
	HS_Current_ConnectToNetworkScreen->UpdateScreen(value);
}
