#include "HS_HomeScreen.h"

HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	homeScreenTheme.panelBGColor = PANEL_BGCOLOR;
	homeScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	homeScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	homeScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	homeScreenTheme.textColor = TEXT_COLOR;

	TFT->loadFont(AA_FONT_18PT);
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);
	gpuLoadWidget = new HS_Dial_Widget(*TFT);
	rtc.begin();

	textPrinter_Sprite->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelBGColor);

	TFT->fillScreen(TFT_DARKGREY);

	Draw_CPU_Panel();
	Draw_GPU_Panel();
	Draw_Time_Panel();
	Draw_Net_Panel();
	Draw_Mem_Panel();
	Draw_HDD_Panel();
}

HS_HomeScreen::~HS_HomeScreen()
{
	delete(cpuLoadWidget);
	delete(gpuLoadWidget);
	delete(netPanel);
	delete(memPanel);
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
		netPanel->Update_Net_DownloadSpeed(dValue);
		break;
	case 'j':
		netPanel->Update_Net_UpLoadSpeed(dValue);
		break;
	case 'k':
		memPanel->Update_Mem_Load(dValue);
		//Update_Mem_Useage(dValue);
		break;
	case 'l':
		memPanel->Update_Mem_Used(dValue);
		//Update_Mem_Used(dValue);
		break;
	case 'm':
		memPanel->Update_Mem_Free(dValue);
		//Update_Mem_Free(dValue);
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

void HS_HomeScreen::UpdateScreenOnInterval()
{
	if (millis() - lastTimeUpate > updateTimeInterval)
	{
		lastTimeUpate = millis();
		Update_Time();
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
	if (GPU_Panel_Touched(x, y))
	{
		Serial.println("Touch me");
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::Game);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	}
}

void HS_HomeScreen::Draw_CPU_Panel()
{
	int x = CPU_PANEL_X;
	int y = CPU_PANEL_Y;

	TFT->loadFont(AA_FONT_18PT);
	DrawBoxWithBorderAndDropShadow(x, y, 207, 95, homeScreenTheme.panelBorderColor,homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x+2, y+2, 18, 91, homeScreenTheme.panelHeaderColor);
	TFT->drawFastVLine(x+19, y + 2, 91, homeScreenTheme.panelBorderColor);
	TFT->drawFastVLine(x + 20, y + 2, 91, homeScreenTheme.panelBorderColor);
		
	TFT->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelHeaderColor);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", x + 10, y + 25);
	TFT->drawString("P", x + 9, y + 47);
	TFT->drawString("U", x + 8, y + 69);


	//CPU Package Load Dial Gauge

	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(x + 123, y + 4,67,26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelBGColor);
	TFT->setTextDatum(TR_DATUM);

	// CPU Power
	DrawBoxWithBorderAndDropShadow(x + 118, y + 34, 77, 26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);

	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(x + 111, y + 64, 93, 26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);

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

	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage, homeScreenTheme.panelDropShadowColor);
}

