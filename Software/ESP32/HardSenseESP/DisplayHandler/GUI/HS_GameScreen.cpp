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
	//Draw_Net_Panel();
	Draw_Mem_Panel();
	DrawGPUCoreLoadPanel();
}

HS_GameScreen::~HS_GameScreen()
{
	delete(gpuCoreLoadWidget);
	delete(GPU_TempAndFanChart);
	delete(memPanel);
	//delete(netPanel);
}


void HS_GameScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{

	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,i|/Ethernet/0/send,j");

	// a = GPU Core Temperature
	// b = GPU Fan Load
	// c = GPU Core Load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/temperature/0,a|/nvidiagpu/0/control/0,b|/nvidiagpu/0/load/0,c");

	// d = GPU Memory Total
	// e = GPU Memory Used
	// f = GPU Memory Free
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/smalldata/3,d|/nvidiagpu/0/smalldata/2,e|/nvidiagpu/0/smalldata/1,f");

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
		UpdateGPUCoreLoad(dValue);
		break;
	case 'd':
		gpuMemoryTotal = dValue;
		break;
	case 'e':
		memPanel->Update_Mem_Load((dValue/gpuMemoryTotal)*100, true);
		memPanel->Update_Mem_Used(dValue / 1000);
		break;
	case 'f':
		memPanel->Update_Mem_Free(dValue / 1000);
		break;
	case 'g':
		break;
	case 'h':
		break;
	case 'i':
		break;
	case 'j':
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
}

void HS_GameScreen::Draw_Mem_Panel()
{
	memPanel = new HS_MemPanel(TFT, HS_Coords(RAM_PANEL_X, RAM_PANEL_Y, 0, 0), gameScreenTheme);
	memPanel->DrawPanel(true);
}

void HS_GameScreen::DrawGPUCoreLoadPanel()
{
	DrawBoxWithBorderAndDropShadow(227, 51, 93, 99, gameScreenTheme.panelBorderColor, gameScreenTheme.panelBGColor, gameScreenTheme.panelDropShadowColor);
	gpuCoreLoadWidget = new HS_Dial_Widget(*TFT);
	gpuCoreLoadWidget->DrawDialScale(*TFT, SCREEN_GAME_LOAD_DIAL_MIN, SCREEN_GAME_LOAD_DIAL_MAX, 30, gpuCoreLoadDial_CurrentRingColor);
	UpdateGPUCoreLoad(0.0);
}

void HS_GameScreen::UpdateGPUCoreLoad(double load)
{
	int angle = map(load, 0, 100, SCREEN_GAME_LOAD_DIAL_MIN, SCREEN_GAME_LOAD_DIAL_MAX);

	uint16_t ringColor;
	if (load > 94)
	{
		ringColor = TFT_RED;
	}
	else if (load > 82)
	{
		ringColor = TFT_ORANGE;
	}
	else if (load > 61)
	{
		ringColor = TFT_YELLOW;
	}
	else
	{
		ringColor = TFT_GREEN;
	}
	if (ringColor != gpuCoreLoadDial_CurrentRingColor)
	{
		gpuCoreLoadDial_CurrentRingColor = ringColor;
		gpuCoreLoadWidget->DrawDialScale(*TFT, SCREEN_GAME_LOAD_DIAL_MIN, SCREEN_GAME_LOAD_DIAL_MAX, 30, gpuCoreLoadDial_CurrentRingColor);
	}
	gpuCoreLoadWidget->PlotDial(SCREEN_GAME_GPU_LOAD_DIAL_X, SCREEN_GAME_GPU_LOAD_DIAL_Y, angle, "Load", load, gameScreenTheme.panelDropShadowColor);
}