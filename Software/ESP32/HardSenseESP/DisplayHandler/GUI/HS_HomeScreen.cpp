#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	TFT->loadFont(AA_FONT_LARGE);
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);

	TFT->fillScreen(TFT_DARKGREY);
	TFT->fillScreen(TFT_WHITE);

	Draw_Net_Panel();
	Draw_Ram_Panel();
	Draw_HDD_Panel();

	cpuLoadWidget = new HS_Dial_Widget(*TFT);
	Draw_CPU_Panel();
	
	gpuLoadWidget = new HS_Dial_Widget(*TFT);
	Draw_GPU_Panel();
}

HS_HomeScreen::~HS_HomeScreen()
{
	delete(cpuLoadWidget);
	delete(gpuLoadWidget);
}

void HS_HomeScreen::UpdateScreen(String value)
{
	String str(value);
	char key = str.charAt(0);
	String subStr = str.substring(str.indexOf(",") + 1);
	double dValue = subStr.toDouble();

	switch (key) {
	case 'b':
		Update_CPU_Panel_Temperature(dValue);
		break;
	case 'c':
		Update_CPU_Panel_ClockSpeed(dValue);
		break;
	case 'd':
		Update_CPU_Panel_Power(dValue);
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
	case 'l':
		Update_Ram_Used(dValue);
		break;
	case 'm':
		Update_Ram_Free(dValue);
	case 'n':
	case 'o':
	case 'p':
	case 'q':
		Update_HDD_Useage(key,dValue);
		break;
	case 'a':
		Update_CPU_Panel_Load(dValue);
		break;
	case 'e':
		Update_GPU_Panel_Load(dValue);
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
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k|/ram/data/0,l|/ram/data/1,m");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/hdd/2/load/0,n|/hdd/3/load/0,o|/hdd/1/load/0,p|/hdd/0/load/0,q");

}


