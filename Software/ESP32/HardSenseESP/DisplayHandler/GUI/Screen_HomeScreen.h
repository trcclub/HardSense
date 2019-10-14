#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>

void Create_Screen_Home(TFT_eSPI TFT);
char* Screen_Home_SensorList();
void Update_Screen_Home(TFT_eSPI TFT, char* value);
void Update_Screen_Home_Ethernet_Recv(TFT_eSPI TFT, double value);
void Destroy_Screen_Home(TFT_eSPI TFT);

void Create_Screen_Home(TFT_eSPI TFT) {
	Serial.println("Create_Screen_Home() !!!");
	TFT.fillScreen(TFT_WHITE);

	TFT.setTextSize(2);
	String str = "Home Screen";
	TFT.setCursor((TFT.width()-TFT.textWidth(str))/2, 20);
	TFT.print(str);


	str = "Network Down Speed";
	TFT.setCursor((TFT.width() - TFT.textWidth(str)) / 2, 60);
	TFT.print(str);

	Update_Screen_Home_Ethernet_Recv(TFT, 0.0);
}

char* Screen_Home_SensorList() {
	//return "/Ethernet/0/recv,a|/intelcpu/0/load/0,b";
	return "/Ethernet/0/recv,a";
}

void Update_Screen_Home_Ethernet_Recv(TFT_eSPI TFT, double value)
{
	TFT.fillRect(80, 100, 180, 50, TFT_BLUE);
	TFT.setTextSize(3);

	TFT.setCursor(90, 115);
	if (value > 1000) {
		value = value / 1000;
		TFT.print(value);
		TFT.print(" MBs");
	}
	else {
		TFT.print(value);
		TFT.print(" KBs");
	}
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
		break;
	default:
		break;
	}
}

void Destroy_Screen_Home(TFT_eSPI TFT) {
	Serial.println("Destroy_Screen_ConnectToNetwork()");
}

