#include "HS_BluetoothConfiguratorScreen.h"

HS_BluetoothConfiguratorScreen::HS_BluetoothConfiguratorScreen(Queues *newQueues, TFT_eSPI* newTFT) : HS_ScreenBase(newQueues, newTFT)
{
	bluetoothConfiguratorScreenTheme.panelBGColor = PANEL_BGCOLOR;
	bluetoothConfiguratorScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	bluetoothConfiguratorScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	bluetoothConfiguratorScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	bluetoothConfiguratorScreenTheme.textColor = TEXT_COLOR;

	rtc.begin();
	
	TFT->unloadFont();
//	TFT->loadFont(AA_FONT_18PT);

	TFT->setTextSize(1);

	textPrinter_Sprite->unloadFont();
//	textPrinter_Sprite->loadFont(AA_FONT_18PT);
//	textPrinter_Sprite->setTextSize(1);

	TFT->fillScreen(TFT_BLACK);
	
	Draw_Network_Panel();
	Draw_Server_Panel();
	Draw_DeviceID_Panel();

	Draw_OTA_Panel();
	Draw_Delete_Touch_Calib_Panel();

	Draw_Time_Panel();
	Update_Time();

//	Serial.println("This is code added by OTA update.");

	allQueues->AddItemToCommandQueue(HardSense_Commands::RefreshBluetoothDisplay,"");
}

HS_BluetoothConfiguratorScreen::~HS_BluetoothConfiguratorScreen()
{
}

// a = SSID
// b = Password
// c = Server Name
// d = Server Port
// e = WiFI Device ID
// f = BT Device ID
void HS_BluetoothConfiguratorScreen::UpdateScreen(String value)
{
	String strValue(value);
	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);
	int intValue = subValue.toInt();

	switch (key) {
	case 'a':
		UpdateSSID(subValue);
		break;
	case 'b':
		UpdatePassword(subValue);
		break;
	case 'c':
		Update_Server_Name(subValue);
		break;
	case 'd':
		Update_Server_Port(intValue);
		break;
	case 'e':
		Update_WiFi_DeviceID(subValue);
		break;
	case 'f':
		Update_BT_DeviceID(subValue);
		break;
	default:
		break;
	}
}

void HS_BluetoothConfiguratorScreen::UpdateScreenOnInterval()
{
	if (millis() - lastTimeUpate > updateTimeInterval)
	{
		lastTimeUpate = millis();
		Update_Time();
	}
}

void HS_BluetoothConfiguratorScreen::HandleTouch(int x, int y)
{
	if (OTA_Panel_Touched(x, y))
	{
		allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::OTA));
	}
	if (Delete_Touch_Calib_Panel_Touched(x, y))
	{
		//allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::BluetoothConfigurator));
		//allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::OTA));
		allQueues->AddItemToCommandQueue(HardSense_Commands::DeleteTouchcalibration,"");
	}
}

void HS_BluetoothConfiguratorScreen::Draw_Time_Panel()
{
	DrawBoxWithBorderAndDropShadow(HS_Coords(TIME_PANEL_X, TIME_PANEL_Y, 113, 52), bluetoothConfiguratorScreenTheme);
	Update_Time();
}

void HS_BluetoothConfiguratorScreen::Update_Time()
{
	int x = TIME_PANEL_X;
	int y = TIME_PANEL_Y;

	DateTime now = rtc.now();
	int d = now.day();
	String date(daysOfTheWeek[now.dayOfTheWeek()]);
	date += ", " + String(monthsOfTheYear[now.month()-1]);
	date += " " + String(d);


	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->createSprite(107,46);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);

	textPrinter_Sprite->drawString(date,53,4,2);

	int hour = now.hour();
	int minutes = now.minute();
	String min = "";
	if (minutes < 10)
	{
		min = "0";
	}
	min += String(minutes);

	int seconds = now.second();
	String sec = "";
	if (seconds < 10)
	{
		sec = "0";
	}
	sec += String(seconds);

	if (hour > 12)
	{
		date = String(hour - 12) + ":" + min + " " + sec + " PM";
	}
	else
	{

		date = String(hour) + ":" + min + ":" + sec + " AM";
	}
	textPrinter_Sprite->drawString(date, 53, 26,2);
	
	textPrinter_Sprite->pushSprite(x + 3, y + 3);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->deleteSprite();
}

void HS_BluetoothConfiguratorScreen::Draw_Network_Panel()
{
	HS_Coords localCoords(NETWORK_PANEL_X, NETWORK_PANEL_Y, 207, 63);

	DrawBoxWithBorderAndDropShadow(localCoords, bluetoothConfiguratorScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, bluetoothConfiguratorScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, bluetoothConfiguratorScreenTheme.panelHeaderColor);


	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(80, 15);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Network", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 80, localCoords.y + 4);


	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("SSID", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 24);

	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("Pass", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 44);

	textPrinter_Sprite->deleteSprite();

	
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 42, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 43, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
}

void HS_BluetoothConfiguratorScreen::UpdateSSID(String ssid)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(ssid, 0, 0,2);
	textPrinter_Sprite->pushSprite(NETWORK_PANEL_X + 55, NETWORK_PANEL_Y + 24);

	textPrinter_Sprite->deleteSprite();
}

