#pragma once
#include "Arduino.h"
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../GlobalDefinitions.h"
#include "../HSSerial/HSSerial.h"
#include <Queue.h>
#include "../QueueItem.h"

#define CALIBRATION_FILE "/calibrationData"
#define TOUCH_DEBOUNCE_TIME 1000


class DisplayHandler
{
private:
	TFT_eSPI tftDisplay;
	DataQueue<QUEUE_ITEM>* displayDataQueue;
	portMUX_TYPE displayQueueMux;

	void DispatchCommand();

	void(*UpdateCurentScreen)(String value) = NULL;
	void(*UpdateCurentScreenOnInterval)() = NULL;
	void(*DestoryCurrentScreen)() = NULL;
	bool(*HandleTouchPoint)(int x, int y) = NULL;

	void LoadNewScreen(char screenID);
	void(*AddItemToOutputQueue)(char key, String value);
	void(*AddItemToDisplayQueue)(char key, String value);

	unsigned long lastTouch = 0;
	void CalibrateTouch();

	void UnloadOldDataFromDisplayQueue();

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, String value), void(*AddItemToDisplayQueue_Func)(char key, String value));
	void Run();

};

