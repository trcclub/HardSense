#include "HSSerial.h"
#include "../DisplayHandler/DisplayHandler.h"
#include <RTClib.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

HSSerial::HSSerial()
{
	connectedToSomething = false;
	OTA_Enabled = false;
	OTA_Initialized = false;
	ConnectedToWifi = false;
	wifiSerial = NULL;
	btSerial = NULL;
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

bool HSSerial::Init(Queues *newQueues, void(*HeartbeatTimerEnabled_Func)(bool))
{
	HeartbeatTimerEnabled = HeartbeatTimerEnabled_Func;
	allQueues = newQueues;
	allQueues->outputQueueMux = portMUX_INITIALIZER_UNLOCKED;

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
	if (strcmp(hardsenseSettings.password,"") == 0)
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
	case TRANS__KEY::CONFIG_SET_WIFI_DEVICED_ID:
		value.toCharArray(hardsenseSettings.wifiDID, FIELD_MAX_LENGTH);
		break;
	case TRANS__KEY::CONFIG_SET_BT_DEVICE_ID:
		value.toCharArray(hardsenseSettings.btDID, FIELD_MAX_LENGTH);
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

	UpdateBluetoothDisplay();
	return true;
}

void HSSerial::UpdateBluetoothDisplay()
{
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, String("a," + String(hardsenseSettings.ssid)));

	String value = "b,";
	if(IsPasswordSet())
	{
		value += "******";
	} else {
		value += " ";
	}

	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, value);
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, String("c," + String(hardsenseSettings.serverName)));
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, String("d," + String(hardsenseSettings.serverPort)));
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, String("e," + String(hardsenseSettings.wifiDID)));
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, String("f," + String(hardsenseSettings.btDID)));
}


void HSSerial::Enable_OTA()
{
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue,String("a," + String(hardsenseSettings.ssid)));

	WiFi.mode(WIFI_STA);
	WiFi.setHostname(hardsenseSettings.wifiDID);
	WiFi.begin(hardsenseSettings.ssid, hardsenseSettings.password);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection Failed! ...");
		delay(5000);
	}

	if (!WiFi.isConnected()) {
		return;
	}

	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue,String("b," + String(hardsenseSettings.ssid)));
	//Serial.printf("\nEnable_OTA():  Connected to Wifi with hostname 1: %s\n", WiFi.getHostname());

	ArduinoOTA.setHostname("HardSenseESP");

	ArduinoOTA
	.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH)
		{
			type = "sketch";
		}
		else // U_SPIFFS
		{
			type = "filesystem";
		}

		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
		Serial.println("Start updating " + type);
		
	})

	.onEnd([]() {
		Serial.println("\nEnd");
	})

	.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
	})

	.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});

	ArduinoOTA.begin();

//	Serial.println("Ready");
//	Serial.print("IP address: ");
//	Serial.println(WiFi.localIP());
	OTA_Initialized = true;


}

void HSSerial::HandleConfigurator()
{
	while(true)
	{
		if(OTA_Enabled)
		{
			if(!OTA_Initialized)
			{
				Enable_OTA();
			}
			ArduinoOTA.handle();
			HandleCommandQueue();
		}
		else 
		{
			HandleBluetooth();
		}
		yield();
	}
}

void HSSerial::HandleBluetooth()
{	
	connectedToSomething = false;

	btSerial = new BluetoothSerial();
	InputAvailable = &HSSerial::BT_Available;
	ReadInputByte = &HSSerial::BT_Read;;
	ReadInputStringUntil = &HSSerial::BT_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::BT_PrintChar;

	btSerial->setTimeout(50);
	btSerial->begin(String(hardsenseSettings.btDID));

	while (!OTA_Enabled)
	{
		WaitForBTConnection();
		while (connectedToSomething && !OTA_Enabled)
		{
			WorkhorseFunction();
			//delay(20);
			yield();
		}
	}
	btSerial->end();
	btSerial->disconnect();
}