void HS_BluetoothConfiguratorScreen::UpdatePassword(String password)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(password, 0, 0,2);
	textPrinter_Sprite->pushSprite(NETWORK_PANEL_X + 55, NETWORK_PANEL_Y + 44);

	textPrinter_Sprite->deleteSprite();
}

void HS_BluetoothConfiguratorScreen::Draw_Server_Panel()
{
	HS_Coords localCoords(SERVER_PANEL_X, SERVER_PANEL_Y, 207, 63);

	DrawBoxWithBorderAndDropShadow(localCoords, bluetoothConfiguratorScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, bluetoothConfiguratorScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, bluetoothConfiguratorScreenTheme.panelHeaderColor);


	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(80, 15);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Server", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 80, localCoords.y + 4);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("Name", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 24);

	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("Port", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 44);

	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 42, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 43, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);

}

void HS_BluetoothConfiguratorScreen::Update_Server_Name(String serverName)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(serverName, 0, 0,2);
	textPrinter_Sprite->pushSprite(SERVER_PANEL_X + 65, SERVER_PANEL_Y + 24);

	textPrinter_Sprite->deleteSprite();

}

void HS_BluetoothConfiguratorScreen::Update_Server_Port(int port)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawNumber(port, 0, 0,2);
	textPrinter_Sprite->pushSprite(SERVER_PANEL_X + 55, SERVER_PANEL_Y + 44);

	textPrinter_Sprite->deleteSprite();
}


void HS_BluetoothConfiguratorScreen::Draw_DeviceID_Panel()
{
	HS_Coords localCoords(DEVICEID_PANEL_X, DEVICEID_PANEL_Y, 207, 63);

	DrawBoxWithBorderAndDropShadow(localCoords, bluetoothConfiguratorScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, bluetoothConfiguratorScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, bluetoothConfiguratorScreenTheme.panelHeaderColor);


	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(95, 15);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Device IDs", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 70, localCoords.y + 4);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("WiFi", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 24);

	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("BT", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 6, localCoords.y + 44);

	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 42, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 43, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);

}

void HS_BluetoothConfiguratorScreen::Update_WiFi_DeviceID(String wifiDID)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(wifiDID, 0, 0,2);
	textPrinter_Sprite->pushSprite(DEVICEID_PANEL_X + 55, DEVICEID_PANEL_Y + 24);

	textPrinter_Sprite->deleteSprite();

}

void HS_BluetoothConfiguratorScreen::Update_BT_DeviceID(String btDID)
{
	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->loadFont(AA_FONT_18PT);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(130, 15);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(btDID, 0, 0, 2);
	textPrinter_Sprite->pushSprite(DEVICEID_PANEL_X + 55, DEVICEID_PANEL_Y + 44);

	textPrinter_Sprite->deleteSprite();
}

void HS_BluetoothConfiguratorScreen::Draw_OTA_Panel()
{
	HS_Coords localCoords(OTA_PANEL_X, OTA_PANEL_Y, 113, 127);

	DrawBoxWithBorderAndDropShadow(localCoords, bluetoothConfiguratorScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, bluetoothConfiguratorScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, bluetoothConfiguratorScreenTheme.panelHeaderColor);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(95, 15);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("OTA", 0,0,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 45, localCoords.y + 4);

	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, bluetoothConfiguratorScreenTheme.panelBorderColor);
}

bool HS_BluetoothConfiguratorScreen::OTA_Panel_Touched(int x, int y)
{
	if ((x >= OTA_PANEL_LOW_X + 5 && x <= OTA_PANEL_HIGH_X - 5) && (y >= OTA_PANEL_LOW_Y + 5 && y <= OTA_PANEL_HIGH_Y - 5))
	{
		return true;
	}
	return false;
}

bool HS_BluetoothConfiguratorScreen::Delete_Touch_Calib_Panel_Touched(int x, int y)
{
	if ((x >= DELETE_CALIB_PANEL_LOW_X + 5 && x <= DELETE_CALIB_PANEL_HIGH_X - 5) && (y >= DELETE_CALIB_PANEL_LOW_Y + 5 && y <= DELETE_CALIB_PANEL_HIGH_Y - 5))
	{
		return true;
	}
	return false;
}

void HS_BluetoothConfiguratorScreen::Draw_Delete_Touch_Calib_Panel()
{
	HS_Coords localCoords(DELETE_CALIB_PANEL_X, DELETE_CALIB_PANEL_Y, 113, 60);

	DrawBoxWithBorderAndDropShadow(localCoords, bluetoothConfiguratorScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	//TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, bluetoothConfiguratorScreenTheme.panelHeaderColor);

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(90, 52);

	textPrinter_Sprite->setTextColor(bluetoothConfiguratorScreenTheme.textColor, bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(bluetoothConfiguratorScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Delete", 10,0,2);
	textPrinter_Sprite->drawString("Touch", 10,18,2);
	textPrinter_Sprite->drawString("Calibation", 0,36,2);
	textPrinter_Sprite->pushSprite(localCoords.x + 20, localCoords.y + 4);

	textPrinter_Sprite->deleteSprite();
}