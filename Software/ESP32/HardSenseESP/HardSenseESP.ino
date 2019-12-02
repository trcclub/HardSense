/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/
#include <ESP32Encoder.h>
#include "freertos/portmacro.h"
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

byte btButton = 5;

portMUX_TYPE displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE outputQueueMux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t* heartbeatTimer = NULL;

ESP32Encoder volumeEncoder;
volatile int32_t volumeLevel = 0;

void IRAM_ATTR onTimer()
{
  hsSerial.FireHeartbeat();
}

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
		"DisplayHandler",            /* pcName */
		3000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&Display_Core_Task_Handle,                 /* pxCreatedTask */
		0);

	QUEUE_ITEM qi;
	qi.key = DisplayCommands::ChangeScreen;
	qi.value = String(ScreenTypes::SplashScreen);
	displayQueue.enqueue(qi);


	//delay(10000);



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


	volumeEncoder.attachHalfQuad(36, 39);
	volumeEncoder.clearCount();
}


void loop() 
{
	if (!hsSerial.connectedToSomething) {
		hsSerial.ConnectToHardsenseServer();
	}
	hsSerial.HandleInput();
	HandleVolumeEncoder();

	while (!outputQueue.isEmpty())
	{
		portENTER_CRITICAL(&outputQueueMux);
		QUEUE_ITEM currItem = outputQueue.dequeue();
		portEXIT_CRITICAL(&outputQueueMux);

		hsSerial.AddStringToOutputMessage(TRANS__KEY(currItem.key), currItem.value);
	}
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

void HandleVolumeEncoder()
{
	
	uint32_t currValue = volumeEncoder.getCount();
	if (currValue != volumeLevel)
	{
		if (currValue > volumeLevel)
		{
			AddItemToOutputQueue(TRANS__KEY::INCREASE_VOLUME, "");
		}
		else 
		{
			AddItemToOutputQueue(TRANS__KEY::DECREASE_VOLUME, "");
		}
		volumeLevel = currValue;
	}
	
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
