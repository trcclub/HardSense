
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
	
	/*
	col[0] = 0xED7F;
	col[1] = 0xD4DB;
	col[2] = 0xBC38;
	col[3] = 0xABD6;
	col[4] = 0x9374;
	col[5] = 0x8332;
	col[6] = 0x72AF;
	col[7] = 0x624D;

	*/

	circleLoopCounter = 1;
	OTA_Update_In_Progress = false;	
	Connected_To_Network = false;

	Draw_OTA_Information_Panel();
	//Draw_Waiting_Animation();	
	allQueues->AddItemToCommandQueue(HardSense_Commands::BeginOTA, "");

}

HS_OTAScreen::~HS_OTAScreen()
{

}


// a = Connecting to network flag and network name
// b = Network connection success
// c = OTA Update Starting
// d = OTA Update is in progress - percentage
// e = OTA Update is complete
void HS_OTAScreen::UpdateScreen(String value)
{
	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);
	int iValue = subValue.toInt();

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
	case 'd':
		Update_In_Progress(iValue);
		break;
	case 'e':
		Ending_OTA_Update();
		break;
	default:
		break;
	}

}

void HS_OTAScreen::UpdateScreenOnInterval()
{
	if(!Connected_To_Network)
	{
		return;
	}
	
	if (millis() - lastUpdate > updateScreenInterval)
	{
		lastUpdate = millis();
		if(!OTA_Update_In_Progress)
		{
			Draw_Waiting_Animation();
		}
		
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

void HS_OTAScreen::Draw_Waiting_Animation()
{
	textPrinter_Sprite->createSprite(OTA_PANEL_WIDTH - 25, 30);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);
	textPrinter_Sprite->fillCircle(20 * circleLoopCounter, 14, 10, otaScreenTheme.panelHeaderColor);

	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 68);

	textPrinter_Sprite->deleteSprite();

	circleLoopCounter++;
	if(circleLoopCounter > 12)
	{
		circleLoopCounter = 1;
	}
	
}

void HS_OTAScreen::Draw_OTA_Information_Panel()
{
	HS_Coords localCoords(OTA_PANEL_X, OTA_PANEL_Y, OTA_PANEL_WIDTH, OAT_PANEL_HEIGHT);

	DrawBoxWithBorderAndDropShadow(localCoords, otaScreenTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, otaScreenTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 19, otaScreenTheme.panelHeaderColor);

	TFT->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelHeaderColor);
	TFT->drawString("OTA Update Status",localCoords.x + 50, localCoords.y + 5);	

	// Horizontal line seperator UNDER the header
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 23, localCoords.w - 6, otaScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 24, localCoords.w - 6, otaScreenTheme.panelBorderColor);


	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(160, 13);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString("SSID:", 0, 0);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 29);

	// Horizontal line seperator UNDER SSID
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 43, localCoords.w - 6, otaScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 44, localCoords.w - 6, otaScreenTheme.panelBorderColor);

	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("OTA ready and waiting....", 0, 0);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 49);

	DrawBoxWithBorderAndDropShadow(HS_Coords(OTA_PANEL_X + 6, OTA_PANEL_Y + 63, OTA_PANEL_WIDTH - 12, 40), otaScreenTheme);

	textPrinter_Sprite->deleteSprite();

}

void HS_OTAScreen::Update_NetworkConnectionInfo(String netID, uint16_t circleColor)
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(80, 13);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString(netID, 0, 0);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 58, OTA_PANEL_Y + 29);

	textPrinter_Sprite->deleteSprite();

	TFT->fillCircle(OTA_PANEL_X + 46, OTA_PANEL_Y + 33, 5,circleColor);
}

void HS_OTAScreen::Update_ConnectingToNetwork(String netID)
{
	Update_NetworkConnectionInfo(netID,TFT_RED);
}

void HS_OTAScreen::Update_ConnectedToNetwork(String netID)
{	
	Connected_To_Network = true;
	Update_NetworkConnectionInfo(netID,TFT_GREEN);
}

void HS_OTAScreen::Starting_OTA_Update()
{
	OTA_Update_In_Progress = true;

	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(190, 13);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString("OTA is running. Do not turn off.", 0, 0);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 49);
	textPrinter_Sprite->deleteSprite();
	
	
	textPrinter_Sprite->createSprite(OTA_PANEL_WIDTH - 25, 30);
	textPrinter_Sprite->fillSprite(0xEDE2);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 68);
	textPrinter_Sprite->deleteSprite();
}

void HS_OTAScreen::Update_In_Progress(int percentage)
{
	float gaugeWidth = OTA_PANEL_WIDTH - 25;
	int doneWidth = (gaugeWidth / (float)100) * percentage;
	
	textPrinter_Sprite->createSprite(gaugeWidth, 30);
	textPrinter_Sprite->fillSprite(0xEDE2);
	textPrinter_Sprite->fillRect(0,0,doneWidth,30,TFT_GREEN);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 68);

	textPrinter_Sprite->deleteSprite();
}

void HS_OTAScreen::Ending_OTA_Update()
{
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(240, 13);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);

	textPrinter_Sprite->drawString("OTA update has completed. Restarting...", 0, 0);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 49);
	textPrinter_Sprite->deleteSprite();


	textPrinter_Sprite->createSprite(OTA_PANEL_WIDTH - 25, 30);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelBGColor);
	textPrinter_Sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 68);
	textPrinter_Sprite->deleteSprite();	
}