/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include <SPI.h>
#include <TFT_eSPI.h>

#include "Bluetooth/BTConfigurator.h"

byte btButton = 22;

TFT_eSPI tftDisplay = TFT_eSPI();
BTConfigurator* btConfig;

void setup() {
	Serial.begin(115200);
	InitDisplay();
	InitButtons();

	CheckForBluetoothConfigRequest();
	
	//if (!SystemChecks()) {
	//	Spin();
	//}

	tftDisplay.print("Entering Normal Run Mode");
}


void loop() {
  
}

/*
bool SystemChecks()
{
	if (!SPIFFS.begin()) {
		tftDisplay.print("SPIFFS is not available.");
		return false;
	}

	return true;
}
*/
void CheckForBluetoothConfigRequest()
{
	if (!digitalRead(btButton))
	{
		tftDisplay.print("Entering Bluetooth Configurator");
		delay(2000);
		btConfig = new BTConfigurator(tftDisplay);
		btConfig->HandleBluetooth();
	}
}

void InitButtons()
{
	pinMode(btButton, INPUT_PULLUP);
}

void InitDisplay()
{
	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
	tftDisplay.setCursor(20, 20);
}

void Spin()
{
	while (true) {
		delay(100);
	}
}