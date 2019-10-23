#include "HS_GameScreen.h"


HS_GameScreen::HS_GameScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	gameScreenTheme.panelBGColor = PANEL_BGCOLOR;
	gameScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	gameScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	gameScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	gameScreenTheme.textColor = TEXT_COLOR;

	TFT->loadFont(AA_FONT_14PT);
	TFT->fillScreen(TFT_BLACK);
	
	textPrinter_Sprite->setTextColor(TEXT_COLOR, PANEL_BGCOLOR);

	Draw_Temp_Panel();
	Draw_Net_Panel();
}

HS_GameScreen::~HS_GameScreen()
{
	delete(GPU_TempAndFanChart);
	delete(netPanel);
}


void HS_GameScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{

	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/temperature/0,a|/nvidiagpu/0/control/0,b");

	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a");

	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");

	/*
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,a|/intelcpu/0/temperature/6,b|/intelcpu/0/clock/1,c|/intelcpu/0/power/0,d");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/0,e|/nvidiagpu/0/temperature/0,f|/nvidiagpu/0/clock/0,g|/nvidiagpu/0/control/0,h");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/ram/load/0,k");
	*/
}

void HS_GameScreen::UpdateScreen(String value)
{
	//Serial.println("HS_GameScreen::UpdateScreen 1");

	char key = value.charAt(0);
	String subValue = value.substring(value.indexOf(",") + 1);

	//Serial.println("HS_GameScreen::UpdateScreen 2");

	double dValue = subValue.toDouble();
	//Serial.println("HS_GameScreen::UpdateScreen 3");

	switch (key) {
	case 'a':
		GPU_TempAndFanChart->UpdateGPUTemp(dValue);
		break;
	case 'b':
		GPU_TempAndFanChart->UpdateGPUFanLoad(dValue);
		break;
	case 'c':
		break;
	case 'i':
		netPanel->Update_Net_DownloadSpeed(dValue);
		break;
	case 'j':
		netPanel->Update_Net_UpLoadSpeed(dValue);
		break;
	default:
		break;
	}

	//Serial.println("HS_GameScreen::UpdateScreen 6");
}

void HS_GameScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > gpuTempAndFanGraphUpdateTime)
	{
		GPU_TempAndFanChart->UpdateGraph();
		lastUpdate = millis();
	}
}

void HS_GameScreen::HandleTouch(int x, int y)
{
	if (HiddenHomeScreen_Touched(x,y))
	{
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::Home);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	}
	
}


void HS_GameScreen::Draw_Temp_Panel()
{
	GPU_TempAndFanChart = new HS_GPU_TempAndFanChart_Panel(TFT, HS_Coords(GPU_TEMP_PANEL_X, GPU_TEMP_PANEL_Y, 0, 0), gameScreenTheme);
	GPU_TempAndFanChart->DrawPanel();

	/*
	int x = 0;
	int y = 0;

	DrawBoxWithBorderAndDropShadow(x, y, 226, 129, BOX_BORDER_COLOR, TFT_BLACK, BOX_DROP_SHADOW);

	TFT->fillRect(x + 3, y + 3, 220, 21, PANEL_HCOLOR);
	TFT->setTextColor(TFT_WHITE, PANEL_HCOLOR);
	TFT->setTextDatum(TC_DATUM);

	
	textPrinter_Sprite->createSprite(190, 17);
	textPrinter_Sprite->fillSprite(PANEL_BGCOLOR);
	textPrinter_Sprite->setTextColor(TEXT_COLOR, PANEL_HCOLOR);
	textPrinter_Sprite->drawString("GPU Core Temp & Fan",0,0);
	
	textPrinter_Sprite->pushSprite(x + 25, y + 4);
	
	TFT->drawFastHLine(x + 3, y + 22, 220, BOX_BORDER_COLOR);
	TFT->drawFastHLine(x + 3, y + 23, 220, BOX_BORDER_COLOR);
	

	graphGrid = new TFT_eSprite(TFT);
	graphGrid->setColorDepth(16);
	graphGrid->createSprite(220, 100);
	graphGrid->fillSprite(TFT_TRANSPARENT);
	graphGrid->drawRect(19, 0, 180, 100, TFT_DARKGREY);

	//Draw horizontal grid lines
	for (int i = 10; i < 100; i += 10) {
		graphGrid->drawFastHLine(x+19, i, 180, TFT_DARKGREY);
	}

	//Draw vertical grid lines
	for (int i = 20; i < 180; i += 20) {
		graphGrid->drawFastVLine(i+19, 0, 100, TFT_DARKGREY);
	}
	graphGrid->loadFont(AA_FONT_12PT);
	graphGrid->setTextColor(TFT_YELLOW, TFT_BLACK);
	graphGrid->setTextDatum(TL_DATUM);
	graphGrid->drawString("80", 201, 0);
	graphGrid->setTextDatum(ML_DATUM);
	graphGrid->drawString("40", 201, 50);
	graphGrid->setTextDatum(BL_DATUM);
	graphGrid->drawString("20", 201, 100);

	graphGrid->setTextColor(TFT_GREEN, TFT_BLACK);
	graphGrid->setTextDatum(TR_DATUM);
	graphGrid->drawString("100", 18, 0);
	graphGrid->setTextDatum(MR_DATUM);
	graphGrid->drawString("50", 18, 50);
	graphGrid->setTextDatum(BR_DATUM);
	graphGrid->drawString("0", 18, 100);

	graphGrid->pushSprite(x + 3, y + 26,TFT_TRANSPARENT);
	graphGrid->unloadFont();
	//graphGrid->deleteSprite();


	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->deleteSprite();
*/
}

void HS_GameScreen::Draw_Net_Panel()
{
	netPanel = new HS_NetPanel(TFT, HS_Coords(NET_PANEL_X, NET_PANEL_Y, 0, 0), gameScreenTheme);
	netPanel->DrawPanel(true);
}