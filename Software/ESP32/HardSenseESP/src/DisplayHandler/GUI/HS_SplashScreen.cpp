#include "HS_SplashScreen.h"

HS_SplashScreen::HS_SplashScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	splashScreenTheme.panelBGColor = PANEL_BGCOLOR;
	splashScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	splashScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	splashScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	splashScreenTheme.textColor = TEXT_COLOR;

	Serial.println("Top of Splash Screen");

	TFT->fillScreen(splashScreenTheme.panelBGColor);


	uint32_t col[8];
	col[0] = 0xED7F;
	col[1] = 0xD4DB;
	col[2] = 0xBC38;
	col[3] = 0xABD6;
	col[4] = 0x9374;
	col[5] = 0x8332;
	col[6] = 0x72AF;
	col[7] = 0x624D;

	for (int i = 8; i > 0; i--) {
		TFT->fillCircle(240 + 40 * (cos(-i * PI / 4)), 120 + 40 * (sin(-i * PI / 4)), 10, col[0]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 1) * PI / 4)), 120 + 40 * (sin(-(i + 1) * PI / 4)), 10, col[1]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 2) * PI / 4)), 120 + 40 * (sin(-(i + 2) * PI / 4)), 10, col[2]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 3) * PI / 4)), 120 + 40 * (sin(-(i + 3) * PI / 4)), 10, col[3]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 4) * PI / 4)), 120 + 40 * (sin(-(i + 4) * PI / 4)), 10, col[4]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 5) * PI / 4)), 120 + 40 * (sin(-(i + 5) * PI / 4)), 10, col[5]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 6) * PI / 4)), 120 + 40 * (sin(-(i + 6) * PI / 4)), 10, col[6]); delay(15);
		TFT->fillCircle(240 + 40 * (cos(-(i + 7) * PI / 4)), 120 + 40 * (sin(-(i + 7) * PI / 4)), 10, col[7]); delay(15);
	}


	Serial.println("Ending of Splash Screen");

	/*
	Serial.println("HS_SplashScreen() !!!");

	TFT->fillScreen(TFT_YELLOW);
	TFT->setCursor(40, 40);
	TFT->setTextSize(2);
	TFT->print("Splash Screen");
	*/
}

HS_SplashScreen::~HS_SplashScreen()
{
}

void HS_SplashScreen::UpdateScreen(String value)
{
	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);

	double dValue = subValue.toDouble();

	switch (key) {
	case 'a':
		//Update_Screen_Home_Ethernet_Recv(TFT, dValue);
		break;
	case 'b':
		//Update_CPU_Total_Load(dValue);
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
	}
}