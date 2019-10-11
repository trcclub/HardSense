/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include <WiFi.h>
#include <BluetoothSerial.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "SerialProtocolKeys.h"

//#include "Bluetooth/BTConfigurator.h"

TaskHandle_t TFT_Core_Handle;

byte btButton = 22;

TFT_eSPI tftDisplay = TFT_eSPI();
WiFiClient client;
BluetoothSerial btSerial;
//BTConfigurator* btConfig;

int counter = 0;

int(*inputAvailable)();
int(*readInputByte)();
String(*readInputStringUntil)(char);


void setup() {
	Serial.begin(115200);
	
	InitDisplay();
	InitButtons();
	

	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandler",            /* pcName */
		1000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&TFT_Core_Handle,                 /* pxCreatedTask */
		0);

	CheckForBluetoothConfigRequest();

	inputAvailable = WiFi_Available;
	readInputByte = WiFi_Read;;
	readInputStringUntil = WiFi_ReadStringUntil;
	

}


void loop() {
	

	delay(10);
}

void TFT_Core_Proc(void* parameter){
	while (true)
	{
		counter++;
		tftDisplay.fillRect(0, 20, tftDisplay.width(), 40, TFT_BLUE);
		tftDisplay.setTextSize(2);
		tftDisplay.setCursor(30, 22);
		tftDisplay.print(counter);
		tftDisplay.print(" times in a loop");
		delay(500);
	}
}

void CheckForBluetoothConfigRequest()
{
	if (!digitalRead(btButton))
	{
		inputAvailable = BT_Available;
		readInputByte = BT_Read;;
		readInputStringUntil = BT_ReadStringUntil;

		//tftDisplay.print("Entering Bluetooth Configurator");
		//delay(2000);
//		btConfig = new BTConfigurator(tftDisplay);
//		btConfig->HandleBluetooth();
	}
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

int BT_Print(String s) {
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

int WiFi_Print(String s) {
	return btSerial.print(s);
}

inline const String BoolToString(bool b)
{
	return b ? "1" : "0";
}

void AddKeyToOutoutMessage(byte key) {
	AddStringToOutputMessage(key, "");
}

void AddBoolToOutputMessage(byte key, bool value) {
	AddStringToOutputMessage(key, BoolToString(value));
	String s = "";

}

void AddStringToOutputMessage(byte key, String value) {

}

void HandleOutput() {

}

void HandleInput() {
	if (!inputAvailable)
		return;

	while (readInputByte() != TRANS__KEY::STX) {}

	String input = readInputStringUntil(TRANS__KEY::ETX);
	Serial.print("INPUT: '");
	Serial.print(input);
	Serial.println("'");

	int currIndex = input.indexOf(TRANS__KEY::PACKET_END);
	int start = 0;
	while (currIndex != -1) {
		String currToken = input.substring(start, currIndex);

		char key = currToken.charAt(0);
		String value = "";
		value = currToken.substring(1);

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
		//AcceptNewBTConnection();
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

void AcceptNewConnection()
{
	AddKeyToOutoutMessage(TRANS__KEY::CONNECTION_ACK);
	//connected = true;
	//DrawBackground();
	//tftDisplay->setTextSize(1);
	//String str = "Bluetooth conection established.";
	//tftDisplay->drawString(str, GetXForCenteredText(str), 60);
}