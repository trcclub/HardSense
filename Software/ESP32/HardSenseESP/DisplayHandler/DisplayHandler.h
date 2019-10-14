#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>
#include "../HSSerial/HSSerial.h"
#include <Queue.h>
#include "../QueueItem.h"

class DisplayHandler
{
private:
	TFT_eSPI tftDisplay;
	DataQueue<QUEUE_ITEM>* displayDataQueue;

	void DrawBackground();
	int counter;

	void DrawWelcomeScreen();
	void(DisplayHandler::* UpdateScreen)();

	//void AddStringToOutputMessage(byte key, char* value);
	//void(* AddStringToOutputMessage)(byte, char*);

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newQueue);

	//Call UpdateDisplay() to, well, update the display.
	void UpdateDisplay();
	//void SetAddStringFunction(void (*func)(byte key, char* value));
};

