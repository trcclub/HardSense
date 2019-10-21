#include "HS_GameScreen.h"


HS_GameScreen::HS_GameScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	TFT->fillScreen(TFT_CYAN);
	TFT->drawString("Game Screen", 40, 40);

	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);

}

HS_GameScreen::~HS_GameScreen()
{
}


void HS_GameScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");

	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");

	/*
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c|/intelcpu/0/power/0,d");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e|/nvidiagpu/0/temperature/0,f|/nvidiagpu/0/clock/0,g|/nvidiagpu/0/control/0,h");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k");
	*/
}

void HS_GameScreen::UpdateScreen(String value)
{
	//Serial.println("HS_HomeScreen_B::UpdateScreen 1");

	char key = value.charAt(0);
	String subValue = value.substring(value.indexOf(",") + 1);

	//Serial.println("HS_HomeScreen_B::UpdateScreen 2");

	double dValue = subValue.toDouble();
	//Serial.println("HS_HomeScreen_B::UpdateScreen 3");

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

	//Serial.println("HS_HomeScreen_B::UpdateScreen 6");
}

void HS_GameScreen::HandleTouch(int x, int y)
{
	char buf[2];
	sprintf(buf, "%c", ScreenTypes::Home);
	AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
}