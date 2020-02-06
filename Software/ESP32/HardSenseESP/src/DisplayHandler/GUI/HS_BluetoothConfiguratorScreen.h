#pragma once
#include "HS_ScreenBase.h"
class HS_BluetoothConfiguratorScreen :
	public HS_ScreenBase
{
private:

public:
	HS_BluetoothConfiguratorScreen(TFT_eSPI* newTFT);
	~HS_BluetoothConfiguratorScreen();
	void UpdateScreen(String value);
	char* GetSensorList();
};

