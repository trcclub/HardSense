/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/
#include <ESP32Encoder.h>
#include "freertos/task.h"
#include "src/DisplayHandler/DisplayHandler.h"
#include "src/HSSerial/HSSerial.h"
#include "src/QueueItem.h"
#include "src/GlobalDefinitions.h"

TaskHandle_t Display_Core_Task_Handle;
DisplayHandler displayHandler;
static HSSerial hsSerial;
static Queues allQueues;

const byte btButton = 27;

ESP32Encoder volumeEncoder;
const byte encoderA_pin = 34;
const byte encoderB_pin = 35;
volatile int32_t volumeLevel = 0;

hw_timer_t* heartbeatTimer = NULL;
void IRAM_ATTR onTimer()
{
  hsSerial.FireHeartbeat();
}

void setup() {
	Serial.begin(115200);

	displayHandler.Init(&allQueues);

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

	if (!hsSerial.Init(&allQueues, HeartbeatTimerEnabled))
	{
		Serial.println("Failed to init SPIFFS");
		while (true) {
			delay(100);  // Spin because we can't go any further without SPIFFS
		}
	}

	volumeEncoder.attachHalfQuad(encoderA_pin, encoderB_pin);
	volumeEncoder.clearCount();
	
	if(IsBTButtonPressed())
	{
		allQueues.AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::BluetoothConfigurator));
		hsSerial.HandleConfigurator();
	}
	allQueues.AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::SplashScreen));
}


void loop() 
{
	if (!hsSerial.connectedToSomething) {
		hsSerial.ConnectToHardsenseServer();
	}
	
	hsSerial.HandleInput();
	HandleVolumeEncoder();
	yield();
	
	while (!allQueues.outputQueue.isEmpty())
	{
		portENTER_CRITICAL(&allQueues.outputQueueMux);
		QUEUE_ITEM currItem = allQueues.outputQueue.dequeue();
		portEXIT_CRITICAL(&allQueues.outputQueueMux);

		hsSerial.AddStringToOutputMessage(TRANS__KEY(currItem.key), currItem.value);
	}
	hsSerial.HandleOutput();

	yield();
}

void TFT_Core_Proc(void* parameter)
{
	displayHandler.Run();
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
			allQueues.AddItemToOutputQueue(TRANS__KEY::INCREASE_VOLUME, "");
		}
		else 
		{
			allQueues.AddItemToOutputQueue(TRANS__KEY::DECREASE_VOLUME, "");
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