#pragma once
#include "Arduino.h"
#include <BluetoothSerial.h>
#include <TFT_eSPI.h>

#define TFT_GREY 0x5AEB

class BTConfigurator
{
private:
	BluetoothSerial btSerial;
	TFT_eSPI *tftDisplay;
	bool connected;
	char* OutputData;
	int OutputDataLength = 0;

	void HandleOutput();
	//bool HandleInput();
	void HandleInput();
	//void DispatchCommand(wchar_t key, char* val);
	void DispatchCommand(char key, String val);

	void DrawBackground();
	inline int GetXForCenteredText(String text);

	bool WaitForConnection();
	void AcceptNewConnection();
	void addKeyToOutput(byte key);
	void addBoolToOutput(byte key, bool val);
	void addCharArrayToOutput(byte key, const char* value, int valueLength);

	int counter;
public:
	BTConfigurator(TFT_eSPI inDisplay);
	void HandleBluetooth();
};

