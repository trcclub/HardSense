/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include "DisplayHandler/DisplayHandler.h"
#include "HSSerial/HSSerial.h"
#include <Queue.h>
#include "QueueItem.h"

TaskHandle_t TFT_Core_Handle;
DataQueue<QUEUE_ITEM> displayQueue(20);
DataQueue<QUEUE_ITEM> outputQueue(5);

DisplayHandler displayHandler;
HSSerial hsSerial;

byte btButton = 22;

int counter = 0;


void setup() {
	Serial.begin(115200);
	displayHandler.Init(&displayQueue);
	
	InitButtons();

	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandlerTask",            /* pcName */
		4000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&TFT_Core_Handle,                 /* pxCreatedTask */
		0);

	delay(10);

	if (!hsSerial.init(&outputQueue))
	{
		Serial.println("Failed to init SPIFFS");
		Spin();
	}

	if (IsBTButtonPressed())
	{
		Serial.println("Starting bluetooth...");
		hsSerial.HandleBluetoothConnection();
	}

	hsSerial.HandleWiFiSocketConnection();
}


void loop() {
	
	if (hsSerial.connectedToSomething)
	{
		hsSerial.HandleInput();
		hsSerial.HandleOutput();
	}
	
	delay(20);
}

void TFT_Core_Proc(void* parameter)
{
	while (true)
	{
		displayHandler.UpdateDisplay();
		delay(100);
	}
	
	delay(20);
}

void InitButtons()
{
	pinMode(btButton, INPUT_PULLUP);
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