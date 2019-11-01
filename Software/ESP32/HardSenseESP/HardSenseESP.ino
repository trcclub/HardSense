/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include "DisplayHandler/DisplayHandler.h"
#include "HSSerial/HSSerial.h"
#include <Queue.h>
#include "QueueItem.h"
#include "GlobalDefinitions.h"

TaskHandle_t Display_Core_Task_Handle;

DataQueue<QUEUE_ITEM> displayQueue(20);
DataQueue<QUEUE_ITEM> outputQueue(10);

DisplayHandler displayHandler;
HSSerial hsSerial;

byte btButton = 33;

portMUX_TYPE displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE outputQueueMux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t* heartbeatTimer = NULL;

void setup() {
	Serial.begin(115200);

	displayHandler.Init(&displayQueue, displayQueueMux, AddItemToOutputQueue, AddItemToDisplayQueue);

	// Init the bluetooth button
	pinMode(btButton, INPUT_PULLUP);

	heartbeatTimer = timerBegin(0, 80, true);
	timerAttachInterrupt(heartbeatTimer, &onTimer, true);
	timerAlarmWrite(heartbeatTimer, HEARTBEAT_TIMER_POLL_TIME, true);
	
	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandlerTask",            /* pcName */
		6000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&Display_Core_Task_Handle,                 /* pxCreatedTask */
		0);

	QUEUE_ITEM qi;
	qi.key = DisplayCommands::ChangeScreen;
	qi.value = String(ScreenTypes::SplashScreen);
	//sprintf(qi.value, "%c", ScreenTypes::SplashScreen);
	displayQueue.enqueue(qi);


	delay(1000);

	if (!hsSerial.Init(&outputQueue, outputQueueMux, AddItemToDisplayQueue, HeartbeatTimerEnabled))
	{
		Serial.println("Failed to init SPIFFS");
		while (true) {
			delay(100);  // Spin because we can't go any further with SPIFFS
		}
	}

	if (IsBTButtonPressed())
	{
		Serial.println("Starting bluetooth...");
		hsSerial.HandleBluetoothConnection();
	}

	hsSerial.HandleWiFiConnection();
}


void loop() 
{
	if (!hsSerial.connectedToSomething) {
		hsSerial.ConnectToHardsenseServer();
	}

	hsSerial.HandleInput();
	hsSerial.HandleOutput();
	delay(20);
}

void TFT_Core_Proc(void* parameter)
{
	displayHandler.Run();
	
	delay(20);
}

bool IsBTButtonPressed()
{
	if (!digitalRead(btButton))
	{
		return true;
	}
	return false;
}

void HeartbeatTimerEnabled(bool enabled)
{
	if (enabled)
	{
		timerAlarmEnable(heartbeatTimer);

	}
	else
	{
		timerAlarmDisable(heartbeatTimer);
	}
}

void AddItemToDisplayQueue(char key, String value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	portENTER_CRITICAL(&displayQueueMux);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMux);
}

void AddItemToOutputQueue(char key, String value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	portENTER_CRITICAL(&outputQueueMux);
	outputQueue.enqueue(qi);
	portEXIT_CRITICAL(&outputQueueMux);
}


void IRAM_ATTR onTimer()
{
	hsSerial.FireHeartbeat();
}
