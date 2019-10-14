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

	void DispatchCommand();

	void(*UpdateCureentScreen)(TFT_eSPI TFT, char key, char* value) = NULL;
	void(*DestoryCurrentScreen)(TFT_eSPI) = NULL;

	void LoadNewScreen(String screenID);
	void DestroyCurrentScreen();

	//void DrawBackground();
	int counter;

	
public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newQueue);
	void Run();

};

