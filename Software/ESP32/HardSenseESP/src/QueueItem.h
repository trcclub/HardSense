#pragma once
#include <Arduino.h>
#include <Queue.h>
#include "freertos/portmacro.h"

struct QUEUE_ITEM {
	char key;
	String value;
};

class Queues
{
private:

public:
	Queues();
	~Queues();
	DataQueue<QUEUE_ITEM> displayQueue;
	DataQueue<QUEUE_ITEM> outputQueue;
	DataQueue<QUEUE_ITEM> commandQueue;

	portMUX_TYPE displayQueueMux;
	portMUX_TYPE outputQueueMux;
	portMUX_TYPE commandQueueMux;

	void AddItemToDisplayQueue(char key, String value);
	void AddItemToOutputQueue(char key, String value);
	void AddItemToCommandQueue(char key, String value);

};