#include "HSFileSystem.h"

HSFileSystem::HSFileSystem()
{
}


HSFileSystem::~HSFileSystem()
{
	SPIFFS.end();
}

bool HSFileSystem::init()
{
	return SPIFFS.begin(false);
}

bool HSFileSystem::getSettings(S_SETTNGS& newSettings)
{
	File file = SPIFFS.open(SETTINGS_FILE_NAME);
	if (!file) {
		return false;
	}

	String line = "";
	while (file.available()) {
		line = file.readStringUntil('\n');
		if (line.length() > 0)
		{
			int currIndex = line.indexOf("=");
			if (currIndex != -1)
			{
				String key = line.substring(0, currIndex);
				String value = line.substring(currIndex + 1);
				if (value.length() == 0) {
					continue;
				}
				if (key.equals("ssid")) {
					value.toCharArray(newSettings.ssid, FIELD_MAX_LENGTH);
				} 
				else if (key.equals("password")) {
					value.toCharArray(newSettings.password, FIELD_MAX_LENGTH);
				}
				else if (key.equals("servername")) {
					value.toCharArray(newSettings.serverName, FIELD_MAX_LENGTH);
				}
				else if (key.equals("serverport")) {
					newSettings.serverPort = value.toInt();
				}
				else if (key.equals("wifiDID")) {
					value.toCharArray(newSettings.wifiDID, FIELD_MAX_LENGTH);
				}
				else if (key.equals("btDID")) {
					value.toCharArray(newSettings.btDID, FIELD_MAX_LENGTH);
				}
			}
		}
	}
	file.close();
	return true;
}

bool HSFileSystem::writeSettings(S_SETTNGS& newSettings)
{
	char buf[1024];
	sprintf(buf, "ssid=%s\npassword=%s\nservername=%s\nserverport=%i\nwifiDID=%s\nbtDID=%s\n", newSettings.ssid, newSettings.password, newSettings.serverName, newSettings.serverPort, newSettings.wifiDID, newSettings.btDID);
	File file = SPIFFS.open(SETTINGS_FILE_NAME, FILE_WRITE);
	if (!file) {
		return false;
	}

	if (file.print(buf)) 
	{
		file.close();
		return true;
	}

	file.close();
	return false;
}