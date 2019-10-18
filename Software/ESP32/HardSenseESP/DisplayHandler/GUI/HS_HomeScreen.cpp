#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	Serial.println("\n----\nCreate_Screen_Home() !!!");
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);

	TFT->fillScreen(TFT_DARKGREY);

	Draw_CPU_Panel();

	Serial.println("Ending Create_Screen_Home() !!!");
}

HS_HomeScreen::~HS_HomeScreen()
{
	delete(cpuLoadWidget);
}

void HS_HomeScreen::UpdateScreen(char* value)
{
	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);

	double dValue = subValue.toDouble();

	switch (key) {
	case 'a':
		Update_CPU_Panel_Load(dValue);
		break;
	case 'b':
		Update_CPU_Panel_Tempererature(dValue);
		break;
	case 'c':
		Update_CPU_Panel_ClockSpeed(dValue);
		break;
	default:
		break;
	}

}

void HS_HomeScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, char* value))
{
	// /intelcpu/0/load/0,a
	// /intelcpu/0/temperature/6,b
	// /intelcpu/0/clock/1,c"
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/6,b");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/clock/1,c");

	/*
	Serial.println("HS_HomeScreen::GetSensorList()");
	char* buf = new char[MAX_QUEUE_ITEM_VALUE_SIZE];
	//char buf[MAX_QUEUE_ITEM_VALUE_SIZE];
	strcpy(buf, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c");

	Serial.print("HS_HomeScreen::GetSensorList():  Return buf:");
	Serial.println(buf);
	Serial.println("Did we get here?");
	return buf;
	//return "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b";
	//return "/intelcpu/0/clock/1,c";
	*/
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);

	//CPU Package Load Dial Gauge
	TFT->fillRect(0, 0, 200, 92, TFT_DARKGREY);
	TFT->fillRect(2, 2, 196, 88, CPU_AREA_BGCOLOR);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	//CPU Package Temperature
	TFT->fillRect(90, 5, 67, TFT->fontHeight() + 8, TFT_DARKGREY);
	TFT->fillRect(92, 7, 63, TFT->fontHeight() + 4, CPU_AREA_BGCOLOR);

	TFT->setCursor(135, 10);
	char d = 0xB0;
	TFT->print(d);
	TFT->print("C");
	Update_CPU_Panel_Tempererature(0.0);

	// CPU Clock Speed
	TFT->fillRect(90, TFT->fontHeight() + 14, 89, TFT->fontHeight() + 8, TFT_DARKGREY);
	TFT->fillRect(92, TFT->fontHeight() + 16, 85, TFT->fontHeight() + 4, CPU_AREA_BGCOLOR);
	TFT->setCursor(138, TFT->fontHeight() + 19);
	TFT->print("MHz");
	Update_CPU_Panel_ClockSpeed(0.0);
}

void HS_HomeScreen::Update_CPU_Panel_Load(double percentage)
{
	int angle = map(percentage, 0, 100, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX);

	uint16_t ringColor;
	if (percentage > 94)
	{
		ringColor = TFT_RED;
	}
	else if (percentage > 82)
	{
		ringColor = TFT_ORANGE;
	}
	else if (percentage > 61)
	{
		ringColor = TFT_YELLOW;
	}
	else
	{
		ringColor = TFT_GREEN;
	}
	if (ringColor != Home_Screen_cpuLoadDial_CurrentRingColor)
	{
		Home_Screen_cpuLoadDial_CurrentRingColor = ringColor;
		cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	}

	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage);

}

void HS_HomeScreen::Update_CPU_Panel_Tempererature(double temp)
{
	Serial.println("Update_CPU_Panel_Tempererature 1");
	TFT->fillRect(93, 9, 40, TFT->fontHeight(), CPU_AREA_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	TFT->drawFloat(temp, 1, 128, 9);
	Serial.println("Update_CPU_Panel_Tempererature 2");
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	Serial.println("Update_CPU_Panel_ClockSpeed 1");
	TFT->fillRect(93, TFT->fontHeight() + 16, 46, TFT->fontHeight(), CPU_AREA_BGCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	int i = clock;
	TFT->drawNumber(i, 95, TFT->fontHeight() + 19);
	//TFT->drawFloat(clock, 0, 95, TFT->fontHeight() + 19);
	Serial.println("Update_CPU_Panel_ClockSpeed 2");
}