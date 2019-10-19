#include "HS_HomeScreen_B.h"


HS_HomeScreen_B::HS_HomeScreen_B(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);
	//gpuLoadWidget = new HS_Dial_Widget(*TFT);

	TFT->fillScreen(TFT_ORANGE);

	Draw_CPU_Panel();
	//Draw_GPU_Panel();
	//Draw_Net_Panel();
	//Draw_Ram_Panel();

	//Serial.println("Ending Create_Screen_Home() !!!");
}

HS_HomeScreen_B::~HS_HomeScreen_B()
{
	delete(cpuLoadWidget);
	//delete(gpuLoadWidget);
}


void HS_HomeScreen_B::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");
	/*
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c|/intelcpu/0/power/0,d");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e|/nvidiagpu/0/temperature/0,f|/nvidiagpu/0/clock/0,g|/nvidiagpu/0/control/0,h");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k");
	*/
}
void HS_HomeScreen_B::UpdateScreen(String value)
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