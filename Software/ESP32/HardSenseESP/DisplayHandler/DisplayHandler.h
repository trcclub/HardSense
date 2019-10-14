#pragma once
#include "Arduino.h"
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

	//void AddStringToOutputMessage(byte key, char* value);
	//void(* AddStringToOutputMessage)(byte, char*);
	void UpdateDisplay();

public:
	DisplayHandler();
	~DisplayHandler();
	void Init(DataQueue<QUEUE_ITEM>* newQueue);
	void Run();

};

