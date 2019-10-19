#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	//Serial.println("\n----\nCreate_Screen_Home() !!!");
	TFT->loadFont(AA_FONT_LARGE);
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);
	gpuLoadWidget = new HS_Dial_Widget(*TFT);
	
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);

	TFT->fillScreen(TFT_WHITE);

	Draw_CPU_Panel();
	Draw_GPU_Panel();
	Draw_Net_Panel();
	Draw_Ram_Panel();

	//Serial.println("Ending Create_Screen_Home() !!!");
}

HS_HomeScreen::~HS_HomeScreen()
{
	delete(cpuLoadWidget);
	delete(gpuLoadWidget);
}

void HS_HomeScreen::UpdateScreen(String value)
{
	char key = value.charAt(0);
	double dValue = value.substring(value.indexOf(",") + 1).toDouble();

	switch (key) {
	case 'a':
		Update_CPU_Panel_Load(dValue);
		break;
	case 'b':
		Update_CPU_Panel_Temperature(dValue);
		break;
	case 'c':
		Update_CPU_Panel_ClockSpeed(dValue);
		break;
	case 'd':
		Update_CPU_Panel_Power(dValue);
		break;
	case 'e':
		Update_GPU_Panel_Load(dValue);
		break;
	case 'f':
		Update_GPU_Panel_Temperature(dValue);
		break;
	case 'g':
		Update_GPU_Panel_ClockSpeed(dValue);
		break;
	case 'h':
		Update_GPU_Panel_FanControl(dValue);
		break;
	case 'i':
		Update_Net_DownloadSpeed(dValue);
		break;
	case 'j':
		Update_Net_UpLoadSpeed(dValue);
		break;
	case 'k':
		Update_Ram_Useage(dValue);
		break;
	default:
		break;
	}

}

void HS_HomeScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c|/intelcpu/0/power/0,d");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e|/nvidiagpu/0/temperature/0,f|/nvidiagpu/0/clock/0,g|/nvidiagpu/0/control/0,h");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k");
}


void HS_HomeScreen::HandleTouch(int x, int y)
{
	if (CPU_Panel_Touched(x, y))
	{
		// Do interesting things here
	}
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0, 22, 94, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);
		
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", 11, 25);
	TFT->drawString("P", 10, 47);
	TFT->drawString("U", 9, 69);

	//CPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0, 188, 94, BOX_BORDER_COLOR,PANEL_BGCOLOR, BOX_DROP_SHADOW);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123,4,67,26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	//TFT->drawString(degreesC, 184, 10);

	// CPU Power
	DrawBoxWithBorderAndDropShadow(118, 34, 77, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("W"), 189, 40);

	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(111, 64, 93, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("MHz"), 198, 70);

	/*
	TFT->setTextDatum(ML_DATUM);
	sd = "CPU";
	TFT->drawString(sd, 120, 47);
	*/

	Update_CPU_Panel_Temperature(0.0);
	Update_CPU_Panel_Power(00.0);
	Update_CPU_Panel_ClockSpeed(0000.0);
}

bool HS_HomeScreen::CPU_Panel_Touched(int x, int y)
{
	if ((x >= CPU_PANEL_LOW_X+5 && x <= CPU_PANEL_HIGH_X-5) && (y >= CPU_PANEL_LOW_Y+5 && y <= CPU_PANEL_HIGH_Y-5))
	{
		Serial.println("Touch me");
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::HomeB);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
		return true;
	}
	return false;
}

void HS_HomeScreen::Update_CPU_Panel_Load(double percentage)
{
	int angle = map(percentage, 0, 100, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX);

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
		cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	}

	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);
}

