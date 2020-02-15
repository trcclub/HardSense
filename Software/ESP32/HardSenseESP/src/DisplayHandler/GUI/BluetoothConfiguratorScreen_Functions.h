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

void Update_BluetoothConfiguratorScreen(String value)
{
	HS_Current_BluetoothConfiguratorScreen->UpdateScreen(value);
}

void Update_BluetoothConfigurator_OnInterval()
{
	HS_Current_BluetoothConfiguratorScreen->UpdateScreenOnInterval();
}


bool Handle_BluetoothConfigurator_Touch(int x, int y)
{
	HS_Current_BluetoothConfiguratorScreen->HandleTouch(x, y);
}

void Set_BluetoothConfiguratorScreen_DisplayQueue(void(*AddItemToDisplayQueue_func)(char key, String value))
{
	HS_Current_BluetoothConfiguratorScreen->SetDisplayQueue(AddItemToDisplayQueue_func);
}