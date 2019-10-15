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

	void(*UpdateCureentScreen)(TFT_eSPI TFT, char* value) = NULL;
	void(*DestoryCurrentScreen)(TFT_eSPI) = NULL;

	void LoadNewScreen(char screenID);
	void(*AddItemToOutputQueue)(char key, char* value);

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newQueue, void(*AddItemToOutputQueue_Func)(char key, char* value), portMUX_TYPE& newDisplayQueueMux);
	void Run();

};

