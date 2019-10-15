#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>



void Create_Screen_BluetoothConfigurator(TFT_eSPI TFT);
void Update_Screen_BluetoothConfigurator(TFT_eSPI TFT, char* value);
void Destroy_Screen_BluetoothConfigurator(TFT_eSPI TFT);


void Create_Screen_BluetoothConfigurator(TFT_eSPI TFT)
{
	Serial.println("Create_Screen_BluetoothConfigurator() !!!");
	TFT.fillScreen(TFT_RED);

	TFT.setTextSize(2);
	String str = "Bluetooth Configurator";
	TFT.setCursor((TFT.width() - TFT.textWidth(str)) / 2, 20);
	TFT.print(str);

}

void Update_Screen_BluetoothConfigurator(TFT_eSPI TFT, char* value)
{

}

void Destroy_Screen_BluetoothConfigurator(TFT_eSPI TFT)
{

}