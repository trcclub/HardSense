#include "HS_HomeScreen_B.h"


HS_HomeScreen_B::HS_HomeScreen_B(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);
	//gpuLoadWidget = new HS_Dial_Widget(*TFT);

	//textPrinter_Sprite = new TFT_eSprite(TFT);
	//textPrinter_Sprite->loadFont(AA_FONT_LARGE);

	TFT->fillScreen(TFT_ORANGE);

	Draw_CPU_Panel();
	//Draw_GPU_Panel();
	Draw_Net_Panel();
	//Draw_Ram_Panel();

	//Serial.println("Ending Create_Screen_Home() !!!");
}

HS_HomeScreen_B::~HS_HomeScreen_B()
{
	//TFT->unloadFont();
	delete(cpuLoadWidget);
	//delete(gpuLoadWidget);

	//textPrinter_Sprite->unloadFont();
	//textPrinter_Sprite->deleteSprite();
	//delete(textPrinter_Sprite);
}


void HS_HomeScreen_B::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");

	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");

	/*
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c|/intelcpu/0/power/0,d");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e|/nvidiagpu/0/temperature/0,f|/nvidiagpu/0/clock/0,g|/nvidiagpu/0/control/0,h");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k");
	*/
}
void HS_HomeScreen_B::UpdateScreen(String value)
{
	//Serial.println("HS_HomeScreen_B::UpdateScreen 1");

	char key = value.charAt(0);
	String subValue = value.substring(value.indexOf(",") + 1);

	//Serial.println("HS_HomeScreen_B::UpdateScreen 2");

	double dValue = subValue.toDouble();
	//Serial.println("HS_HomeScreen_B::UpdateScreen 3");

	switch (key) {
	case 'a':
		Serial.println("HS_HomeScreen_B::UpdateScreen 1");
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
		//Update_GPU_Panel_Temperature(dValue);
		break;
	case 'g':
		//Update_GPU_Panel_ClockSpeed(dValue);
		break;
	case 'h':
		//Update_GPU_Panel_FanControl(dValue);
		break;
	case 'i':
		//Update_Net_DownloadSpeed(dValue);
		break;
	case 'j':
		//Update_Net_UpLoadSpeed(dValue);
		break;
	case 'k':
		//Update_Ram_Useage(dValue);
		break;
	default:
		break;
	}

	//Serial.println("HS_HomeScreen_B::UpdateScreen 6");
}

void HS_HomeScreen_B::HandleTouch(int x, int y)
{
	if (CPU_Panel_Touched(x, y))
	{
		// Do interesting things here
	}
}

bool HS_HomeScreen_B::CPU_Panel_Touched(int x, int y)
{
	if ((x >= CPU_PANEL_LOW_X + 5 && x <= CPU_PANEL_HIGH_X - 5) && (y >= CPU_PANEL_LOW_Y + 5 && y <= CPU_PANEL_HIGH_Y - 5))
	{
		Serial.println("Touch me B");
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::Home);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
		return true;
	}
	return false;
}

void HS_HomeScreen_B::Draw_CPU_Panel()
{
	TFT->loadFont(AA_FONT_LARGE);
	DrawBoxWithBorderAndDropShadow(0, 0, 22, 94, BOX_BORDER_COLOR, PANEL_HCOLOR, BOX_DROP_SHADOW);

	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("C", 11, 25);
	TFT->drawString("P", 10, 47);
	TFT->drawString("U", 9, 69);

	//CPU Package Load Dial Gauge
	DrawBoxWithBorderAndDropShadow(20, 0, 188, 94, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_LOAD_DIAL_MIN, SCREEN_HOME_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Panel_Load(0.0);

	/*
	//CPU Package Temperature
	DrawBoxWithBorderAndDropShadow(123, 4, 67, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);
	TFT->drawString(degreesC, 184, 10);

	// CPU Power
	DrawBoxWithBorderAndDropShadow(118, 34, 77, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawString(String("W"), 189, 40);

	// CPU Clock Speed
	DrawBoxWithBorderAndDropShadow(113, 64, 89, 26, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	TFT->drawString(String("MHz"), 198, 70);
	*/
	
	//Update_CPU_Panel_Temperature(0.0);
	//Update_CPU_Panel_Power(999.9);
	//Update_CPU_Panel_ClockSpeed(0000.0);
}

void HS_HomeScreen_B::Update_CPU_Panel_Load(double percentage)
{
	Serial.println("HS_HomeScreen_B::Update_CPU_Panel_Load 1");
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

	Serial.println("HS_HomeScreen_B::Update_CPU_Panel_Load 2");
	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "Load", percentage, BOX_DROP_SHADOW);
	Serial.println("HS_HomeScreen_B::Update_CPU_Panel_Load 3");
}



void HS_HomeScreen_B::Draw_Net_Panel()
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

	//Update_Net_UpLoadSpeed(0.0);
	//Update_Net_DownloadSpeed(0.0);

}

void HS_HomeScreen_B::Update_Net_UpLoadSpeed(double uSpeed)
{
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->createSprite(81, 18);

	//Serial.println("HS_HomeScreen_B::Update_Net_UpLoadSpeed 2");
	//textPrinter_Sprite->loadFont(AA_FONT_LARGE);
	//Serial.println("HS_HomeScreen_B::Update_Net_UpLoadSpeed 3");

	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(uSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(232, 28);
	//textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->deleteSprite();
}

void HS_HomeScreen_B::Update_Net_DownloadSpeed(double dSpeed)
{
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->createSprite(81, 18);


	//Serial.println("HS_HomeScreen_B::Update_Net_DownloadSpeed 2");
	//textPrinter_Sprite->loadFont(AA_FONT_LARGE);
	//Serial.println("HS_HomeScreen_B::Update_Net_DownloadSpeed 3");

	textPrinter_Sprite->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->drawString(GetSpeedString(dSpeed),81,0);
	textPrinter_Sprite->pushSprite(232,53);
	//textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->deleteSprite();
	
	
	/*
	TFT->fillRect(229, 50, 87, 20, PANEL_BGCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_BGCOLOR);
	TFT->setTextDatum(TR_DATUM);

	TFT->drawString(GetSpeedString(dSpeed), 312, 52);
	*/
}

String HS_HomeScreen_B::GetSpeedString(double speed)
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