bool HSSerial::WaitForBTConnection()
{
	while (!connectedToSomething && !OTA_Enabled)
	{
		WorkhorseFunction();
		if (connectedToSomething)
		{
			break;
		}
		yield();
		//delay(20);
	}
}

void HSSerial::WorkhorseFunction()
{
	HandleInput();
	HandleOutput();
	HandleCommandQueue();
}

void HSSerial::HandleCommandQueue()
{
	while (!allQueues->commandQueue.isEmpty())
	{
		portENTER_CRITICAL(&allQueues->commandQueueMux);
		QUEUE_ITEM currItem = allQueues->commandQueue.dequeue();
		portEXIT_CRITICAL(&allQueues->commandQueueMux);

		switch (currItem.key) {
		case HardSense_Commands::BeginOTA:
			OTA_Enabled = true;
			break;
		case HardSense_Commands::RefreshBluetoothDisplay:
			UpdateBluetoothDisplay();
			break;
		default:
			break;
		}
		yield();
	}
}

void HSSerial::HandleWiFiConnection()
{
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue,String("a," + String(hardsenseSettings.ssid)));

	wifiSerial = new WiFiClient();
	InputAvailable = &HSSerial::WiFi_Available;
	ReadInputByte = &HSSerial::WiFi_Read;;
	ReadInputStringUntil = &HSSerial::WiFi_ReadStringUntil;
	PrintMessageToOutput = &HSSerial::WiFi_PrintChar;

//	Serial.print("\n Connecting to Wifi: ");
//	Serial.print(hardsenseSettings.ssid);
//	Serial.println("");

	WiFi.mode(WIFI_STA);
	WiFi.setHostname(hardsenseSettings.wifiDID);
	WiFi.begin(hardsenseSettings.ssid, hardsenseSettings.password);

	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection Failed! ...");
		delay(5000);
	}
	
	if (!WiFi.isConnected()) {
		return;
	}

	ConnectedToWifi = true;
	//Serial.printf("\nHostname 1: %s\n", WiFi.getHostname());
	//Serial.println("");
	//Serial.println("WiFi connected");
	//Serial.println("IP address: ");
	//Serial.println(WiFi.localIP());
}

void HSSerial::ConnectToHardsenseServer()
{
	if (!WiFi.isConnected()) {
		ConnectedToWifi = false;
		HandleWiFiConnection();
		return;
	}
	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue,String("b," + String(hardsenseSettings.ssid)));

	allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue,String("c," + String(hardsenseSettings.serverName) + ":" + String(hardsenseSettings.serverPort)));

	//Serial.print("\n Connecting to socket on ");
	//Serial.print(hardsenseSettings.serverName);
	//Serial.print(":");
	//Serial.println(hardsenseSettings.serverPort);

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
	connectedToSomething = true;
	//Serial.print("Connected to: ");
	//Serial.println(hardsenseSettings.serverName);

	AddKeyToOutputMessage(TRANS__KEY::START_SENSOR_DATA_STREAM);
	HandleOutput();

	allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::Home));

	HeartbeatTimerEnabled(true);
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

void HSSerial::AddKeyToOutputMessage(TRANS__KEY key)
{
	AddStringToOutputMessage(key, "");
}

void HSSerial::AddIntToOutputMessage(TRANS__KEY key, int val)
{
	char newVal[MAX_BUF_SIZE];
	sprintf(newVal, "%i", hardsenseSettings.serverPort);
	AddStringToOutputMessage(key, newVal);
}

void HSSerial::AddBoolToOutputMessage(TRANS__KEY key, bool value)
{
	value ? AddStringToOutputMessage(key, "true") : AddStringToOutputMessage(key, "false");
}

void HSSerial::AddStringToOutputMessage(TRANS__KEY key, String value)
{
	char buf[MAX_BUF_SIZE];
	value.toCharArray(buf, MAX_BUF_SIZE);
	AddStringToOutputMessage(key, buf);
}

