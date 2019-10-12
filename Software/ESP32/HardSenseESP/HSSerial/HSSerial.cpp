#include "HSSerial.h"

HSSerial::HSSerial()
{
	connectedToSomething = false;
	client = NULL;
	btSerial = NULL;
}

HSSerial::~HSSerial()
{
	if (client != NULL)
	{
		delete(client);
	}
	if (btSerial != NULL)
	{
		delete(btSerial);
	}
}

void HSSerial::HandleBluetoothConnection()
{
	btSerial = new BluetoothSerial();
	InputAvailable = &HSSerial::BT_Available;
	ReadInputByte = &HSSerial::BT_Read;;
	ReadInputStringUntil = &HSSerial::BT_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::BT_PrintChar;

	btSerial->setTimeout(500);
	btSerial->begin("HardSenseESP");

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

bool HSSerial::WaitForBTConnection()
{
	while (!connectedToSomething)
	{
		HandleInput();
		HandleOutput();
		if (connectedToSomething)
		{
			break;
		}

		delay(20);
	}
}

void HSSerial::HandleWiFiSocketConnection()
{
	InputAvailable = &HSSerial::WiFi_Available;
	ReadInputByte = &HSSerial::WiFi_Read;;
	ReadInputStringUntil = &HSSerial::WiFi_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::WiFi_PrintChar;



}

void HSSerial::AcceptNewConnection()
{
	AddKeyToOutputMessage(TRANS__KEY::CONNECTION_ACK);
	connectedToSomething = true;
}

int HSSerial::BT_Available() {
	return btSerial->available();
}

int HSSerial::BT_Read() {
	return btSerial->read();
}

int HSSerial::BT_PrintChar(char c) {
	return btSerial->print(c);
}

String HSSerial::BT_ReadStringUntil(char terminator) {
	return btSerial->readStringUntil(terminator);
}

int HSSerial::WiFi_Available() {
	return client->available();
}

int HSSerial::WiFi_Read() {
	return client->read();
}

String HSSerial::WiFi_ReadStringUntil(char terminator) {
	return client->readStringUntil(terminator);
}

int HSSerial::WiFi_PrintChar(char c) {
	return client->print(c);
}

void HSSerial::AddKeyToOutputMessage(byte key)
{
	AddStringToOutputMessage(key, "");
}

void HSSerial::AddBoolToOutputMessage(byte key, bool value)
{
	value ? AddStringToOutputMessage(key, "1") : AddStringToOutputMessage(key, "0");
}

void HSSerial::AddStringToOutputMessage(byte key, char *value)
{
	int valueLength = strlen(value);
	int newLength = OutputDataLength + valueLength + 3;
	char tmp[newLength];

	if (OutputDataLength > 0) {
		strcpy(tmp, OutputData);
		delete[] OutputData;
	}

	tmp[OutputDataLength] = key;
	int y = OutputDataLength + 1;
	for (int x = 0; x < valueLength; x++) {
		tmp[y] = value[x];
		y++;
	}

	tmp[newLength - 2] = TRANS__KEY::PACKET_END;
	tmp[newLength - 1] = '\0';

	OutputData = new char[newLength];
	strcpy(OutputData, tmp);
	OutputDataLength = newLength - 1;
}

void HSSerial::HandleOutput() {
	if (!connectedToSomething) {
		return;
	}
	if (OutputDataLength == 0) {
		return;
	}

	(this->*PrintMessageToOutput)((char)TRANS__KEY::STX);
	for (int x = 0; x < OutputDataLength; x++) {
		(this->*PrintMessageToOutput)(OutputData[x]);
	}
	(this->*PrintMessageToOutput)((char)TRANS__KEY::ETX);
	delete[] OutputData;
	OutputDataLength = 0;

}


void HSSerial::HandleInput() {
	if (!connectedToSomething) {
		return;
	}
	if (!(this->*InputAvailable)())
		return;

	while ((this->*ReadInputByte)() != TRANS__KEY::STX) {}
	ParseInput((this->*ReadInputStringUntil)(TRANS__KEY::ETX));
}

void HSSerial::ParseInput(String input)
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

void HSSerial::DispatchCommand(char key, String val) {

	switch (key) {
	case TRANS__KEY::DISCONNECT:
		//Serial.println("TRANS__KEY::DISCONNECT");
		connectedToSomething = false;
		break;
	case TRANS__KEY::CONNECTION_REQUEST:
		//Serial.println("TRANS__KEY::CONNECTION_REQUEST");
		AcceptNewConnection();
		break;
	case TRANS__KEY::CONFIG_REQUEST_SSID:
		//Serial.println("TRANS__KEY::CONFIG_REQUEST_SSID");
		AddStringToOutputMessage(TRANS__KEY::CURRENT_SSID, "Starside");
		break;
	case TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET:
		//Serial.println("TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_CURRENT_PASSWORD_IS_SET, true);
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_HOSTNAME:
		//Serial.println("TRANS__KEY::CONFIG_REQUEST_SERVER_HOSTNAME");
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_HOSTNAME, "Aerie");
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_PORT:
		//Serial.println("TRANS__KEY::CONFIG_REQUEST_SERVER_PORT");
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_PORT, "4121");
		break;
	case TRANS__KEY::CONFIG_SET_SSID:
		//Serial.println("TRANS__KEY::CONFIG_SET_SSID");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SSID_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_PASSWORD:
		//Serial.println("TRANS__KEY::CONFIG_SET_PASSWORD");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_PASSWORD_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME:
		//Serial.println("TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS, true);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_PORT:
		//Serial.println("TRANS__KEY::CONFIG_SET_SERVER_PORT");
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_PORT_UPDATE_SUCCESS, true);
		break;
	default:
		//Serial.print("Unknown Command: '");
		//Serial.println(key, HEX);
		break;
	}
}