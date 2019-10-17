#pragma once
#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../HSSerial/HSSerial.h"
#include <Queue.h>
#include "../QueueItem.h"

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

	void(*UpdateCureentScreen)(char* value) = NULL;
	void(*DestoryCurrentScreen)() = NULL;

	void LoadNewScreen(char screenID);
	void(*AddItemToOutputQueue)(char key, char* value);

	void* currentScreen = NULL;

	//void GetSensorData(char*(*sensorFunc)());


public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, char* value));
	void Run();

};

