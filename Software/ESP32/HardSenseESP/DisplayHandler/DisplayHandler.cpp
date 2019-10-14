#include "DisplayHandler.h"


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
	UpdateScreen = &DisplayHandler::DrawWelcomeScreen;

	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
}

void DisplayHandler::UpdateDisplay()
{
	while (!displayDataQueue->isEmpty())
	{
		QUEUE_ITEM currItem = displayDataQueue->dequeue();
		switch (currItem.key) {
		case 'a':
			break;
		default:
			break;
		}
	}

	(this->*UpdateScreen)();
}

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

