
#include "HS_OTA_Screen.h"

HS_OTAScreen::HS_OTAScreen(Queues *newQueues, TFT_eSPI* newTFT) : HS_ScreenBase(newQueues, newTFT)
{
	otaScreenTheme.panelBGColor = PANEL_BGCOLOR;
	otaScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	otaScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	otaScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	otaScreenTheme.textColor = TEXT_COLOR;


	TFT->unloadFont();
	TFT->setTextSize(1);

	TFT->fillScreen(otaScreenTheme.panelBGColor);
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->setTextSize(1);

    
    
	HS_Coords localCoords(40, 40, 113, 127);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->createSprite(95, 15);

	textPrinter_Sprite->setTextColor(otaScreenTheme.textColor, otaScreenTheme.panelHeaderColor);
	textPrinter_Sprite->fillSprite(otaScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("OTA", 0,0);
	textPrinter_Sprite->pushSprite(localCoords.x + 45, localCoords.y + 4);


	textPrinter_Sprite->deleteSprite();
}

HS_OTAScreen::~HS_OTAScreen()
{

}


// a = 
// b = 
// c = 
void HS_OTAScreen::UpdateScreen(String value)
{
	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);

	switch (key) {
	case 'a':
		
		break;
	case 'b':
    
		break;
	case 'c':

		break;
	default:
		break;
	}

}

void HS_OTAScreen::HandleTouch(int x, int y)
{
	if (HiddenHomeScreen_Touched(x,y))
	{
		allQueues->AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::BluetoothConfigurator));
	}
}