#include "HS_CPUScreen.h"

HS_CPUScreen::HS_CPUScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	cpuScreenTheme.panelBGColor = PANEL_BGCOLOR;
	cpuScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	cpuScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	cpuScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	cpuScreenTheme.textColor = TEXT_COLOR;


	TFT->loadFont(AA_FONT_14PT);
	TFT->fillScreen(TFT_BLACK);

	textPrinter_Sprite->setTextColor(TEXT_COLOR, PANEL_BGCOLOR);
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_14PT);
}

HS_CPUScreen::~HS_CPUScreen()
{

}


void HS_CPUScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
}


void HS_CPUScreen::UpdateScreen(String value)
{
	String str(value);
	char key = str.charAt(0);
	String subStr = str.substring(str.indexOf(",") + 1);
	double dValue = subStr.toDouble();

	switch (key) {
	case 'a':
		
		break;
	default:
		break;
	}
}


void HS_CPUScreen::UpdateScreenOnInterval()
{
}

void HS_CPUScreen::HandleTouch(int x, int y)
{
	if (HiddenHomeScreen_Touched(x, y))
	{
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::Home);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	}
}