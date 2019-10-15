/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include "DisplayHandler/DisplayHandler.h"
#include "HSSerial/HSSerial.h"
#include <Queue.h>
#include "QueueItem.h"

#define HEARTBEAT_TIMER_POLL_TIME 2000000

TaskHandle_t TFT_Core_Handle;
DataQueue<QUEUE_ITEM> displayQueue(20);
DataQueue<QUEUE_ITEM> outputQueue(5);

DisplayHandler displayHandler;
HSSerial hsSerial;

byte btButton = 22;

//int counter = 0;

portMUX_TYPE displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE outputQueueMux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t* heartbeatTimer = NULL;

void setup() {
	Serial.begin(115200);
	displayHandler.Init(&displayQueue, displayQueueMux, AddItemToOutputQueue);
	
	InitButtons();
	heartbeatTimer = timerBegin(0, 80, true);
	timerAttachInterrupt(heartbeatTimer, &onTimer, true);
	timerAlarmWrite(heartbeatTimer, HEARTBEAT_TIMER_POLL_TIME, true);

	QUEUE_ITEM qi;
	qi.key = DisplayCommands::ChangeScreen;
	sprintf(qi.value, "%c", ScreenTypes::SplashScreen);
	displayQueue.enqueue(qi);

	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandlerTask",            /* pcName */
		4000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&TFT_Core_Handle,                 /* pxCreatedTask */
		0);

	delay(2000);

	if (!hsSerial.Init(&outputQueue, outputQueueMux, AddItemToDisplayQueue, HeartbeatTimerEnabled))
	{
		Serial.println("Failed to init SPIFFS");
		Spin();
	}

	if (IsBTButtonPressed())
	{
		Serial.println("Starting bluetooth...");
		hsSerial.HandleBluetoothConnection();
	}

	hsSerial.HandleWiFiConnection();

}


void loop() {
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

void AddItemToDisplayQueue(char key, char* value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	strcpy(qi.value, value);
	portENTER_CRITICAL(&displayQueueMux);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMux);
}

void AddItemToOutputQueue(char key, char* value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	strcpy(qi.value, value);
	portENTER_CRITICAL(&outputQueueMux);
	outputQueue.enqueue(qi);
	portEXIT_CRITICAL(&outputQueueMux);
}


void IRAM_ATTR onTimer()
{
	hsSerial.FireHeartbeat();
}

void Spin()
{
	while (true) {
		delay(100);
	}
}