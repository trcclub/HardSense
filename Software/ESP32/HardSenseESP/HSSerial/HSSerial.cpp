#include "HSSerial.h"

HSSerial::HSSerial()
{
	connectedToSomething = false;
	wifiSerial = NULL;
	btSerial = NULL;
}

HSSerial::~HSSerial()
{
	if (wifiSerial != NULL)
	{
		delete(wifiSerial);
	}
	if (btSerial != NULL)
	{
		delete(btSerial);
	}
}

bool HSSerial::init(DataQueue<QUEUE_ITEM>* newQueue)
{
	outputDataQueue = newQueue;
	HSFileSystem hsFS;
	if (!hsFS.init()) {
		Serial.println("Failed to init SPIFFS");
		return false;
	}
	
	if (!hsFS.getSettings(hardsenseSettings))
	{
		Serial.println("Failed to retrieve settings from SPIFFS!");
		return false;
	}
	
	return true;
}

bool HSSerial::IsPasswordSet()
{
	if (strlen(hardsenseSettings.password) == 0)
	{
		return false;
	}
	return true;
}

bool HSSerial::UpdateSetting(char key, String value)
{
	switch (key) {
	case TRANS__KEY::CONFIG_SET_SSID:
		value.toCharArray(hardsenseSettings.ssid, FIELD_MAX_LENGTH);
		break;
	case TRANS__KEY::CONFIG_SET_PASSWORD:
		value.toCharArray(hardsenseSettings.password, FIELD_MAX_LENGTH);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME:
		value.toCharArray(hardsenseSettings.serverName, FIELD_MAX_LENGTH);
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_PORT:
		hardsenseSettings.serverPort = value.toInt();
		break;
	default:
		return false;
	}
	return SaveSettingsToFS();
}

bool HSSerial::SaveSettingsToFS()
{
	HSFileSystem hsFS;
	if (!hsFS.init()) {
		Serial.println("Failed to init SPIFFS");
		return false;
	}

	if (!hsFS.writeSettings(hardsenseSettings))
	{
		Serial.println("Failed to write settings to SPIFFS!");
		return false;
	}

	return true;
}

void HSSerial::HandleBluetoothConnection()
{
	btSerial = new BluetoothSerial();
	InputAvailable = &HSSerial::BT_Available;
	ReadInputByte = &HSSerial::BT_Read;;
	ReadInputStringUntil = &HSSerial::BT_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::BT_PrintChar;

	btSerial->setTimeout(50);
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
	wifiSerial = new WiFiClient();
	InputAvailable = &HSSerial::WiFi_Available;
	ReadInputByte = &HSSerial::WiFi_Read;;
	ReadInputStringUntil = &HSSerial::WiFi_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::WiFi_PrintChar;

	Serial.print("\n Connecting to Wifi: ");
	Serial.print(hardsenseSettings.ssid);
	//Serial.print(":");
	//Serial.print(hardsenseSettings.password);
	Serial.println("");

	WiFi.begin();
	WiFi.setHostname("HardSenseESP");
	WiFi.begin(hardsenseSettings.ssid, hardsenseSettings.password);
	

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.printf("Hostname 1: %s\n", WiFi.getHostname());
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.print("\n Connecting to socket on ");
	Serial.print(hardsenseSettings.serverName);
	Serial.print(":");
	Serial.println(hardsenseSettings.serverPort);

	if (!wifiSerial->connect(hardsenseSettings.serverName, hardsenseSettings.serverPort)) {
		Serial.println("connection failed");
		//Spin();
	}
	connectedToSomething = true;
	Serial.print("Connected to: ");
	Serial.println(hardsenseSettings.serverName);

	char buffer[128];
	int size = sprintf(buffer, "/Ethernet/0/recv,a|/intelcpu/0/load/0,b");
	//AddStringToOutputMessage(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, buffer);
	//AddKeyToOutputMessage(TRANS__KEY::START_SENSOR_DATA_STREAM);
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
	return wifiSerial->available();
}

int HSSerial::WiFi_Read() {
	return wifiSerial->read();
}

String HSSerial::WiFi_ReadStringUntil(char terminator) {
	return wifiSerial->readStringUntil(terminator);
}

int HSSerial::WiFi_PrintChar(char c) {
	return wifiSerial->print(c);
}

void HSSerial::AddKeyToOutputMessage(byte key)
{
	AddStringToOutputMessage(key, "");
}

void HSSerial::AddIntToOutputMessage(byte key, int val)
{
	char newVal[FIELD_MAX_LENGTH];
	sprintf(newVal, "%i", hardsenseSettings.serverPort);
	AddStringToOutputMessage(key, newVal);
}

void HSSerial::AddBoolToOutputMessage(byte key, bool value)
{
	value ? AddStringToOutputMessage(key, "true") : AddStringToOutputMessage(key, "false");
}

void HSSerial::AddStringToOutputMessage(byte key, String value)
{
	char buf[128];
	value.toCharArray(buf, 128);
	AddStringToOutputMessage(key, buf);
}

void HSSerial::AddStringToOutputMessage(byte key, char *value)
{
	return;

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
	while (!outputDataQueue->isEmpty())
	{
		QUEUE_ITEM currItem = outputDataQueue->dequeue();
		AddStringToOutputMessage(currItem.key, currItem.value);
	}


	if (!(this->*InputAvailable)())
		return;

	while ((this->*InputAvailable)() && (this->*ReadInputByte)() != TRANS__KEY::STX) {}

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
		connectedToSomething = false;
		break;
	case TRANS__KEY::CONNECTION_REQUEST:
		AcceptNewConnection();
		break;
	case TRANS__KEY::CONFIG_REQUEST_SSID:
		AddStringToOutputMessage(TRANS__KEY::CURRENT_SSID, hardsenseSettings.ssid);
		break;
	case TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_CURRENT_PASSWORD_IS_SET, IsPasswordSet());
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_HOSTNAME:
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_HOSTNAME, hardsenseSettings.serverName);
		break;
	case TRANS__KEY::CONFIG_REQUEST_SERVER_PORT:
		AddIntToOutputMessage(TRANS__KEY::CONFIG_CURRENT_SERVER_PORT, hardsenseSettings.serverPort);
		break;
	case TRANS__KEY::CONFIG_SET_SSID:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SSID_UPDATE_SUCCESS, UpdateSetting(key,val));
		break;
	case TRANS__KEY::CONFIG_SET_PASSWORD:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_PASSWORD_UPDATE_SUCCESS, UpdateSetting(key, val));
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_HOSTNAME:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_HOSTNAME_UPDATE_SUCCESS, UpdateSetting(key, val));
		break;
	case TRANS__KEY::CONFIG_SET_SERVER_PORT:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_SERVER_PORT_UPDATE_SUCCESS, UpdateSetting(key, val));
		break;
	case TRANS__KEY::HEARTBEAT:
		AddKeyToOutputMessage(TRANS__KEY::HEARTBEAT_ACK);
		break;
	default:
		//Serial.print("Unknown Command: '");
		//Serial.println(key, HEX);
		break;
	}
}