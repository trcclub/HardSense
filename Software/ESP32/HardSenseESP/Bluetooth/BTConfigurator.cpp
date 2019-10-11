#include "BTConfigurator.h"
#include "../CommonSerialInterface/SerialInterface.h"


BTConfigurator::BTConfigurator(TFT_eSPI inDisplay)
{
	connected = false;
	tftDisplay = &inDisplay;
	btSerial.setTimeout(500);
	btSerial.begin("HardSenseESP");
}

void BTConfigurator::DrawBackground()
{

	tftDisplay->fillScreen(TFT_NAVY);
	tftDisplay->fillRect(6, 6, tftDisplay->width() - 12, tftDisplay->height() - 12, TFT_LIGHTGREY);
	tftDisplay->setTextColor(TFT_BLACK);
	tftDisplay->setTextSize(2);
	String str = "Bluetooth Configurator";
	tftDisplay->drawString(str, GetXForCenteredText(str), 20);

}

void BTConfigurator::HandleBluetooth()
{	
	while (true)
	{
		WaitForConnection();
		while (connected)
		{
			HandleInput();

			HandleOutput();

			delay(50);
		}
	}
}

bool BTConfigurator::WaitForConnection()
{
	DrawBackground();
	tftDisplay->setTextSize(1);
	String str = "Waiting for connection...";
	tftDisplay->drawString(str, GetXForCenteredText(str), 60);

	while (!connected)
	{
		HandleInput();
		if (connected)
		{
			break;
		}
			
		HandleOutput();
		delay(50);
	}
}

void BTConfigurator::AcceptNewConnection()
{
	addKeyToOutput(TRANS__KEY::CONNECTION_ACK);
	connected = true;
	DrawBackground();
	tftDisplay->setTextSize(1);
	String str = "Bluetooth conection established.";
	tftDisplay->drawString(str, GetXForCenteredText(str), 60);
}

inline int BTConfigurator::GetXForCenteredText(String text)
{
	return ((tftDisplay->width() - tftDisplay->textWidth(text)) / 2);
}

void BTConfigurator::HandleOutput()
{
	if (OutputDataLength == 0) {
		return;
	}
	
	Serial.print("Sending:  ");
	Serial.println(OutputData);

	btSerial.write(TRANS__KEY::SOT);
	for (int x = 0; x < OutputDataLength; x++) {
		btSerial.write(OutputData[x]);
	}
	btSerial.write(TRANS__KEY::ETX);

	delete[] OutputData;
	OutputDataLength = 0;
}

void BTConfigurator::HandleInput() {
	if (!btSerial.available())
		return;

	while (btSerial.read() != TRANS__KEY::SOT) {}
	
	String input = btSerial.readStringUntil(TRANS__KEY::ETX);
	Serial.print("INPUT: '");
	Serial.print(input);
	Serial.println("'");

	int currIndex = input.indexOf(TRANS__KEY::PACKET_END);
	int start = 0;
	while (currIndex != -1) {
		String currToken = input.substring(start,currIndex);
		
		char key = currToken.charAt(0);
		String value = "";
		value = currToken.substring(1);

		DispatchCommand(key, value);

		start = currIndex+1;
		currIndex = input.indexOf(TRANS__KEY::PACKET_END, start);
	}
}

//void BTConfigurator::DispatchCommand(wchar_t key, char* val) {
void BTConfigurator::DispatchCommand(char key, String val) {

	switch (key) {
	case TRANS__KEY::DISCONNECT:
		Serial.println("TRANS__KEY::DISCONNECT");
		connected = false;
		break;
	case TRANS__KEY::CONNECTION_REQUEST:
		Serial.println("TRANS__KEY::CONNECTION_REQUEST");
		AcceptNewConnection();
		break;
	case TRANS__KEY::CONFIG_REQUEST_SSID:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_SSID");
		addCharArrayToOutput(TRANS__KEY::CURRENT_SSID, "Starside", 8);
		break;
	case TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET:
		Serial.println("TRANS__KEY::CONFIG_REQUEST_IS_PASSWORD_SET");
		addBoolToOutput(TRANS__KEY::CONFIG_CURRENT_PASSWORD_IS_SET, true);
		break;
	default:
		Serial.print("Unknown Command: '");
		Serial.println(key, HEX);
		break;
	}
}

void BTConfigurator::addBoolToOutput(byte key, bool val) {
	
	if (val)
	{
		addCharArrayToOutput(key, "1", 1);
	}
	else {
		addCharArrayToOutput(key, "0", 1);
	}
	
}

void BTConfigurator::addKeyToOutput(byte key) {
	addCharArrayToOutput(key, NULL, 0);
}

void BTConfigurator::addCharArrayToOutput(byte key, const char* value, int valueLength) {
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