void HS_HomeScreen::Update_CPU_Panel_Temperature(double temp)
{
	textPrinter_Sprite->createSprite(62, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(127, 10);
	textPrinter_Sprite->deleteSprite();
	
	/*
	TFT->fillRect(128, 9, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 10);
	*/
}

void HS_HomeScreen::Update_CPU_Panel_Power(double power)
{
	textPrinter_Sprite->createSprite(67, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f W", power);
	textPrinter_Sprite->drawString(String(buf), 67, 0);
	textPrinter_Sprite->pushSprite(122, 40);
	textPrinter_Sprite->deleteSprite();

	/*
	TFT->fillRect(123, 39, 48, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 167, 40);
	*/
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	textPrinter_Sprite->createSprite(83, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(116, 70);
	textPrinter_Sprite->deleteSprite();



	/*
	//Serial.println("Update_CPU_Panel_ClockSpeed 1");
	TFT->fillRect(117, 69, 44, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	int i = clock;
	TFT->drawNumber(i, 118, 70);
	//Serial.println("Update_CPU_Panel_ClockSpeed 2");
	*/
}

void HS_HomeScreen::Draw_GPU_Panel()
{
	int yAdd = 95;
	String printer = "";
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0 + yAdd, 22, 94, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("G", 10, 25 + yAdd);
	TFT->drawString("P", 10, 47 + yAdd);
	TFT->drawString("U", 9, 69 + yAdd);



	//GPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0 + yAdd, 188, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	Update_GPU_Panel_Load(0.0);

	//GPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123, 4 + yAdd, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	//TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	//TFT->setTextDatum(TR_DATUM);
	//TFT->drawString(degreesC, 184, 10 + yAdd);

	// Fan load
	DrawBoxWithBorderAndDropShadow(123, 34 + yAdd, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("%"), 184, 40 + yAdd);


	// GPU Clock Speed
	DrawBoxWithBorderAndDropShadow(111, 64 + yAdd, 93, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//TFT->drawString(String("MHz"), 198, 70 + yAdd);

	Update_GPU_Panel_Temperature(00.0);
	Update_GPU_Panel_FanControl(00.0);
	Update_GPU_Panel_ClockSpeed(0000.0);

}

void HS_HomeScreen::Update_GPU_Panel_Load(double percentage)
{
	int angle = map(percentage, 0, 100, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX);

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
		gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	}
	gpuLoadWidget->PlotDial(SCREEN_HOME_GPU_LOAD_DIAL_X, SCREEN_HOME_GPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);
}

void HS_HomeScreen::Update_GPU_Panel_Temperature(double temp)
{
	textPrinter_Sprite->createSprite(62, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(127, 105);
	textPrinter_Sprite->deleteSprite();

	/*
	int yAdd = 95;
	TFT->fillRect(128, 9 + yAdd, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 10 + yAdd);
	*/
}

void HS_HomeScreen::Update_GPU_Panel_FanControl(double load)
{
	textPrinter_Sprite->createSprite(59, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.1f %%", load);
	textPrinter_Sprite->drawString(String(buf), 59, 0);
	textPrinter_Sprite->pushSprite(126, 135);
	textPrinter_Sprite->deleteSprite();

	/*
	int yAdd = 95;
	TFT->fillRect(128, 39 + yAdd, 38, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->drawFloat(temp, 1, 162, 40 + yAdd);
	*/
}

void HS_HomeScreen::Update_GPU_Panel_ClockSpeed(double clock)
{
	textPrinter_Sprite->createSprite(83, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(116, 165);
	textPrinter_Sprite->deleteSprite();

	/*
	int yAdd = 95;
	TFT->fillRect(117, 69 + yAdd, 44, 18, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	int i = clock;
	TFT->drawNumber(i, 158, 70 + yAdd);
	*/
}

void HS_HomeScreen::Draw_Net_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(206, 0, 113, 24, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("NET", 265, 13);

	

	DrawBoxWithBorderAndDropShadow(206, 22, 113, 51, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawFastHLine(206, 48, 113, BOX_BORDER_COLOR);
	TFT->drawFastHLine(206, 49, 113, BOX_DROP_SHADOW);


	TFT->fillTriangle(220, 30, 227, 41, 213, 41, TFT_GREEN);
	TFT->fillTriangle(220, 32, 225, 40, 215, 40, PANEL_BGCOLOR);

	TFT->fillTriangle(220, 65, 227, 54, 213, 54, TFT_GREEN);
	TFT->fillTriangle(220, 63, 225, 55, 215, 55, PANEL_BGCOLOR);
	
	Update_Net_UpLoadSpeed(0.0);
	Update_Net_DownloadSpeed(0.0);
	
}

void HS_HomeScreen::Update_Net_UpLoadSpeed(double uSpeed)
{
	textPrinter_Sprite->createSprite(81, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(uSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(232, 28);
	textPrinter_Sprite->deleteSprite();
	
	/*
	TFT->fillRect(229, 27, 87, 20, PANEL_BGCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);

	TFT->drawString(GetSpeedString(uSpeed), 312, 29);
	*/
}

void HS_HomeScreen::Update_Net_DownloadSpeed(double dSpeed)
{
	textPrinter_Sprite->createSprite(81, 18);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(dSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(232, 53);
	textPrinter_Sprite->deleteSprite();
	
	/*
	TFT->fillRect(229, 50, 87, 20, PANEL_BGCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	
	TFT->drawString(GetSpeedString(dSpeed), 312, 52);
	*/
}

String HS_HomeScreen::GetSpeedString(double speed)
{
	char buf[32];
	if (speed > 1000)
	{
		speed = speed / 1000;
		sprintf(buf, "%.1f MBs", speed);
	}
	else
	{
		sprintf(buf, "%.1f KBs", speed);
	}

	return String(buf);
}

void HS_HomeScreen::Draw_Ram_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(206, 72, 113, 24, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->drawString("RAM", 212, 76);
	//TFT->drawString("%", 300, 76);

	DrawBoxWithBorderAndDropShadow(206, 94, 113, 51, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->drawFastHLine(206, 123, 113, BOX_BORDER_COLOR);
	TFT->drawFastHLine(206, 124, 113, BOX_DROP_SHADOW);

	Update_Ram_Useage(0.0);
}

void HS_HomeScreen::Update_Ram_Useage(double dPercent)
{
	textPrinter_Sprite->createSprite(50, 18);
	textPrinter_Sprite->fillSprite(PANEL_HCOLOR);
	char buf[12];
	sprintf(buf, "%.0f %%", dPercent);
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	textPrinter_Sprite->drawString(String(buf), 50, 0);
	textPrinter_Sprite->pushSprite(262,76);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);  //put the text color back.
	/*
	TFT->fillRect(253, 75, 44, 18, PANEL_HCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(TR_DATUM);

	TFT->drawFloat(dPercent, 1, 295, 76);
	*/
}