void HS_HomeScreen::Update_CPU_Panel_Temperature(double temp)
{
	int x = CPU_PANEL_X;
	int y = CPU_PANEL_Y;
	textPrinter_Sprite->createSprite(62, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(x+127, y + 10);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_CPU_Panel_Power(double power)
{
	int x = CPU_PANEL_X;
	int y = CPU_PANEL_Y;
	textPrinter_Sprite->createSprite(67, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[10];
	sprintf(buf, "%.1f W", power);
	textPrinter_Sprite->drawString(String(buf), 67, 0);
	textPrinter_Sprite->pushSprite(x + 122, y + 40);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_CPU_Panel_ClockSpeed(double clock)
{
	int x = CPU_PANEL_X;
	int y = CPU_PANEL_Y;
	textPrinter_Sprite->createSprite(83, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(x + 116, y + 70);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Draw_GPU_Panel()
{
	int x = GPU_PANEL_X;
	int y = GPU_PANEL_Y;
	TFT->loadFont(AA_FONT_18PT);
	DrawBoxWithBorderAndDropShadow(x, y, 207, 95, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x+2, y + 2, 18, 91, homeScreenTheme.panelHeaderColor);
	TFT->drawFastVLine(x + 19, y + 2, 91, homeScreenTheme.panelBorderColor);
	TFT->drawFastVLine(x + 20, y + 2, 91, homeScreenTheme.panelBorderColor);

	TFT->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelHeaderColor);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("G", x + 9, y + 25);
	TFT->drawString("P", x + 9, y + 47);
	TFT->drawString("U", x + 8, y + 69);



	//GPU Package Load Dial Gauge
	gpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_gpuLoadDial_CurrentRingColor);
	Update_GPU_Panel_Load(0.0);

	//GPU Package Temperature
	DrawBoxWithBorderAndDropShadow(x + 123, y + 4, 67, 26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);

	// Fan load
	DrawBoxWithBorderAndDropShadow(x + 123, y + 34, 67, 26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);

	// GPU Clock Speed
	DrawBoxWithBorderAndDropShadow(x + 111, y + 64, 93, 26, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);

	Update_GPU_Panel_Temperature(00.0);
	Update_GPU_Panel_FanControl(00.0);
	Update_GPU_Panel_ClockSpeed(0000.0);
}

bool HS_HomeScreen::GPU_Panel_Touched(int x, int y)
{
	if ((x >= GPU_PANEL_LOW_X + 5 && x <= GPU_PANEL_HIGH_X - 5) && (y >= GPU_PANEL_LOW_Y + 5 && y <= GPU_PANEL_HIGH_Y - 5))
	{
		return true;
	}
	return false;
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
	gpuLoadWidget->PlotDial(SCREEN_HOME_GPU_LOAD_DIAL_X, SCREEN_HOME_GPU_LOAD_DIAL_Y, angle, "Load", percentage, homeScreenTheme.panelDropShadowColor);
}

void HS_HomeScreen::Update_GPU_Panel_Temperature(double temp)
{
	int x = GPU_PANEL_X;
	int y = GPU_PANEL_Y;
	textPrinter_Sprite->createSprite(62, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 62, 0);
	textPrinter_Sprite->pushSprite(x + 127, y+ 10);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_GPU_Panel_FanControl(double load)
{
	int x = GPU_PANEL_X;
	int y = GPU_PANEL_Y;
	textPrinter_Sprite->createSprite(59, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.1f %%", load);
	textPrinter_Sprite->drawString(String(buf), 59, 0);
	textPrinter_Sprite->pushSprite(x + 126, y + 40);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Update_GPU_Panel_ClockSpeed(double clock)
{
	int x = GPU_PANEL_X;
	int y = GPU_PANEL_Y;
	textPrinter_Sprite->createSprite(83, 16);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 83, 0);
	textPrinter_Sprite->pushSprite(x + 116, y + 70);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Draw_Net_Panel()
{
	netPanel = new HS_NetPanel(TFT, HS_Coords(NET_PANEL_X, NET_PANEL_Y, 0, 0), homeScreenTheme);
	netPanel->DrawPanel();
}

void HS_HomeScreen::Draw_Mem_Panel()
{
	memPanel = new HS_MemPanel(TFT, HS_Coords(RAM_PANEL_X, RAM_PANEL_Y, 0, 0), homeScreenTheme);
	memPanel->DrawPanel();
}

void HS_HomeScreen::Draw_HDD_Panel()
{
	TFT->loadFont(AA_FONT_18PT);
	TFT->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelHeaderColor);

	// C:  /hdd/2/load/0	n
	// E:  /hdd/3/load/0	o
	// F:  /hdd/1/load/0	p
	// G:  /hdd/0/load/0	q

	TFT->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelHeaderColor);
	TFT->setTextDatum(TC_DATUM);
	
	int xStart = HDD_PANEL_X;
	int x = xStart;
	int y = HDD_PANEL_Y;
	int xAdd = 80;
	int multiplier = 0;

	// C:
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x+3, y+3, 75, 22, homeScreenTheme.panelHeaderColor);
	TFT->drawString("C:", x+42, y+6);
	TFT->drawFastHLine(x, y+25, 80, homeScreenTheme.panelBorderColor);
	TFT->drawFastHLine(x, y+26, 80, homeScreenTheme.panelBorderColor);

	// E:
	x = xStart + xAdd;
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x + 3, y + 3, 75, 22, homeScreenTheme.panelHeaderColor);
	TFT->drawString("E:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, homeScreenTheme.panelBorderColor);
	TFT->drawFastHLine(x, y + 26, 80, homeScreenTheme.panelBorderColor);

	// F:
	x = xStart + (xAdd*2);
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x + 3, y + 3, 75, 22, homeScreenTheme.panelHeaderColor);
	TFT->drawString("F:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, homeScreenTheme.panelBorderColor);
	TFT->drawFastHLine(x, y + 26, 80, homeScreenTheme.panelBorderColor);

	// G:
	x = xStart + (xAdd*3);
	DrawBoxWithBorderAndDropShadow(x, y, 80, 50, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	TFT->fillRect(x + 3, y + 3, 75, 22, homeScreenTheme.panelHeaderColor);
	TFT->drawString("G:", x + 42, y + 6);
	TFT->drawFastHLine(x, y + 25, 80, homeScreenTheme.panelBorderColor);
	TFT->drawFastHLine(x, y + 26, 80, homeScreenTheme.panelBorderColor);


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
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.1f %%", percent);
	textPrinter_Sprite->drawString(String(buf), 70, 0);
	textPrinter_Sprite->fillCircle(6, 7, 5, circleColor);
	textPrinter_Sprite->pushSprite(x, y);
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen::Draw_Time_Panel()
{
	int x = TIME_PANEL_X;
	int y = TIME_PANEL_Y;

	TFT->loadFont(AA_FONT_18PT);
	DrawBoxWithBorderAndDropShadow(x, y, 113, 52, homeScreenTheme.panelBorderColor, homeScreenTheme.panelBGColor, homeScreenTheme.panelDropShadowColor);
	Update_Time();
}

void HS_HomeScreen::Update_Time()
{
	int x = TIME_PANEL_X;
	int y = TIME_PANEL_Y;

	DateTime now = rtc.now();
	int d = now.day();
	if (d < 10)
	{
		d = d + 10;
	}
	String date(daysOfTheWeek[now.dayOfTheWeek()]);
	date += ", " + String(monthsOfTheYear[now.month()-1]);
	date += " " + String(d);


	textPrinter_Sprite->createSprite(107,46);
	textPrinter_Sprite->fillSprite(homeScreenTheme.panelHeaderColor);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelHeaderColor);

	textPrinter_Sprite->drawString(date,53,4);
	

	int hour = now.hour();
	int minutes = now.minute();
	String min = "";
	if (minutes < 10)
	{
		min = "0";
	}
	min += String(minutes);

	int seconds = now.second();
	String sec = "";
	if (seconds < 10)
	{
		sec = "0";
	}
	sec += String(seconds);

	if (hour > 12)
	{
		date = String(hour - 12) + ":" + min + " " + sec + " PM";
	}
	else
	{

		date = String(hour) + ":" + min + ":" + sec + " AM";
	}
	textPrinter_Sprite->drawString(date, 53, 26);
	
	textPrinter_Sprite->pushSprite(x + 3, y + 3);

	textPrinter_Sprite->setTextColor(homeScreenTheme.textColor, homeScreenTheme.panelBGColor);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->deleteSprite();
}