#pragma once
#include "HS_BluetoothConfiguratorScreen.h"


HS_BluetoothConfiguratorScreen* HS_Current_BluetoothConfiguratorScreen = NULL;

void Create_BluetoothConfiguratorScreen(TFT_eSPI* TFT) {
	Serial.println("START 1");
	HS_Current_BluetoothConfiguratorScreen = new HS_BluetoothConfiguratorScreen(TFT);
	Serial.println("START 2");
}

void Destroy_BluetoothConfiguratorScreen()
{
	if (HS_Current_BluetoothConfiguratorScreen != NULL)
	{
		delete(HS_Current_BluetoothConfiguratorScreen);
		HS_Current_BluetoothConfiguratorScreen = NULL;
	}
}

void Update_BluetoothConfiguratorScreen(String value)
{
	HS_Current_BluetoothConfiguratorScreen->UpdateScreen(value);
}
