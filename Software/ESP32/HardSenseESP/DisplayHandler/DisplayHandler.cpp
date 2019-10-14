#include "DisplayHandler.h"
#include "GUI/SplashScreen.h"
#include "GUI/Screen_ConnectToNetwork.h"

DisplayHandler::DisplayHandler()
{
	counter = 0;
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(DataQueue<QUEUE_ITEM>* newQueue)
{
	displayDataQueue = newQueue;

	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
}

void DisplayHandler::Run()
{
	while (true)
	{
		DispatchCommand();
		//HandleButtons();
		//UpdateDisplay();
		delay(20);
	}

}

void DisplayHandler::LoadNewScreen(String screenID)
{
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen(tftDisplay);
		DestoryCurrentScreen = NULL;
		UpdateCureentScreen = NULL;
	}

	char key = screenID.charAt(0);
	switch (key) {
	case ScreenTypes::SplashScreen:
		Create_SplashScreen(tftDisplay);
		DestoryCurrentScreen = Destroy_SplashScreen;
		UpdateCureentScreen = Update_SplashScreen;
		break;
	case ScreenTypes::ConnectToNetwork:
		Create_Screen_ConnectToNetwork(tftDisplay);
		DestoryCurrentScreen = Destroy_Screen_ConnectToNetwork;
		UpdateCureentScreen = Update_Screen_ConnectToNetwork;
		break;
	default:
		break;
	}
}

void DisplayHandler::DispatchCommand()
{
	while (!displayDataQueue->isEmpty())
	{
		QUEUE_ITEM currItem = displayDataQueue->dequeue();
		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			LoadNewScreen(currItem.value);
			break;
		case DisplayCommands::UpdateValue:
			if (UpdateCureentScreen != NULL) {
				UpdateCureentScreen(tftDisplay, currItem.key, currItem.value);
			}
			break;
		default:
			break;
		}
	}
}



/*
void DisplayHandler::DrawBackground()
{
	tftDisplay.fillScreen(TFT_NAVY);
	tftDisplay.fillRect(6, 6, tftDisplay.width() - 12, tftDisplay.height() - 12, TFT_LIGHTGREY);
	tftDisplay.setTextColor(TFT_BLACK);
	tftDisplay.setTextSize(2);
	String str = "Doin stuff here...";
	tftDisplay.drawString(str, 10, 20);
}

void DisplayHandler::DrawWelcomeScreen()
{
	counter++;
	tftDisplay.fillRect(6, 20, tftDisplay.width() - 12, 40, TFT_LIGHTGREY);
	tftDisplay.setTextSize(2);
	tftDisplay.setCursor(40, 22);
	tftDisplay.setTextColor(TFT_BLACK);
	tftDisplay.print(counter);
	tftDisplay.print(" times in a loop");

}
*/
