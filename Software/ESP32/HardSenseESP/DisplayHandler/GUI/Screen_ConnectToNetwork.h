#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>


void Create_Screen_ConnectToNetwork(TFT_eSPI TFT) {
	Serial.println("Create_Screen_ConnectToNetwork() !!!");
	TFT.fillScreen(TFT_ORANGE);
	TFT.setCursor(40, 80);
	TFT.setTextSize(2);
	TFT.print("Create_Screen_ConnectToNetwork()");
}

void Update_Screen_ConnectToNetwork(TFT_eSPI TFT, char key, char* value)
{

}

void Destroy_Screen_ConnectToNetwork(TFT_eSPI TFT) {
	Serial.println("Destroy_Screen_ConnectToNetwork()");
}

