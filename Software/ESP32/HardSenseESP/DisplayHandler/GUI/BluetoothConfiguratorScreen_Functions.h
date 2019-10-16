#pragma once
#include "HS_BluetoothConfiguratorScreen.h"


HS_BluetoothConfiguratorScreen* HS_Current_BluetoothConfiguratorScreen = NULL;

void Create_BluetoothConfiguratorScreen(TFT_eSPI* TFT) {
	HS_Current_BluetoothConfiguratorScreen = new HS_BluetoothConfiguratorScreen(TFT);
}

void Destroy_BluetoothConfiguratorScreen()
{
	if (HS_Current_BluetoothConfiguratorScreen != NULL)
	{
		delete(HS_Current_BluetoothConfiguratorScreen);
		HS_Current_BluetoothConfiguratorScreen = NULL;
	}
}

void Update_BluetoothConfiguratorScreen(char* value)
{
	HS_Current_BluetoothConfiguratorScreen->UpdateScreen(value);
}
