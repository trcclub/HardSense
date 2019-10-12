#pragma once

#include <WiFi.h>
#include <BluetoothSerial.h>
#include "SerialInterface.h"
#include "../HSFileSystem/HSFileSystem.h"

class HSSerial
{
private:
	WiFiClient *client;
	BluetoothSerial *btSerial;

	int(HSSerial::*InputAvailable)();
	int(HSSerial::*ReadInputByte)();
	String(HSSerial::*ReadInputStringUntil)(char);
	int(HSSerial::*PrintMessageToOutput)(char);

	bool WaitForBTConnection();
	void AcceptNewConnection();

	int BT_Available();
	int BT_Read();
	int BT_PrintChar(char c);
	String BT_ReadStringUntil(char terminator);
	int WiFi_Available();
	int WiFi_Read();
	String WiFi_ReadStringUntil(char terminator);
	int WiFi_PrintChar(char c);
	void AddKeyToOutputMessage(byte key);
	void AddBoolToOutputMessage(byte key, bool value);
	void AddStringToOutputMessage(byte key, char *value);
	void ParseInput(String input);
	void DispatchCommand(char key, String val);

	char* OutputData;
	int OutputDataLength = 0;

public:
	HSSerial();
	~HSSerial();

	bool connectedToSomething = false;

	void HandleBluetoothConnection();
	void HandleWiFiSocketConnection();
	void HandleInput();
	void HandleOutput();
};

