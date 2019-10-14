#include "DisplayHandler.h"
#include "../HSSerial/SerialInterface.h"
#include "GUI/SplashScreen.h"
#include "GUI/Screen_ConnectToNetwork.h"
#include "GUI/Screen_HomeScreen.h"


DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(DataQueue<QUEUE_ITEM>* newQueue, void(*AddItemToOutputQueue_Func)(char key, char* value))
{
	displayDataQueue = newQueue;
	AddItemToOutputQueue = AddItemToOutputQueue_Func;

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
		delay(20);
	}

}

void DisplayHandler::LoadNewScreen(String screenID)
{
	Serial.println("LoadNewScreen");
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen(tftDisplay);
		DestoryCurrentScreen = NULL;
		UpdateCureentScreen = NULL;
	}

	char key = screenID.charAt(0);
	switch (key) {
	case ScreenTypes::SplashScreen:
		DestoryCurrentScreen = Destroy_SplashScreen;
		UpdateCureentScreen = Update_SplashScreen;
		Create_SplashScreen(tftDisplay);
		break;
	case ScreenTypes::ConnectToNetwork:
		DestoryCurrentScreen = Destroy_Screen_ConnectToNetwork;
		UpdateCureentScreen = Update_Screen_ConnectToNetwork;
		Create_Screen_ConnectToNetwork(tftDisplay);
		break;
	case ScreenTypes::Home:
		Serial.println("LoadNewScreen:: HOME");
		DestoryCurrentScreen = Destroy_Screen_Home;
		UpdateCureentScreen = Update_Screen_Home;
		Create_Screen_Home(tftDisplay);
		AddItemToOutputQueue(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, Screen_Home_SensorList());
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
			//Serial.printf("DisplayHandler::DispatchCommand()");
			if (UpdateCureentScreen != NULL) {
				//Serial.printf("Updateing Screen with sensor data: '%s'\n",currItem.value);
				UpdateCureentScreen(tftDisplay, currItem.value);
			}
			break;
		default:
			break;
		}
	}
}