/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include <WiFi.h>
#include <BluetoothSerial.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "SerialInterface/SerialInterface.h"

TaskHandle_t TFT_Core_Handle;
TFT_eSPI tftDisplay = TFT_eSPI();


WiFiClient client;
BluetoothSerial btSerial;

bool connectedToSomething = false;

int(*InputAvailable)();
int(*ReadInputByte)();
String(*ReadInputStringUntil)(char);
int(*PrintMessageToOutput)(String);

String OutputData;

byte btButton = 22;


int counter = 0;


void setup() {
	Serial.begin(115200);
	
	InitDisplay();
	InitButtons();
	
	//xTaskCreatePinnedToCore(
	//	TFT_Core_Proc,                  /* pvTaskCode */
	//	"DisplayHandler",            /* pcName */
	//	1000,                   /* usStackDepth */
	//	NULL,                   /* pvParameters */
	//	1,                      /* uxPriority */
	//	&TFT_Core_Handle,                 /* pxCreatedTask */
	//	0);

	CheckForBluetoothConfigRequest();

	InputAvailable = WiFi_Available;
	ReadInputByte = WiFi_Read;;
	ReadInputStringUntil = WiFi_ReadStringUntil;
	PrintMessageToOutput = WiFi_PrintString;
}


void loop() {
	//HandleInput();


	//HandleOutput();

	delay(10);
}

void TFT_Core_Proc(void* parameter){
	while (true)
	{
		counter++;
		tftDisplay.fillRect(0, 20, tftDisplay.width(), 40, TFT_BLUE);
		tftDisplay.setTextSize(2);
		tftDisplay.setCursor(40, 22);
		tftDisplay.print(counter);
		tftDisplay.print(" times in a loop");
		delay(500);
	}
}

void CheckForBluetoothConfigRequest()
{
	if (!digitalRead(btButton))
	{
		InputAvailable = BT_Available;
		ReadInputByte = BT_Read;;
		ReadInputStringUntil = BT_ReadStringUntil;
		PrintMessageToOutput = BT_PrintString;

		btSerial.setTimeout(500);
		btSerial.begin("HardSenseESP");

		while (true)
		{
			WaitForBTConnection();
			while (connectedToSomething)
			{
				HandleInput();

				HandleOutput();

				delay(20);
			}
		}
	}
}

bool WaitForBTConnection()
{
	DrawBackground();
	tftDisplay.setTextSize(1);
	String str = "Waiting for connection...";
	tftDisplay.drawString(str, 10, 60);

	while (!connectedToSomething)
	{
		HandleInput();
		if (connectedToSomething)
		{
			break;
		}

		HandleOutput();
		delay(20);
	}
}

void AcceptNewConnection()
{
	AddKeyToOutputMessage(TRANS__KEY::CONNECTION_ACK);
	connectedToSomething = true;
	DrawBackground();
	tftDisplay.setTextSize(1);
	String str = "Bluetooth conection established.";
	tftDisplay.drawString(str, 10, 60);
}
void DrawBackground()
{
	tftDisplay.fillScreen(TFT_NAVY);
	tftDisplay.fillRect(6, 6, tftDisplay.width() - 12, tftDisplay.height() - 12, TFT_LIGHTGREY);
	tftDisplay.setTextColor(TFT_BLACK);
	tftDisplay.setTextSize(2);
	String str = "Bluetooth Configurator";
	tftDisplay.drawString(str, 10, 20);
}

void InitButtons()
{
	pinMode(btButton, INPUT_PULLUP);
}

void InitDisplay()
{
	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
	tftDisplay.setCursor(20, 20);
}


int BT_Available() {
	return btSerial.available();
}

int BT_Read() {
	return btSerial.read();
}

int BT_PrintString(String s) {
	return btSerial.print(s);
}

String BT_ReadStringUntil(char terminator) {
	return btSerial.readStringUntil(terminator);
}

int WiFi_Available() {
	return client.available();
}

int WiFi_Read() {
	return client.read();
}

String WiFi_ReadStringUntil(char terminator) {
	return client.readStringUntil(terminator);
}

int WiFi_PrintString(String s) {
	return client.print(s);
}

void AddKeyToOutputMessage(byte key)
{
	AddStringToOutputMessage(key, "");
}

void AddBoolToOutputMessage(byte key, bool value) 
{
	value ? AddStringToOutputMessage(key, "1") : AddStringToOutputMessage(key, "0");
}

void AddStringToOutputMessage(byte key, String value)
{
	OutputData += key;
	OutputData += value;
	OutputData += TRANS__KEY::PACKET_END;
}

void HandleOutput() {
	if (OutputData.length() == 1) {
		return;
	}
	OutputData += TRANS__KEY::ETX;
	
	PrintMessageToOutput(OutputData);

	OutputData = "";
	OutputData += TRANS__KEY::STX;
}


void HandleInput() {
	if (!InputAvailable())
		return;

	while (ReadInputByte() != TRANS__KEY::STX) {}

	String str = ReadInputStringUntil(TRANS__KEY::ETX);
	Serial.print("HandleInput: '");
	Serial.print(str);
	Serial.println("'");
	ParseInput(str);
	//ParseInput(ReadInputStringUntil(TRANS__KEY::ETX));
}

void ParseInput(String input)
{
	int currIndex = input.indexOf(TRANS__KEY::PACKET_END);
	int start = 0;
	while (currIndex != -1) {
		String currToken = input.substring(start, currIndex);

		char key = currToken.charAt(0);
		String value = currToken.substring(1);

		DispatchCommand(key, value);

		start = currIndex + 1;
		currIndex = input.indexOf(TRANS__KEY::PACKET_END, start);
	}
}

//void BTConfigurator::DispatchCommand(wchar_t key, char* val) {
void DispatchCommand(char key, String val) {

	switch (key) {
	case TRANS__KEY::DISCONNECT:
		Serial.println("TRANS__KEY::DISCONNECT");
		//connected = false;
		break;
	case TRANS__KEY::CONNECTION_REQUEST:
		Serial.println("TRANS__KEY::CONNECTION_REQUEST");
		AcceptNewConnection();
		break;
	case TRANS__KEY::CONFIG_REQUEST_SSID:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_SSID");
		AddStringToOutputMessage(TRANS__KEY::CURRENT_SSID, "Starside");
		break;
	case TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_CURRENT_PASSWORD_IS_SET, true);
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_HOSTNAME:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_SERVER_HOSTNAME");
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_HOSTNAME, "Aerie");
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_PORT:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_SERVER_PORT");
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_PORT, "4121");
		break;
	case TRANS__KEY::CONFIG_SET_SSID:
		Serial.println("TRANS__KEY::CONFIG_SET_SSID");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SSID_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_PASSWORD:
		Serial.println("TRANS__KEY::CONFIG_SET_PASSWORD");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_PASSWORD_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME:
		Serial.println("TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_PORT:
		Serial.println("TRANS__KEY::CONFIG_SET_SERVER_PORT");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_PORT_UPDATE_SUCCESS, true);
		break;
	default:
		Serial.print("Unknown Command: '");
		Serial.println(key, HEX);
		break;
	}
}

void Spin()
{
	while (true) {
		delay(100);
	}
}