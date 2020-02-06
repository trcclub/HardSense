#pragma once

#include "FS.h"
#include "SPIFFS.h"

#define SETTINGS_FILE_NAME "/hardsens.txt"
#define FIELD_MAX_LENGTH 64

struct S_SETTNGS {
	char ssid[FIELD_MAX_LENGTH];
	char password[FIELD_MAX_LENGTH];
	char serverName[FIELD_MAX_LENGTH];
	int serverPort;
	S_SETTNGS() {
		strcpy(ssid, "");
		strcpy(password, "");
		strcpy(serverName, "");
		serverPort = 0;
	}
};

class HSFileSystem
{
private:

public:
	HSFileSystem();
	~HSFileSystem();
	static bool init();
	static bool getSettings(S_SETTNGS& newSettings);
	static bool writeSettings(S_SETTNGS& newSettings);
};

