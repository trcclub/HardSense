#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	//Serial.println("\n----\nCreate_Screen_Home() !!!");
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);

	TFT->fillScreen(TFT_WHITE);

	Draw_CPU_Panel();
	//Draw_GPU_Panel();

	//Serial.println("Ending Create_Screen_Home() !!!");
}

HS_HomeScreen::~HS_HomeScreen()
{
	delete(cpuLoadWidget);
}

void HS_HomeScreen::UpdateScreen(String value)
{
	char key = value.charAt(0);
	String subValue = value.substring(value.indexOf(",") + 1);

	double dValue = subValue.toDouble();

	switch (key) {
	case 'a':
		Serial.print("HS_HomeScreen::UpdateScreen - Handle:  Update_CPU_Panel_Load: ");
		Serial.println(dValue);
		Update_CPU_Panel_Load(dValue);
		break;
	case 'b':
		//Update_CPU_Panel_Temperature(dValue);
		break;
	case 'c':
		//Update_CPU_Panel_ClockSpeed(dValue);
		break;
	case 'd':
		//Update_CPU_Panel_Power(dValue);
		break;
	case 'e':
		//Update_GPU_Panel_Load(dValue);
		break;
	case 'f':
		//Serial.println("Found GPU Temp");
		//Update_GPU_Panel_Temperature(dValue);
		break;
	default:
		break;
	}

}

void HS_HomeScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	// /intelcpu/0/load/0,a
	// /intelcpu/0/temperature/6,b
	// /intelcpu/0/clock/1,c"
	
	
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/6,b");
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/clock/1,c");
	//delay(20);
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/power/0,d");
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e");
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/temperature/0,f");
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0, 22, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", 11, 25);
	TFT->drawString("P", 10, 47);
	TFT->drawString("U", 9, 69);



	//CPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0, 188, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(90.9);

	/*
	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123, 4, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->drawString(degreesC, 184, 10);

	// CPU Power
	DrawBoxWithBorderAndDropShadow(123, 34, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawString(String("W"), 184, 40);


	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(113, 64, 89, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawString(String("MHz"), 198, 70);

	*/

	/*
	TFT->setTextDatum(ML_DATUM);
	sd = "CPU";
	TFT->drawString(sd, 120, 47);
	*/
	//Update_CPU_Panel_Temperature(0.0);
	//Update_CPU_Panel_Power(0.0);
	//Update_CPU_Panel_ClockSpeed(0000.0);
}

void HS_HomeScreen::Draw_GPU_Panel()
{
	/*
	int yAdd = 95;
	String printer = "";
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0 + yAdd, 22, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("G", 10, 25 + yAdd);
	TFT->drawString("P", 10, 47 + yAdd);
	TFT->drawString("U", 9, 69 + yAdd);



	//GPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0 + yAdd, 188, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	Update_GPU_Panel_Load(0.0);

	//GPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123, 4 + yAdd, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->drawString(degreesC, 184, 10 + yAdd);


	// GPU Power
	//DrawBoxWithBorderAndDropShadow(123, 34, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("W"), 184, 40);


	// CPU Clock Speed
	//DrawBoxWithBorderAndDropShadow(113, 64, 89, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("MHz"), 198, 70);

	Update_GPU_Panel_Temperature(00.0);
	*/
}

bool HS_HomeScreen::CPU_Panel_Touched(int x, int y)
{
	if ((x >= CPU_PANEL_LOW_X+5 && x <= CPU_PANEL_HIGH_X-5) && (y >= CPU_PANEL_LOW_Y+5 && y <= CPU_PANEL_HIGH_Y-5))
	{
		Serial.println("Touch me");
		return true;
	}
	return false;
}


void HS_HomeScreen::HandleTouch(int x, int y)
{
	if (CPU_Panel_Touched(x,y))
	{
		// Do interesting things here
	}
}

void HS_HomeScreen::Update_CPU_Panel_Load(double percentage)
{
	Serial.println("Update_CPU_Panel_Load START");
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

	Serial.print("Percentage: ");
	Serial.print(percentage);
	Serial.print("  |  angle: ");
	Serial.println(angle);
	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);

}

void HS_HomeScreen::Update_CPU_Panel_Temperature(double temp)
{
	//Serial.println("Update_CPU_Panel_Tempererature 1");

	TFT->fillRect(128, 9, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 10);
	//Serial.println("Update_CPU_Panel_Tempererature 2");
}

void HS_HomeScreen::Update_CPU_Panel_Power(double temp)
{
	TFT->fillRect(128, 39, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 40);
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	//Serial.println("Update_CPU_Panel_ClockSpeed 1");
	TFT->fillRect(117, 69, 44, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	int i = clock;
	TFT->drawNumber(i, 118, 70);
	//Serial.println("Update_CPU_Panel_ClockSpeed 2");
}


void HS_HomeScreen::Update_GPU_Panel_Load(double percentage)
{
	/*
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
	if (ringColor != Home_Screen_gpuLoadDial_CurrentRingColor)
	{
		Home_Screen_gpuLoadDial_CurrentRingColor = ringColor;
		gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	}

	gpuLoadWidget->PlotDial(SCREEN_HOME_GPU_LOAD_DIAL_X, SCREEN_HOME_GPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);
	*/
}

void HS_HomeScreen::Update_GPU_Panel_Temperature(double temp)
{
	int yAdd = 95;
	TFT->fillRect(128, 9+yAdd, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 10 + yAdd);
}