#include "DisplayHandler.h"
#include "FS.h"
#include "SPIFFS.h"
#include "../HSSerial/SerialInterface.h"

#include "GUI/HomeScreen_Functions.h"
#include "GUI//SplashScreen_Functions.h"
#include "GUI/BluetoothConfiguratorScreen_Functions.h"
#include "GUI/GameScreen_Functions.h"
#include "GUI/CpuDetailsScreen_Functions.h"
#include "GUI/OTAScreen_Functions.h"

DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(Queues *newQueues)
{
	allQueues = newQueues;
	tftDisplay.init();

	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
	CalibrateTouch();
}

void DisplayHandler::Run()
{
	int counter = 0;
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

		yield();
	}
}

void DisplayHandler::LoadNewScreen(ScreenTypes screenID)
{
	allQueues->AddItemToOutputQueue(TRANS__KEY::CLEAR_SENSOR_LIST, "");
	
	// Give the output handler enough time to send the clear sensor command.  
	// Otherwise, the next few statement seem to run faster then the output queue.
	// This causes invalid data to get injected into the new screen because the sensor keys get re-used, but for a different sensor.
	//  All though that still seems to happen (sigh) just not as frequently.  So weird.
	delay(10); 
	
	
	if (DestroyCurrentScreen != NULL) {
		DestroyCurrentScreen();
		DestroyCurrentScreen = NULL;
	}
	UpdateCurentScreen = NULL;
	UpdateCurentScreenOnInterval = NULL;
	HandleTouchPoint = NULL;
	UnloadOldDataFromDisplayQueue();


	switch (screenID) {
	case ScreenTypes::SplashScreen:
		DestroyCurrentScreen = Destroy_SplashScreen;
		UpdateCurentScreen = Update_SplashScreen;
		UpdateCurentScreenOnInterval = Update_SplashScreen_OnInterval;
		Create_SplashScreen(allQueues, &tftDisplay);
		break;
	case ScreenTypes::BluetoothConfigurator:
		DestroyCurrentScreen = Destroy_BluetoothConfiguratorScreen;
		UpdateCurentScreen = Update_BluetoothConfiguratorScreen;
		UpdateCurentScreenOnInterval = Update_BluetoothConfigurator_OnInterval;
		HandleTouchPoint = Handle_BluetoothConfigurator_Touch;
		Create_BluetoothConfiguratorScreen(allQueues, &tftDisplay);
		break;
	case ScreenTypes::Home:
		DestroyCurrentScreen = Destroy_HomeScreen;
		UpdateCurentScreen = Update_HomeScreen;
		HandleTouchPoint = Handle_HomeScreen_Touch;
		UpdateCurentScreenOnInterval = Update_HomeScreen_OnInterval;
		Create_HomeScreen(allQueues, &tftDisplay);
		break;
	case ScreenTypes::Game:
		DestroyCurrentScreen = Destroy_GameScreen;
		UpdateCurentScreen = Update_GameScreen;
		HandleTouchPoint = Handle_GameScreen_Touch;
		UpdateCurentScreenOnInterval = Update_GameScreen_OnInterval;
		Create_GameScreen(allQueues, &tftDisplay);
		break;
	case ScreenTypes::CpuDetails:
		DestroyCurrentScreen = Destroy_CpuScreen;
		UpdateCurentScreen = Update_CpuScreen;
		HandleTouchPoint = Handle_CpuScreen_Touch;
		UpdateCurentScreenOnInterval = Update_CpuScreen_OnInterval;
		Create_CpuScreen(allQueues, &tftDisplay);
		break;
	case ScreenTypes::OTA:
		DestroyCurrentScreen = Destroy_OTAScreen;
		UpdateCurentScreen = Update_OTAScreen;
		HandleTouchPoint = Handle_OTAScreen_Touch;
		UpdateCurentScreenOnInterval = Update_OTAScreen_OnInterval;
		Create_OTAScreen(allQueues, &tftDisplay);
		break;
	default:
		break;
	}
}

void DisplayHandler::DispatchCommand()
{
	while (!allQueues->displayQueue.isEmpty())
	{
		portENTER_CRITICAL(&allQueues->displayQueueMux);
		QUEUE_ITEM currItem = allQueues->displayQueue.dequeue();
		portEXIT_CRITICAL(&allQueues->displayQueueMux);

		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			if (currItem.value.length() == 1) 
			{
				LoadNewScreen(static_cast<ScreenTypes>(currItem.value.toInt()));
			}			
			break;
		case DisplayCommands::UpdateValue:
			if (UpdateCurentScreen != NULL && currItem.value.length() >= 3) 
			{
				UpdateCurentScreen(String(currItem.value));
			}
			break;
		default:
			break;
		}
		yield();
	}
}

void DisplayHandler::UnloadOldDataFromDisplayQueue()
{
	while (!allQueues->displayQueue.isEmpty())
	{
		portENTER_CRITICAL(&allQueues->displayQueueMux);
		allQueues->displayQueue.dequeue();
		portEXIT_CRITICAL(&allQueues->displayQueueMux);
	}
}

void DisplayHandler::CalibrateTouch()
{
	uint16_t calibrationData[5];
	uint8_t calDataOK = 0;

	if (!SPIFFS.begin()) {
		Serial.println("DisplayHandler::CalibrateTouch(): SPIFFS initialisation failed!");
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