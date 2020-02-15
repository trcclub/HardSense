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
	Queues *allQueues;

	void DispatchCommand();

	void(*UpdateCurentScreen)(String value) = NULL;
	void(*UpdateCurentScreenOnInterval)() = NULL;
	void(*DestroyCurrentScreen)() = NULL;
	bool(*HandleTouchPoint)(int x, int y) = NULL;

	void LoadNewScreen(ScreenTypes screenID);
	
	unsigned long lastTouch = 0;
	void CalibrateTouch();

	void UnloadOldDataFromDisplayQueue();

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(Queues *newQueues);
	void Run();

};

