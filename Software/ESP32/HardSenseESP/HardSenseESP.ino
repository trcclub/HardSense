/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include "HSSerial/HSSerial.h"

TaskHandle_t TFT_Core_Handle;
TFT_eSPI tftDisplay = TFT_eSPI();
HSSerial hsSerial;

byte btButton = 22;

int counter = 0;


void setup() {
	Serial.begin(115200);

	
	InitDisplay();
	InitButtons();
	delay(10);

	if (!hsSerial.init())
	{
		Serial.println("Failed to init SPIFFS");
		Spin();
	}

	if (IsBTButtonPressed())
	{
		Serial.println("Starting bluetooth...");
		hsSerial.HandleBluetoothConnection();
	}

	Serial.println("Passed the button check");
	hsSerial.HandleWiFiSocketConnection();

	//xTaskCreatePinnedToCore(
	//	TFT_Core_Proc,                  /* pvTaskCode */
	//	"DisplayHandler",            /* pcName */
	//	1000,                   /* usStackDepth */
	//	NULL,                   /* pvParameters */
	//	1,                      /* uxPriority */
	//	&TFT_Core_Handle,                 /* pxCreatedTask */
	//	0);

}


void loop() {
	/*
	if (hsSerial.connectedToSomething)
	{
		hsSerial.HandleInput();
		hsSerial.HandleOutput();
	}
	*/
	delay(10);
}

void TFT_Core_Proc(void* parameter){
	while (true)
	{
		counter++;
		tftDisplay.fillRect(0, 20, tftDisplay.width(), 40, TFT_BLUE);
		tftDisplay.setTextSize(2);
		tftDisplay.setCursor(40, 22);
		tftDisplay.print(counter);
		tftDisplay.print(" times in a loop");
		delay(500);
	}
}


void DrawBackground()
{
	tftDisplay.fillScreen(TFT_NAVY);
	tftDisplay.fillRect(6, 6, tftDisplay.width() - 12, tftDisplay.height() - 12, TFT_LIGHTGREY);
	tftDisplay.setTextColor(TFT_BLACK);
	tftDisplay.setTextSize(2);
	String str = "Bluetooth Configurator";
	tftDisplay.drawString(str, 10, 20);
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

bool IsBTButtonPressed()
{
	if (!digitalRead(btButton))
	{
		return true;
	}
	return false;
}

void Spin()
{
	while (true) {
		delay(100);
	}
}