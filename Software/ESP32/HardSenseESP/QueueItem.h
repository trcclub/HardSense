#pragma once
#include "Arduino.h"
#define MAX_QUEUE_ITEM_VALUE_SIZE 64
struct QUEUE_ITEM {
	char key;
	char value[MAX_QUEUE_ITEM_VALUE_SIZE];
};