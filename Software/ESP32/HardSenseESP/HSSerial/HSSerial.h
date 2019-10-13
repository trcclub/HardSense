#pragma once

#include <WiFi.h>
#include <BluetoothSerial.h>
#include "SerialInterface.h"
#include "../HSFileSystem/HSFileSystem.h"

class HSSerial
{
private:
	WiFiClient *wifiSerial;
	BluetoothSerial *btSerial;
	S_SETTNGS hardsenseSettings;

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
	void AddIntToOutputMessage(byte key, int val);
	void AddBoolToOutputMessage(byte key, bool value);
	void AddStringToOutputMessage(byte key, char *value);
	void ParseInput(String input);
	void DispatchCommand(char key, String val);

	bool IsPasswordSet();
	bool UpdateSetting(char key, String value);
	bool SaveSettingsToFS();
	char* OutputData;
	int OutputDataLength = 0;

public:
	HSSerial();
	~HSSerial();
	bool init();

	bool connectedToSomething = false;

	void HandleBluetoothConnection();
	void HandleWiFiSocketConnection();
	void HandleInput();
	void HandleOutput();
};

