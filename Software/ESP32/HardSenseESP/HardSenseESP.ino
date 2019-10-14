/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include "DisplayHandler/DisplayHandler.h"
#include "HSSerial/HSSerial.h"
#include <Queue.h>
#include <Bounce2.h>
#include "QueueItem.h"

typedef struct infoButtons_s {
	int pin;
	Bounce* debouncer;
	void(*handle)();
}infoButtons;
#define BUTTON_DEBOUNCE_TIME 25

//const infoButtons buttonsTable[] = {
//	{ 23, new Bounce(), navInfoButton_Action },
//	{ 21, new Bounce(), jobInfoButton_Action },
//	{ 22, new Bounce(), truckInfoButton_Action }
//};

//int NUM_INFO_BUTTONS = (sizeof(infoButtonsTable) / sizeof(infoButtons_s));

TaskHandle_t TFT_Core_Handle;
DataQueue<QUEUE_ITEM> displayQueue(20);
DataQueue<QUEUE_ITEM> outputQueue(5);

DisplayHandler displayHandler;
HSSerial hsSerial;

byte btButton = 23;

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
	/*
	while (true)
	{
		HandleButtons();
		displayHandler.UpdateDisplay();
		delay(20);
	}
	*/

	delay(20);
}


void HandleButtons() 
{
	/*
	for (int i = 0; i < NUM_INFO_BUTTONS; i++) {
		infoButtonsTable[i].debouncer->update();
		if (infoButtonsTable[i].debouncer->fell())
			infoButtonsTable[i].handle();
	}
	*/
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