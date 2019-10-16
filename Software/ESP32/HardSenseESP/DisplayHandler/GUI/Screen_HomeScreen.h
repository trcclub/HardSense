#pragma once



#include <SPI.h>
#include <TFT_eSPI.h>
#include "Widgets/HS_Dial_Widget.h"

void Create_Screen_Home(TFT_eSPI TFT);
char* Screen_Home_SensorList();
void Update_Screen_Home(TFT_eSPI TFT, char* value);
void Update_Screen_Home_Ethernet_Recv(TFT_eSPI TFT, double value);
void Destroy_Screen_Home(TFT_eSPI TFT);
void Update_Screen_Home_CPU_Total_Load(TFT_eSPI TFT, double percentage);

HS_Dial_Widget* cpuLoadWidget;

#define SCREEN_HOME_CPU_LOAD_DIAL_MIN -120
#define SCREEN_HOME_CPU_LOAD_DIAL_MAX 120
#define SCREEN_HOME_CPU_LOAD_DIAL_X 0
#define SCREEN_HOME_CPU_LOAD_DIAL_Y 0


uint16_t Home_Screen_cpuLoadDial_CurrentRingColor = TFT_GREEN;

void Create_Screen_Home(TFT_eSPI TFT) {
	Serial.println("Create_Screen_Home() !!!");
	cpuLoadWidget = new HS_Dial_Widget(TFT);

	TFT.fillScreen(TFT_BLACK);

	TFT.fillRect(0, 0, 160, 92, TFT_DARKGREY);
	TFT.fillRect(2, 2, 156, 88, TFT_NAVY);
	cpuLoadWidget->DrawDialScale(TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	Update_Screen_Home_CPU_Total_Load(TFT, 0.0);



	/*

	TFT.setTextSize(2);
	TFT.setTextColor(TFT_BLACK, TFT_WHITE);
	String str = "Home Screen";
	TFT.setCursor((TFT.width()-TFT.textWidth(str))/2, 20);
	TFT.print(str);


	str = "Network Down Speed";
	TFT.setCursor((TFT.width() - TFT.textWidth(str)) / 2, 60);
	TFT.print(str);

	Update_Screen_Home_Ethernet_Recv(TFT, 0.0);
	*/
}

void Update_Screen_Home_CPU_Total_Load(TFT_eSPI TFT, double percentage)
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
		cpuLoadWidget->DrawDialScale(TFT, SCREEN_HOME_CPU_LOAD_DIAL_MIN, SCREEN_HOME_CPU_LOAD_DIAL_MAX, 30, Home_Screen_cpuLoadDial_CurrentRingColor);
	}
	cpuLoadWidget->PlotDial(SCREEN_HOME_CPU_LOAD_DIAL_X, SCREEN_HOME_CPU_LOAD_DIAL_Y, angle, "CPU", percentage);
}

void Update_Screen_Home_Ethernet_Recv(TFT_eSPI TFT, double value)
{
	
	String str = "";
	if (value > 1000) {
		value = value / 1000;
		str += String(value) + " MBs";
	}
	else {
		str += String(value) + " KBs";
	}

	//TFT.fillRect(80, 100, 180, 50, TFT_BLUE);
	TFT.fillRoundRect(50, 100, 220, 50, 15, TFT_BLUE);
	TFT.setTextDatum(TR_DATUM);
	TFT.setTextSize(3);
	TFT.setTextColor(TFT_WHITE, TFT_BLUE);
	TFT.drawString(str, 260, 115);//
	
	//TFT.print(str);
	
}


char* Screen_Home_SensorList() {
	//return "/Ethernet/0/recv,a|/intelcpu/0/load/0,b";
	//return "/Ethernet/0/recv,a";
	return "/intelcpu/0/load/0,b";
}

void Update_Screen_Home(TFT_eSPI TFT, char* value)
{

	String strValue(value);

	char key = strValue.charAt(0);
	String subValue = strValue.substring(strValue.indexOf(",") + 1);

	double dValue = subValue.toDouble();

	switch (key) {
	case 'a':
		Update_Screen_Home_Ethernet_Recv(TFT, dValue);
		break;
	case 'b':
		Update_Screen_Home_CPU_Total_Load(TFT, dValue);
		break;
	default:
		break;
	}
}

void Destroy_Screen_Home(TFT_eSPI TFT) {
	Serial.println("Destroy_Screen_ConnectToNetwork()");
	//delete(textPrinter);
	delete(cpuLoadWidget);
}

