#pragma once
#include "Arduino.h"
#include <BluetoothSerial.h>

class BTConfigurator
{
private:
	BluetoothSerial btSerial;

public:
	void HandleBluetooth();
};

