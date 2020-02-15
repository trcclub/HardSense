#include "HS_SplashScreen.h"

HS_SplashScreen::HS_SplashScreen(Queues *newQueues, TFT_eSPI* newTFT) : HS_ScreenBase(newQueues, newTFT)
{
	splashScreenTheme.panelBGColor = PANEL_BGCOLOR;
	splashScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	splashScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	splashScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	splashScreenTheme.textColor = TEXT_COLOR;

	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_18PT);
	TFT->unloadFont();
	TFT->loadFont(AA_FONT_18PT);

	col[0] = 0xED7F;
	col[1] = 0xD4DB;
	col[2] = 0xBC38;
	col[3] = 0xABD6;
	col[4] = 0x9374;
	col[5] = 0x8332;
	col[6] = 0x72AF;
	col[7] = 0x624D;

	circleLoopCounter = 8;
	secondaryLoopCounter = 0;

	TFT->fillScreen(splashScreenTheme.panelBGColor);
	Draw_NetworkConnectionPanel();
	Draw_ServerConnectionPanel();

}

HS_SplashScreen::~HS_SplashScreen()
{

}


// a = Connecting to network flag and network name
// b = Network connection success
// c = Conecting to HardSense64 Server and full name:port
void HS_SplashScreen::UpdateScreen(String value)
{
	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);

	switch (key) {
	case 'a':
		Update_ConnectingToNetwork(subValue);
		break;
	case 'b':
		Update_ConnectedToNetwork(subValue);
		break;
	case 'c':
		Update_ConnectingToServer(subValue);
		break;
	default:
		break;
	}

}

void HS_SplashScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		lastUpdate = millis();
		UpdateCircle();
	}
}

void HS_SplashScreen::UpdateCircle()
{
	TFT->fillCircle(240 + 40 * (cos(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), 120 + 40 * (sin(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), 10, col[secondaryLoopCounter]); 
	
	secondaryLoopCounter++;
	if(secondaryLoopCounter > 7)
	{
		secondaryLoopCounter = 0;
		circleLoopCounter--;
	}
	
	if(circleLoopCounter <= 0)
	{
		circleLoopCounter == 8;
	}
}


void HS_SplashScreen::Update_ConnectingToNetwork(String netID)
{
	Update_NetworkConnectionInfo(netID,TFT_RED);
}

void HS_SplashScreen::Update_ConnectedToNetwork(String netID)
{	
	Update_NetworkConnectionInfo(netID,TFT_GREEN);
}

void HS_SplashScreen::Draw_NetworkConnectionPanel()
{
	HS_Coords localCoords(NET_PANEL_X, NET_PANEL_Y, 150, 45);

	DrawBoxWithBorderAndDropShadow(localCoords, splashScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, splashScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, splashScreenTheme.panelHeaderColor);

	TFT->setTextColor(splashScreenTheme.textColor, splashScreenTheme.panelHeaderColor);
	TFT->drawString("Network",localCoords.x + 40, localCoords.y + 4);	

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, splashScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, splashScreenTheme.panelBorderColor);

}

void HS_SplashScreen::Update_NetworkConnectionInfo(String netID, uint16_t circleColor)
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(splashScreenTheme.textColor, splashScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(80, 15);
	textPrinter_Sprite->fillSprite(splashScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(netID, 0, 0);
	textPrinter_Sprite->pushSprite(NET_PANEL_X + 26, NET_PANEL_Y + 25);

	textPrinter_Sprite->deleteSprite();

	TFT->fillCircle(NET_PANEL_X + 14, NET_PANEL_Y + 31, 7,circleColor);

}

void HS_SplashScreen::Draw_ServerConnectionPanel()
{
	HS_Coords localCoords(SERVER_PANEL_X, SERVER_PANEL_Y, 150, 45);

	DrawBoxWithBorderAndDropShadow(localCoords, splashScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, splashScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, splashScreenTheme.panelHeaderColor);

	TFT->setTextColor(splashScreenTheme.textColor, splashScreenTheme.panelHeaderColor);
	TFT->drawString("Server",localCoords.x + 40, localCoords.y + 4);	

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, splashScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, splashScreenTheme.panelBorderColor);
}


void HS_SplashScreen::Update_ConnectingToServer(String serverID)
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(splashScreenTheme.textColor, splashScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(80, 15);
	textPrinter_Sprite->fillSprite(splashScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(serverID, 0, 0);
	textPrinter_Sprite->pushSprite(SERVER_PANEL_X + 26, SERVER_PANEL_Y + 25);

	textPrinter_Sprite->deleteSprite();

	TFT->fillCircle(SERVER_PANEL_X + 14, SERVER_PANEL_Y + 31, 7,TFT_RED);
}

