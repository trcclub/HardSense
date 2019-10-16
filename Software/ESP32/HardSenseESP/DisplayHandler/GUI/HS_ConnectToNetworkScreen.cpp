#include "HS_ConnectToNetworkScreen.h"

HS_ConnectToNetworkScreen::HS_ConnectToNetworkScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	Serial.println("HS_SplashScreen() !!!");

	TFT->fillScreen(TFT_GREEN);
	TFT->setCursor(40, 40);
	TFT->setTextSize(2);
	TFT->print("Connect To Network");
}

HS_ConnectToNetworkScreen::~HS_ConnectToNetworkScreen()
{
}

void HS_ConnectToNetworkScreen::UpdateScreen(char* value)
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

char* HS_ConnectToNetworkScreen::GetSensorList()
{
	return "";
}