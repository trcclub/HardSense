#include "DisplayHandler.h"
#include "../HSSerial/SerialInterface.h"

#include "GUI/HomeScreen_Functions.h"
#include "GUI//SplashScreen_Functions.h"
#include "GUI//ConnectToNetworkScreen_Functions.h"
#include "GUI/BluetoothConfiguratorScreen_Functions.h"


DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, char* value))
{
	displayDataQueue = newDisplayQueue;
	AddItemToOutputQueue = AddItemToOutputQueue_Func;
	displayQueueMux = newDisplayQueueMux;

	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
}

void DisplayHandler::Run()
{

	unsigned long last = millis();
	while (true)
	{
		DispatchCommand();

		/*
		if (millis() - last > 1000)
		{
			Serial.println(ESP.getFreeHeap());
			last = millis();
		}
		*/

		//HandleButtons();
		yield();
		delay(20);

	}

}

/*
void DisplayHandler::GetSensorData(char* (*sensorFunc)())
{
	char buf[MAX_QUEUE_ITEM_VALUE_SIZE];
	Serial.println("\n--------\nDisplayHandler::LoadNewScreen:  Getting Home Screen sensors: ");
	char* list = sensorFunc();
	Serial.println("1");
	strcpy(buf, list);
	Serial.println("2");
	delete(list);
	Serial.println("DisplayHandler::LoadNewScreen:  Adding Home Screen sensors: ");
	Serial.println(buf);
	AddItemToOutputQueue(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, buf);
	Serial.print("DisplayHandler::LoadNewScreen:  Sensors added: \n-----------");

}
*/

void DisplayHandler::LoadNewScreen(char screenID)
{
	//Serial.println("LoadNewScreen");
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen();
		DestoryCurrentScreen = NULL;
		UpdateCureentScreen = NULL;
	}
	AddItemToOutputQueue(TRANS__KEY::CLEAR_SENSOR_LIST, "");

	char key = screenID;
	switch (key) {
	case ScreenTypes::SplashScreen:
		DestoryCurrentScreen = Destroy_SplashScreen;
		UpdateCureentScreen = Update_SplashScreen;
		Create_SplashScreen(&tftDisplay);
		break;
	case ScreenTypes::ConnectToNetwork:
		DestoryCurrentScreen = Destroy_ConnectToNetworkScreen;
		UpdateCureentScreen = Update_ConnectToNetworkScreen;
		Create_ConnectToNetworkScreen(&tftDisplay);
		break;
	case ScreenTypes::BluetoothConfigurator:
		DestoryCurrentScreen = Destroy_BluetoothConfiguratorScreen;
		UpdateCureentScreen = Update_BluetoothConfiguratorScreen;
		Create_BluetoothConfiguratorScreen(&tftDisplay);
		break;
	case ScreenTypes::Home:
		DestoryCurrentScreen = Destroy_HomeScreen;
		UpdateCureentScreen = Update_HomeScreen;
		Create_HomeScreen(&tftDisplay);
		//GetSensorData(Home_Screen_SensorList);
		Set_Home_Screen_SensorList(AddItemToOutputQueue);
		break;
	default:
		break;
	}


	/*
	//Serial.println("LoadNewScreen");
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen(tftDisplay);
		DestoryCurrentScreen = NULL;
		UpdateCureentScreen = NULL;
	}
	AddItemToOutputQueue(TRANS__KEY::CLEAR_SENSOR_LIST, "");

	char key = screenID;
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
		DestoryCurrentScreen = Destroy_Screen_Home;
		UpdateCureentScreen = Update_Screen_Home;
		Create_Screen_Home(tftDisplay);
		AddItemToOutputQueue(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, Screen_Home_SensorList());
		break;
	case ScreenTypes::BluetoothConfigurator:
		DestoryCurrentScreen = Destroy_Screen_BluetoothConfigurator;
		UpdateCureentScreen = Update_Screen_BluetoothConfigurator;
		Create_Screen_BluetoothConfigurator(tftDisplay);
		break;
	default:
		break;
	}
	*/
}

void DisplayHandler::DispatchCommand()
{
	while (!displayDataQueue->isEmpty())
	{
		Serial.println("DisplayHandler::DispatchCommand 1");
		portENTER_CRITICAL(&displayQueueMux);
		QUEUE_ITEM currItem = displayDataQueue->dequeue();
		portEXIT_CRITICAL(&displayQueueMux);

		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			LoadNewScreen(currItem.value[0]);
			break;
		case DisplayCommands::UpdateValue:
			if (UpdateCureentScreen != NULL) {
				UpdateCureentScreen(currItem.value);
			}
			break;
		default:
			break;
		}
		yield();
		Serial.println("DisplayHandler::DispatchCommand 2");
	}
}