#pragma once
#include "Arduino.h"
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../HSSerial/HSSerial.h"
#include <Queue.h>
#include "../QueueItem.h"

#define CALIBRATION_FILE "/calibrationData"
#define TOUCH_DEBOUNCE_TIME 250

enum DisplayCommands {
	ChangeScreen = 0,
	UpdateValue,
};

enum ScreenTypes {
	SplashScreen = 0,
	ConnectToNetwork,
	BluetoothConfigurator,
	Home,
};

class DisplayHandler
{
private:
	TFT_eSPI tftDisplay;
	DataQueue<QUEUE_ITEM>* displayDataQueue;
	portMUX_TYPE displayQueueMux;

	void DispatchCommand();

	void(*UpdateCurrentScreen)(String value) = NULL;
	void(*DestoryCurrentScreen)() = NULL;
	bool(*HandleTouchPoint)(int x, int y) = NULL;

	void LoadNewScreen(char screenID);
	void(*AddItemToOutputQueue)(char key, String value);

	unsigned long lastTouch = 0;
	void SetTouch();

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, String value));
	void Run();

};

