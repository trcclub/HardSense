#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>


TFT_eSprite *dial;
TFT_eSprite *needle;

void Create_SplashScreen(TFT_eSPI TFT) {
	Serial.println("CreateSplashScreen() !!!");
	TFT.fillScreen(TFT_GREEN);
	TFT.setCursor(40, 40);
	TFT.setTextSize(2);
	TFT.print("Splash Screen");

	dial = new TFT_eSprite(&TFT);
	needle = new TFT_eSprite(&TFT);

	//delay(2000);
}

void Update_SplashScreen(TFT_eSPI TFT, char* value)
{

}

void Destroy_SplashScreen(TFT_eSPI TFT) {
	Serial.println("DestroySplashScreen()");

	delete(dial);
	delete(needle);
}

