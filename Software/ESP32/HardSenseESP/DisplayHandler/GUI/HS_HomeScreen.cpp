#include "HS_HomeScreen.h"


HS_HomeScreen::HS_HomeScreen(TFT_eSPI *newTFT) : HS_ScreenBase(newTFT)
{
	Serial.println("Create_Screen_Home() !!!");
	Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;
	cpuLoadWidget = new HS_Dial_Widget(*TFT);

	TFT->fillScreen(TFT_BLACK);

	TFT->fillRect(0, 0, 160, 92, TFT_DARKGREY);
	TFT->fillRect(2, 2, 156, 88, TFT_NAVY);
	cpuLoadWidget->DrawDialScale(*TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_CPU_Total_Load(0.0);

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
		//Update_Screen_Home_Ethernet_Recv(TFT, dValue);
		break;
	case 'b':
		Update_CPU_Total_Load(dValue);
		break;
	default:
		break;
	}

}

char* HS_HomeScreen::GetSensorList()
{
	return "/intelcpu/0/load/0,b";
}

void HS_HomeScreen::Update_CPU_Total_Load(double percentage)
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

	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "CPU", percentage);

}