void HS_HomeScreen::HandleTouch(int x, int y)
{
	if (CPU_Panel_Touched(x, y))
	{
		Serial.println("Touch me");
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::HomeB);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	}
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	//DrawBoxWithBorderAndDropShadow(0, 0, 22, 94, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);
	DrawBoxWithBorderAndDropShadow(0, 0, 207, 95, BOX_BORDER_COLOR,PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(2, 2, 18, 91, PANEL_HCOLOR);
	TFT->drawFastVLine(19, 2, 91, BOX_BORDER_COLOR);
	TFT->drawFastVLine(20, 2, 91, BOX_BORDER_COLOR);
		
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", 10, 25);
	TFT->drawString("P", 9, 47);
	TFT->drawString("U", 8, 69);


	//CPU Package Load Dial Gauge

	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123,4,67,26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);

	// CPU Power
	DrawBoxWithBorderAndDropShadow(118, 34, 77, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(111, 64, 93, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	Update_CPU_Panel_Temperature(0.0);
	Update_CPU_Panel_Power(00.0);
	Update_CPU_Panel_ClockSpeed(0000.0);
}

bool HS_HomeScreen::CPU_Panel_Touched(int x, int y)
{
	if ((x >= CPU_PANEL_LOW_X+5 && x <= CPU_PANEL_HIGH_X-5) && (y >= CPU_PANEL_LOW_Y+5 && y <= CPU_PANEL_HIGH_Y-5))
	{
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
	textPrinter_Sprite->createSprite(62, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(127, 10);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_CPU_Panel_Power(double power)
{
	textPrinter_Sprite->createSprite(67, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f W", power);
	textPrinter_Sprite->drawString(String(buf), 67, 0);
	textPrinter_Sprite->pushSprite(122, 40);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	textPrinter_Sprite->createSprite(83, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(116, 70);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Draw_GPU_Panel()
{
	int yAdd = 95;
	TFT->loadFont(AA_FONT_LARGE);
	//DrawBoxWithBorderAndDropShadow(0, 0 + yAdd, 22, 94, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);
	DrawBoxWithBorderAndDropShadow(0, 0 + yAdd, 207, 95, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(2, 2 + yAdd, 18, 91, PANEL_HCOLOR);
	TFT->drawFastVLine(19, 2 + yAdd, 91, BOX_BORDER_COLOR);
	TFT->drawFastVLine(20, 2 + yAdd, 91, BOX_BORDER_COLOR);

	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("G", 9, 25 + yAdd);
	TFT->drawString("P", 9, 47 + yAdd);
	TFT->drawString("U", 8, 69 + yAdd);



	//GPU Package Load Dial Gauge
	gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	Update_GPU_Panel_Load(0.0);

	//GPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123, 4 + yAdd, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	// Fan load
	DrawBoxWithBorderAndDropShadow(123, 34 + yAdd, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	// GPU Clock Speed
	DrawBoxWithBorderAndDropShadow(111, 64 + yAdd, 93, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

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
	textPrinter_Sprite->createSprite(62, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(127, 105);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_GPU_Panel_FanControl(double load)
{
	textPrinter_Sprite->createSprite(59, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.1f %%", load);
	textPrinter_Sprite->drawString(String(buf), 59, 0);
	textPrinter_Sprite->pushSprite(126, 135);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_GPU_Panel_ClockSpeed(double clock)
{
	textPrinter_Sprite->createSprite(83, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(116, 165);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Draw_Net_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(207, 0, 113, 69, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->fillRect(210, 3, 107, 17, PANEL_HCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("NET", 266, 13);
	TFT->drawFastHLine(210, 20, 107, BOX_BORDER_COLOR);
	TFT->drawFastHLine(210, 21, 107, BOX_BORDER_COLOR);
	

	TFT->drawFastHLine(210, 43, 107, BOX_BORDER_COLOR);
	TFT->drawFastHLine(210, 44, 107, BOX_DROP_SHADOW);

	TFT->drawTriangle(221, 27, 228, 38, 214, 38, TFT_GREEN);
	TFT->drawTriangle(221, 60, 228, 49, 214, 49, TFT_RED);
	
	Update_Net_UpLoadSpeed(0.0);
	Update_Net_DownloadSpeed(0.0);
}

void HS_HomeScreen::Update_Net_UpLoadSpeed(double uSpeed)
{
	textPrinter_Sprite->createSprite(81, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(uSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(233, 25);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_Net_DownloadSpeed(double dSpeed)
{
	textPrinter_Sprite->createSprite(81, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(dSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(233, 48);
	textPrinter_Sprite->deleteSprite();
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
	//DrawBoxWithBorderAndDropShadow(206, 72, 113, 24, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);
	DrawBoxWithBorderAndDropShadow(207, 69, 113, 69, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);

	TFT->fillRect(210, 72, 107, 17, PANEL_HCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(TL_DATUM);
	TFT->drawString("RAM", 211, 73);

	TFT->drawFastHLine(210, 89, 107, BOX_BORDER_COLOR);
	TFT->drawFastHLine(210, 90, 107, BOX_BORDER_COLOR);
	

	TFT->setTextColor(TFT_RED, PANEL_BGCOLOR);
	TFT->drawString("U:", 211, 94);

	TFT->drawFastHLine(210, 112, 107, BOX_BORDER_COLOR);
	TFT->drawFastHLine(210, 113, 107, BOX_DROP_SHADOW);
	
	TFT->setTextColor(TFT_GREEN, PANEL_BGCOLOR);
	TFT->drawString("F:", 211, 116);

	Update_Ram_Used(0.0);
	Update_Ram_Useage(0.0);
}

void HS_HomeScreen::Update_Ram_Useage(double dPercent)
{
	textPrinter_Sprite->createSprite(50, 16);
	textPrinter_Sprite->fillSprite(PANEL_HCOLOR);
	char buf[12];
	sprintf(buf, "%.0f %%", dPercent);
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	textPrinter_Sprite->drawString(String(buf), 50, 0);
	textPrinter_Sprite->pushSprite(263,73);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);  //put the text color back.
}

void HS_HomeScreen::Update_Ram_Used(double used)
{
	textPrinter_Sprite->createSprite(80, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.2f GB", used);
	textPrinter_Sprite->drawString(String(buf), 80, 0);
	textPrinter_Sprite->pushSprite(233, 95);
	textPrinter_Sprite->deleteSprite();

}

void HS_HomeScreen::Update_Ram_Free(double free)
{
	textPrinter_Sprite->createSprite(80, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.2f GB", free);
	textPrinter_Sprite->drawString(String(buf), 80, 0);
	textPrinter_Sprite->pushSprite(233, 117);
	textPrinter_Sprite->deleteSprite();

}

void HS_HomeScreen::Draw_HDD_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);

	// C:  /hdd/2/load/0	n
	// E:  /hdd/3/load/0	o
	// F:  /hdd/1/load/0	p
	// G:  /hdd/0/load/0	q


	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(TC_DATUM);
	
	int xStart = 0;
	int x = xStart;
	int y = 190;
	int xAdd = 80;
	int multiplier = 0;

	// C:
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(x+3, y+3, 75, 22, PANEL_HCOLOR);
	TFT->drawString("C:", x+42, y+6);
	TFT->drawFastHLine(x, y+25, 80, BOX_BORDER_COLOR);
	TFT->drawFastHLine(x, y+26, 80, BOX_BORDER_COLOR);

	// E:
	x = xStart + xAdd;
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(x + 3, y + 3, 75, 22, PANEL_HCOLOR);
	TFT->drawString("E:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, BOX_BORDER_COLOR);
	TFT->drawFastHLine(x, y + 26, 80, BOX_BORDER_COLOR);

	// F:
	x = xStart + (xAdd*2);
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(x + 3, y + 3, 75, 22, PANEL_HCOLOR);
	TFT->drawString("F:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, BOX_BORDER_COLOR);
	TFT->drawFastHLine(x, y + 26, 80, BOX_BORDER_COLOR);

	// G:
	x = xStart + (xAdd*3);
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->fillRect(x + 3, y + 3, 75, 22, PANEL_HCOLOR);
	TFT->drawString("G:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, BOX_BORDER_COLOR);
	TFT->drawFastHLine(x, y + 26, 80, BOX_BORDER_COLOR);


	Update_HDD_Useage('n', 0.0);
	Update_HDD_Useage('o', 0.0);
	Update_HDD_Useage('p', 0.0);
	Update_HDD_Useage('q', 0.0);
}

void HS_HomeScreen::Update_HDD_Useage(char key, double percent)
{
	// C:  /hdd/2/load/0	n
	// E:  /hdd/3/load/0	o
	// F:  /hdd/1/load/0	p
	// G:  /hdd/0/load/0	q
	
	uint32_t circleColor = TFT_GREEN;
	if (percent > 95) {
		circleColor = TFT_RED;
	}
	else if (percent > 86) {
		circleColor = TFT_ORANGE;
	}
	else if (percent > 65)
	{
		circleColor = TFT_YELLOW;
	}


	int x = 4;
	int y = 220;
	int xAdd = 80;
	int multiplier = 0;
	switch (key) {
	case 'o':
		multiplier = 1;
		break;
	case 'p':
		multiplier = 2;
		break;
	case 'q':
		multiplier = 3;
		break;

	}
	x = x + (xAdd * multiplier);


	
	textPrinter_Sprite->createSprite(70, 16);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	char buf[12];
	sprintf(buf, "%.1f %%", percent);
	textPrinter_Sprite->drawString(String(buf), 70, 0);
	textPrinter_Sprite->fillCircle(6, 7, 5, circleColor);
	textPrinter_Sprite->pushSprite(x, y);
	textPrinter_Sprite->deleteSprite();
}