#include "QueueItem.h"


Queues::Queues()
{
	DataQueue<QUEUE_ITEM> displayQueue(20);
	DataQueue<QUEUE_ITEM> outputQueue(10);
	DataQueue<QUEUE_ITEM> commandQueue(2);

    displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
    outputQueueMux = portMUX_INITIALIZER_UNLOCKED;
    commandQueueMux = portMUX_INITIALIZER_UNLOCKED;
}

Queues::~Queues()
{

}

void Queues::AddItemToDisplayQueue(char key, String value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	portENTER_CRITICAL(&displayQueueMux);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMux);
}

void Queues::AddItemToOutputQueue(char key, String value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	portENTER_CRITICAL(&outputQueueMux);
	outputQueue.enqueue(qi);
	portEXIT_CRITICAL(&outputQueueMux);
}

void Queues::AddItemToCommandQueue(char key, String value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	qi.value = value;
	portENTER_CRITICAL(&commandQueueMux);
	commandQueue.enqueue(qi);
	portEXIT_CRITICAL(&commandQueueMux);
}
