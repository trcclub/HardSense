#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	Serial.println("\n----\nCreate_Screen_Home() !!!");
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);

	TFT->fillScreen(TFT_WHITE);

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
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	String printer = "";
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0, 22, 94, BOX_BORDER_COLOR, CPU_AREA_BGCOLOR, BOX_DROP_SHADOW);
		
	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", 11, 25);
	TFT->drawString("P", 10, 47);
	TFT->drawString("U", 9, 69);



	//CPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0, 188, 94, BOX_BORDER_COLOR,CPU_AREA_BGCOLOR, BOX_DROP_SHADOW);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123,4,67,26, BOX_BORDER_COLOR, CPU_AREA_BGCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->drawString(degreesC, 184, 10);


	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(113, 64, 89, 26, BOX_BORDER_COLOR, CPU_AREA_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawString(String("MHz"), 198, 70);
	

	/*
	TFT->setTextDatum(ML_DATUM);
	sd = "CPU";
	TFT->drawString(sd, 120, 47);
	*/

	Update_CPU_Panel_Tempererature(55.5);
	Update_CPU_Panel_ClockSpeed(4444.0);
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

	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);

}

void HS_HomeScreen::Update_CPU_Panel_Tempererature(double temp)
{
	Serial.println("Update_CPU_Panel_Tempererature 1");

	TFT->fillRect(128, 9, 38, 18, CPU_AREA_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 10);
	Serial.println("Update_CPU_Panel_Tempererature 2");
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	Serial.println("Update_CPU_Panel_ClockSpeed 1");
	TFT->fillRect(117, 69, 44, 18, CPU_AREA_BGCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->setTextColor(TFT_WHITE, CPU_AREA_BGCOLOR);
	int i = clock;
	TFT->drawNumber(i, 118, 70);
	Serial.println("Update_CPU_Panel_ClockSpeed 2");
}