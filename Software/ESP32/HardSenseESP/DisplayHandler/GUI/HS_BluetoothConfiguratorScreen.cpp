#include "HS_BluetoothConfiguratorScreen.h"

HS_BluetoothConfiguratorScreen::HS_BluetoothConfiguratorScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	Serial.println("HS_SplashScreen() !!!");

	TFT->fillScreen(TFT_RED);
	TFT->setCursor(40, 40);
	TFT->setTextSize(2);
	TFT->print("Bluetooth Configurator");
}

HS_BluetoothConfiguratorScreen::~HS_BluetoothConfiguratorScreen()
{
}

void HS_BluetoothConfiguratorScreen::UpdateScreen(String value)
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

char* HS_BluetoothConfiguratorScreen::GetSensorList()
{
	return "";
}