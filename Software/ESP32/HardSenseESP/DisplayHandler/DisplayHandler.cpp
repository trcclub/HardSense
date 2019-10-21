#include "DisplayHandler.h"
#include "FS.h"
#include "SPIFFS.h"
#include "../HSSerial/SerialInterface.h"

#include "GUI/HomeScreen_Functions.h"
#include "GUI//SplashScreen_Functions.h"
#include "GUI//ConnectToNetworkScreen_Functions.h"
#include "GUI/BluetoothConfiguratorScreen_Functions.h"
#include "GUI/HomeScreenB_Functions.h"

DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, String value), void(*AddItemToDisplayQueue_Func)(char key, String value))
{
	displayDataQueue = newDisplayQueue;
	AddItemToOutputQueue = AddItemToOutputQueue_Func;
	displayQueueMux = newDisplayQueueMux;
	AddItemToDisplayQueue = AddItemToDisplayQueue_Func;

	tftDisplay.init();

	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
	SetTouch();
}

void DisplayHandler::Run()
{
	unsigned long last = millis();
	uint16_t x, y;
	while (true)
	{
		if (tftDisplay.getTouch(&x, &y))
		{
			if ((millis() - lastTouch > TOUCH_DEBOUNCE_TIME) && (HandleTouchPoint != NULL))
			{
				HandleTouchPoint(x, y);
				lastTouch = millis();
			}			
		}
		DispatchCommand();

		if (UpdateCurentScreenOnInterval != NULL)
		{
			UpdateCurentScreenOnInterval();
		}

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

void DisplayHandler::LoadNewScreen(char screenID)
{
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen();
		DestoryCurrentScreen = NULL;
		UpdateCurentScreen = NULL;
		UpdateCurentScreenOnInterval = NULL;
		HandleTouchPoint = NULL;
	}
	AddItemToOutputQueue(TRANS__KEY::CLEAR_SENSOR_LIST, "");
	char key = screenID;
	switch (key) {
	case ScreenTypes::SplashScreen:
		DestoryCurrentScreen = Destroy_SplashScreen;
		UpdateCurentScreen = Update_SplashScreen;
		Create_SplashScreen(&tftDisplay);
		break;
	case ScreenTypes::ConnectToNetwork:
		DestoryCurrentScreen = Destroy_ConnectToNetworkScreen;
		UpdateCurentScreen = Update_ConnectToNetworkScreen;
		Create_ConnectToNetworkScreen(&tftDisplay);
		break;
	case ScreenTypes::BluetoothConfigurator:
		DestoryCurrentScreen = Destroy_BluetoothConfiguratorScreen;
		UpdateCurentScreen = Update_BluetoothConfiguratorScreen;
		Create_BluetoothConfiguratorScreen(&tftDisplay);
		break;
	case ScreenTypes::Home:
		DestoryCurrentScreen = Destroy_HomeScreen;
		UpdateCurentScreen = Update_HomeScreen;
		HandleTouchPoint = Handle_HomeScreen_Touch;
		UpdateCurentScreenOnInterval = Update_HomeScreen_OnInterval;
		Create_HomeScreen(&tftDisplay);
		Set_Home_Screen_SensorList(AddItemToOutputQueue);
		Set_HomeScreen_DisplayQueue(AddItemToDisplayQueue);
		break;
	case ScreenTypes::HomeB:
		DestoryCurrentScreen = Destroy_HomeScreenB;
		UpdateCurentScreen = Update_HomeScreenB;
		HandleTouchPoint = Handle_HomeScreenB_Touch;
		Create_HomeScreenB(&tftDisplay);
		Set_Home_ScreenB_SensorList(AddItemToOutputQueue);
		Set_HomeScreenB_DisplayQueue(AddItemToDisplayQueue);
		break;
	default:
		break;
	}
}

void DisplayHandler::DispatchCommand()
{
	while (!displayDataQueue->isEmpty())
	{
		portENTER_CRITICAL(&displayQueueMux);
		QUEUE_ITEM currItem = displayDataQueue->dequeue();
		portEXIT_CRITICAL(&displayQueueMux);

		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			if (currItem.value.length() == 1) 
			{
				LoadNewScreen(currItem.value[0]);
			}			
			break;
		case DisplayCommands::UpdateValue:
			if (UpdateCurentScreen != NULL) {
				if (currItem.value.length() >= 3)
				{
					UpdateCurentScreen(String(currItem.value));
				}
				
			}
			break;
		default:
			break;
		}
		yield();
	}
}

void DisplayHandler::SetTouch()
{
	uint16_t calibrationData[5];
	uint8_t calDataOK = 0;

	if (!SPIFFS.begin()) {
		Serial.println("DisplayHandler::SetTouch(): SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	if (SPIFFS.exists(CALIBRATION_FILE)) {
		File f = SPIFFS.open(CALIBRATION_FILE, "r");
		if (f) {
			if (f.readBytes((char*)calibrationData, 14) == 14)
				calDataOK = 1;
			f.close();
		}
	}
	if (calDataOK) {
		// calibration data valid
		tftDisplay.setTouch(calibrationData);
	}
	else {
		// data not valid. recalibrate
		tftDisplay.calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
		// store data
		File f = SPIFFS.open(CALIBRATION_FILE, "w");
		if (f) {
			f.write((const unsigned char*)calibrationData, 14);
			f.close();
		}
		else {
			Serial.print("Failed to write file:  '");
			Serial.print(f.getWriteError());
			Serial.println("'");
		}
	}

}