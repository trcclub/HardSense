#include "HSSerial.h"
#include "../DisplayHandler/DisplayHandler.h"

HSSerial::HSSerial()
{
	connectedToSomething = false;
	ConnectedToWifi = false;
	wifiSerial = NULL;
	btSerial = NULL;
	outputDataMux = portMUX_INITIALIZER_UNLOCKED;
	heartbeatCounter = 0;
	heartbeatMux = portMUX_INITIALIZER_UNLOCKED;
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

bool HSSerial::Init(DataQueue<QUEUE_ITEM>* newOutputQueue, portMUX_TYPE& newOutputQueueMux, void(*AddItemToDisplayQueue_Func)(char key, String value), void(*HeartbeatTimerEnabled_Func)(bool))
{
	outputDataQueue = newOutputQueue;
	AddItemToDisplayQueue = AddItemToDisplayQueue_Func;
	outputQueueMux = newOutputQueueMux;
	HeartbeatTimerEnabled = HeartbeatTimerEnabled_Func;

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
	char buf[2];
	sprintf(buf, "%c", ScreenTypes::BluetoothConfigurator);
	AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	connectedToSomething = false;
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

			yield();
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
		yield();
		delay(20);
	}
}

void HSSerial::HandleWiFiConnection()
{
	char buf[2];
	sprintf(buf, "%c", ScreenTypes::ConnectToNetwork);
	AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);

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
	
	if (!WiFi.isConnected()) {
		return;
	}
	ConnectedToWifi = true;
	Serial.printf("Hostname 1: %s\n", WiFi.getHostname());
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

}

void HSSerial::ConnectToHardsenseServer()
{
	if (!WiFi.isConnected()) {
		ConnectedToWifi = false;
		HandleWiFiConnection();
		return;
	}

	char buf[2];
	sprintf(buf, "%c", ScreenTypes::ConnectToNetwork);
	AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);

	Serial.print("\n Connecting to socket on ");
	Serial.print(hardsenseSettings.serverName);
	Serial.print(":");
	Serial.println(hardsenseSettings.serverPort);
	connectedToSomething = false;
	wifiSerial->connect(hardsenseSettings.serverName, hardsenseSettings.serverPort);
	AddKeyToOutputMessage(TRANS__KEY::REQUEST_NEW_CONNECTION);
	HandleOutput();

	unsigned long now = millis();
	while(!connectedToSomething && (millis() - now) < 5000) 
	{
		HandleInput();
		delay(50);
	}
}

void HSSerial::NewSocketRequestAccepted()
{
	Serial.println("NewSocketRequestAccepted()");
	connectedToSomething = true;
	Serial.print("Connected to: ");
	Serial.println(hardsenseSettings.serverName);

	AddKeyToOutputMessage(TRANS__KEY::START_SENSOR_DATA_STREAM);
	HandleOutput();

	char buf[2];
	sprintf(buf, "%c", ScreenTypes::Home);
	AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);

	//
	// ** RE-ENABLE THE HEARTBEAT
	//

	HeartbeatTimerEnabled(true);

	//
	// ** RE-ENABLE THE HEARTBEAT
	//
}

void HSSerial::AcceptNewBTConnection()
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
	char newVal[MAX_BUF_SIZE];
	sprintf(newVal, "%i", hardsenseSettings.serverPort);
	AddStringToOutputMessage(key, newVal);
}

void HSSerial::AddBoolToOutputMessage(byte key, bool value)
{
	value ? AddStringToOutputMessage(key, "true") : AddStringToOutputMessage(key, "false");
}

void HSSerial::AddStringToOutputMessage(byte key, String value)
{
	char buf[MAX_BUF_SIZE];
	value.toCharArray(buf, MAX_BUF_SIZE);
	AddStringToOutputMessage(key, buf);
}

void HSSerial::AddStringToOutputMessage(byte key, char *value)
{
	int valueLength = strlen(value);
	int newLength = OutputDataLength + valueLength + 3;

	if (newLength > MAX_BUF_SIZE) {
		HandleOutput();
		valueLength = strlen(value);
		newLength = OutputDataLength + valueLength + 3;
	}
	portENTER_CRITICAL(&outputDataMux);

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

	portEXIT_CRITICAL(&outputDataMux);
}

