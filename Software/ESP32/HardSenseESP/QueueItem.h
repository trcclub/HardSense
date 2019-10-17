#pragma once
#include "Arduino.h"
#include "GlobalDefinitions.h"

#define MAX_QUEUE_ITEM_VALUE_SIZE 128

struct QUEUE_ITEM {
	char key;
	char value[MAX_QUEUE_ITEM_VALUE_SIZE];
};