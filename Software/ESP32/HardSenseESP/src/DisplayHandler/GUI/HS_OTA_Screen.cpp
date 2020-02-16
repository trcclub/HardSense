
#include "HS_OTA_Screen.h"

HS_OTAScreen::HS_OTAScreen(Queues *newQueues, TFT_eSPI* newTFT) : HS_ScreenBase(newQueues, newTFT)
{

	otaScreenTheme.panelBGColor = PANEL_BGCOLOR;
	otaScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	otaScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	otaScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	otaScreenTheme.textColor = TEXT_COLOR;

	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_14PT);
	TFT->unloadFont();
	TFT->loadFont(AA_FONT_18PT);    

	TFT->fillScreen(otaScreenTheme.panelBGColor);
	
	
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

	Draw_NetworkConnectionPanel();
	
	allQueues->AddItemToCommandQueue(HardSense_Commands::BeginOTA, "");

}

HS_OTAScreen::~HS_OTAScreen()
{

}


// a = Connecting to network flag and network name
// b = Network connection success
// c = OTA Update Starting
// d = 
void HS_OTAScreen::UpdateScreen(String value)
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
		Starting_OTA_Update();
		break;
		
	default:
		break;
	}

}

void HS_OTAScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		lastUpdate = millis();
		UpdateCircle();
	}
}

void HS_OTAScreen::HandleTouch(int x, int y)
{
	if (HiddenHomeScreen_Touched(x,y))
	{
		allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::BluetoothConfigurator));
		//allQueues->AddItemToCommandQueue(HardSense_Commands::EnableBluetooth,"");
	}
}


void HS_OTAScreen::UpdateCircle()
{
	TFT->fillCircle(250 + 40 * (cos(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), 90 + 40 * (sin(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), 10, col[secondaryLoopCounter]); 
	
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

void HS_OTAScreen::Draw_NetworkConnectionPanel()
{
	HS_Coords localCoords(NET_PANEL_X, NET_PANEL_Y, 190, 45);

	DrawBoxWithBorderAndDropShadow(localCoords, otaScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, otaScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, otaScreenTheme.panelHeaderColor);

	TFT->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelHeaderColor);
	TFT->drawString("OTA Update Status",localCoords.x + 10, localCoords.y + 4);	

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, otaScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, otaScreenTheme.panelBorderColor);
}

void HS_OTAScreen::Update_NetworkConnectionInfo(String netID, uint16_t circleColor)
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(80, 15);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(netID, 0, 0);
	textPrinter_Sprite->pushSprite(NET_PANEL_X + 26, NET_PANEL_Y + 25);

	textPrinter_Sprite->deleteSprite();

	TFT->fillCircle(NET_PANEL_X + 14, NET_PANEL_Y + 31, 7,circleColor);
}


void HS_OTAScreen::Update_ConnectingToNetwork(String netID)
{
	Update_NetworkConnectionInfo(netID,TFT_RED);
}

void HS_OTAScreen::Update_ConnectedToNetwork(String netID)
{	
	Update_NetworkConnectionInfo(netID,TFT_GREEN);
}

void HS_OTAScreen::Starting_OTA_Update()
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(80, 15);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString("OTA Starting,,", 0, 0);
	textPrinter_Sprite->pushSprite(NET_PANEL_X + 26, NET_PANEL_Y + 90);

	textPrinter_Sprite->deleteSprite();

}