void HSSerial::HandleOutput() 
{
	if (AddHeartbeatToOutput)
	{
		portENTER_CRITICAL(&heartbeatMux);
		AddHeartbeatToOutput = false;
		portEXIT_CRITICAL(&heartbeatMux);
		if (connectedToSomething && IncrementHeartbeatCounter())
		{
			AddKeyToOutputMessage(TRANS__KEY::HEARTBEAT);
		}
	}

	if (OutputDataLength == 0) {
		return;
	}

	portENTER_CRITICAL(&outputDataMux);
	
	int tmpOutputDataLength = OutputDataLength;
	char tmpOutputData[tmpOutputDataLength];
	strcpy(tmpOutputData, OutputData);
	delete[] OutputData;
	OutputDataLength = 0;

	portEXIT_CRITICAL(&outputDataMux);

	//Serial.print("OUTPUT: ");
	//Serial.println(tmpOutputData);

	(this->*PrintMessageToOutput)((char)TRANS__KEY::STX);
	for (int x = 0; x < tmpOutputDataLength; x++) {
		(this->*PrintMessageToOutput)(tmpOutputData[x]);
	}
	(this->*PrintMessageToOutput)((char)TRANS__KEY::ETX);

	
}


void HSSerial::HandleInput() {
	while (!outputDataQueue->isEmpty())
	{
		portENTER_CRITICAL(&outputQueueMux);
		QUEUE_ITEM currItem = outputDataQueue->dequeue();
		portEXIT_CRITICAL(&outputQueueMux);

		AddStringToOutputMessage(currItem.key, currItem.value);
	}

	if (!(this->*InputAvailable)())
		return;

	//Serial.println("HandleInput 1");
	while ((this->*InputAvailable)() && (this->*ReadInputByte)() != TRANS__KEY::STX) {}

	ParseInput((this->*ReadInputStringUntil)(TRANS__KEY::ETX));
	//Serial.println("HandleInput 2");
}

void HSSerial::ParseInput(String input)
{
	//Serial.print("ParseInput 1 ");
	//Serial.println(input);
	int currIndex = input.indexOf(TRANS__KEY::PACKET_END);
	int start = 0;
	while (currIndex != -1) {
		String currToken = input.substring(start, currIndex);

		char key = currToken.charAt(0);
		String value = currToken.substring(1);
		DispatchCommand(key, value);

		start = currIndex + 1;
		currIndex = input.indexOf(TRANS__KEY::PACKET_END, start);
		//yield(); // Avoid a watchdog time-out
	}
	//Serial.println("ParseInput 2");
}

void HSSerial::DispatchCommand(char key, String val) {

	switch (key) {
	case TRANS__KEY::NEW_CONNECTION_APPROVED:
		NewSocketRequestAccepted();
		break;
	case TRANS__KEY::DISCONNECT:
		connectedToSomething = false;
		HeartbeatTimerEnabled(false);
		break;
	case TRANS__KEY::CONNECTION_REQUEST:
		AcceptNewBTConnection();
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
	case TRANS__KEY::UPDATE_SENSOR_VALUE:
		AddItemToDisplayQueue(DisplayCommands::UpdateValue, val);
		//UpdateSensorValuesToDisplay(val);
		break;
	case TRANS__KEY::HEARTBEAT:
		AddKeyToOutputMessage(TRANS__KEY::HEARTBEAT_ACK);
		break;
	case TRANS__KEY::HEARTBEAT_ACK:
		heartbeatCounter = 0;
		break;
	default:
		//Serial.print("Unknown Command: '");
		//Serial.println(key, HEX);
		break;
	}
}

void IRAM_ATTR HSSerial::FireHeartbeat()
{
	portENTER_CRITICAL(&heartbeatMux);
	AddHeartbeatToOutput = true;
	portEXIT_CRITICAL(&heartbeatMux);

	/*
	if (!connectedToSomething) {
		return;
	}

	if (IncrementHeartbeatCounter())
	{
		AddKeyToOutputMessage(TRANS__KEY::HEARTBEAT);
	}
	*/
}

bool HSSerial::IncrementHeartbeatCounter()
{
	if (heartbeatCounter > MAX_HEARTBEATS_MISSED) {
		connectedToSomething = false;
		heartbeatCounter = 0;
		HeartbeatTimerEnabled(false);
		return false;
	}
	heartbeatCounter++;
	return true;
}

/*
void HSSerial::UpdateSensorValuesToDisplay(String value)
{
	// value format:
	// <key>,<double value as string>
	//
	//Serial.print("UpdateSensorValuesToDisplay: ");
	//Serial.println(value);

	//char buf[MAX_QUEUE_ITEM_VALUE_SIZE];
	//value.toCharArray(buf, MAX_QUEUE_ITEM_VALUE_SIZE);
	//AddItemToDisplayQueue(DisplayCommands::UpdateValue, buf);
	AddItemToDisplayQueue(DisplayCommands::UpdateValue, value);
}
*/