void HSSerial::AddStringToOutputMessage(TRANS__KEY key, char *value)
{
	int valueLength = strlen(value);
	int newLength = OutputDataLength + valueLength + 3;

	if (newLength > MAX_BUF_SIZE) {
		HandleOutput();
		valueLength = strlen(value);
		newLength = OutputDataLength + valueLength + 3;
	}
	portENTER_CRITICAL(&allQueues->outputQueueMux);

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

	portEXIT_CRITICAL(&allQueues->outputQueueMux);
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

	portENTER_CRITICAL(&allQueues->outputQueueMux);
	
	int tmpOutputDataLength = OutputDataLength;
	char tmpOutputData[tmpOutputDataLength];
	strcpy(tmpOutputData, OutputData);
	delete[] OutputData;
	OutputDataLength = 0;

	portEXIT_CRITICAL(&allQueues->outputQueueMux);

	(this->*PrintMessageToOutput)((char)TRANS__KEY::STX);
	for (int x = 0; x < tmpOutputDataLength; x++) {
		(this->*PrintMessageToOutput)(tmpOutputData[x]);
	}
	(this->*PrintMessageToOutput)((char)TRANS__KEY::ETX);
}


void HSSerial::HandleInput() 
{
	if (!(this->*InputAvailable)())
		return;

	while ((this->*InputAvailable)() && (this->*ReadInputByte)() != TRANS__KEY::STX) {  }

	yield();
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
		yield(); // Avoid a watchdog time-out
	}
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
	case TRANS__KEY::CONFIG_REQUEST_WIFI_DEVICE_ID:
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_WIFI_DEVICE_ID, hardsenseSettings.wifiDID);
		break;
	case TRANS__KEY::CONFIG_REQUEST_BT_DEVICE_ID:
		AddStringToOutputMessage(TRANS__KEY::CONFIG_CURRENT_BT_DEVICE_ID, hardsenseSettings.btDID);
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
	case TRANS__KEY::CONFIG_SET_WIFI_DEVICED_ID:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_WIFI_DEVICE_ID_UPDATE_SUCCESS, UpdateSetting(key, val));
		break;
	case TRANS__KEY::CONFIG_SET_BT_DEVICE_ID:
		AddBoolToOutputMessage(TRANS__KEY::CONFIG_BT_DEVICE_ID_UPDATE_SUCCESS, UpdateSetting(key, val));
		break;
	case TRANS__KEY::UPDATE_SENSOR_VALUE:
		allQueues->AddItemToDisplayQueue(DisplayCommands::UpdateValue, val);
		break;
	case TRANS__KEY::HEARTBEAT:
		AddKeyToOutputMessage(TRANS__KEY::HEARTBEAT_ACK);
		break;
	case TRANS__KEY::HEARTBEAT_ACK:
		heartbeatCounter = 0;
		break;
	case TRANS__KEY::CONFIG_DELETE_TOUCH_CALIBRATION_FILE:
		SPIFFS.remove(CALIBRATION_FILE);
		break;
	case TRANS__KEY::CONFIG_UPDATE_TIME:
		Update_RTC_Time(val);
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
}

bool HSSerial::IncrementHeartbeatCounter()
{
	if (heartbeatCounter > MAX_HEARTBEATS_MISSED) {
		connectedToSomething = false;
		heartbeatCounter = 0;
		HeartbeatTimerEnabled(false);
		allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::SplashScreen));
		delay(50);
		return false;
	}
	heartbeatCounter++;
	return true;
}

void HSSerial::Update_RTC_Time(String rawTime)
{
	RTC_DS1307 rtc;
	rtc.begin();

	char date[12];
	char time[9];
	int sep = rawTime.indexOf(",");
	
	rawTime.substring(0, sep).toCharArray(date, 12);
	rawTime.substring(sep + 1).toCharArray(time, 9);

	rtc.adjust(DateTime(date,time));
	
	DateTime now = rtc.now();
}