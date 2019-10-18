#pragma once
#include "HS_ScreenBase.h"
class HS_ConnectToNetworkScreen :
	public HS_ScreenBase
{
private:

public:
	HS_ConnectToNetworkScreen(TFT_eSPI* newTFT);
	~HS_ConnectToNetworkScreen();
	void UpdateScreen(String value);
	char* GetSensorList();
};

