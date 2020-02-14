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
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_14PT);

//	Serial.print("HS_GameScreen: 1: ");
//	Serial.println(ESP.getFreeHeap());

	DrawTempPanel();
//	Serial.print("HS_GameScreen: 2: ");
//	Serial.println(ESP.getFreeHeap());

	DrawMemPanel();
//	Serial.print("HS_GameScreen: 3: ");
//	Serial.println(ESP.getFreeHeap());

	DrawClockSpeedPanel();
//	Serial.print("HS_GameScreen: 4: ");
//	Serial.println(ESP.getFreeHeap());

	DrawGPUCoreLoadPanel();
//	Serial.print("HS_GameScreen: 5: ");
//	Serial.println(ESP.getFreeHeap());

	DrawFPSPanel();
//	Serial.print("HS_GameScreen: 6: ");
//	Serial.println(ESP.getFreeHeap());

	DrawCPULoadPanel();
//	Serial.print("HS_GameScreen: 7: ");
//	Serial.println(ESP.getFreeHeap());

	Draw_Net_Panel();
//	Serial.print("HS_GameScreen: 8: ");
//	Serial.println(ESP.getFreeHeap());
}



HS_GameScreen::~HS_GameScreen()
{
	delete(gpuCoreLoadWidget);
	delete(GPU_TempAndFanChart);
	delete(memPanel);
	delete(netPanel);
}


void HS_GameScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	// a = GPU Core Temperature
	// b = GPU Fan Load
	// c = GPU Core Load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/temperature/0,a|/nvidiagpu/0/control/0,b|/nvidiagpu/0/load/0,c");

	// d = GPU Memory Load
	// e = GPU Memory Used
	// f = GPU Memory Free
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/load/3,d|/nvidiagpu/0/smalldata/2,e|/nvidiagpu/0/smalldata/1,f");

	// g = GPU Core Clock
	// h = GPU Memory Clock
	// i = GPU Shader Clock
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/nvidiagpu/0/clock/0,g|/nvidiagpu/0/clock/1,h|/nvidiagpu/0/clock/2,i");

	// j = FPS Counter
	// k = GPU Mem Load
	//AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/fps/0/counter,j|/nvidiagpu/0/load/1,k");
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/fps/0/counter,j");

	// k = Net download speed
	// l = net upload speed
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/Ethernet/0/recv,k|/Ethernet/0/send,l");

	// m = CPU Load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/load/0,m");
}

void HS_GameScreen::UpdateScreen(String value)
{
	String str(value);
	char key = str.charAt(0);
	String subStr = str.substring(str.indexOf(",") + 1);
	double dValue = subStr.toDouble();

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
		memPanel->Update_Mem_Load(dValue,true);
		break;
	case 'e':
		memPanel->Update_Mem_Used(dValue / 1000);
		break;
	case 'f':
		memPanel->Update_Mem_Free(dValue / 1000);
		break;
	case 'g':
		UpdateGPUCoreClock(dValue);
		break;
	case 'h':
		UpdateGPUMemoryClock(dValue);
		break;
	case 'i':
		UpdateGPUShaderClock(dValue);
		break;
	case 'j':
		UpdateFPS(dValue);
		break;
	case 'k':
		netPanel->Update_Net_DownloadSpeed(dValue);
		break;
	case 'l':
		netPanel->Update_Net_UpLoadSpeed(dValue);
		break;
	case 'm':
		UpdateCPULoad(dValue);
		break;
	default:
		break;
	}
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
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, String(ScreenTypes::Home));
	}
}

void HS_GameScreen::DrawTempPanel()
{
	GPU_TempAndFanChart = new HS_GPU_TempAndFanChart_Panel(TFT, HS_Coords(GPU_TEMP_PANEL_X, GPU_TEMP_PANEL_Y, 0, 0), gameScreenTheme);
	GPU_TempAndFanChart->DrawPanel();
}

void HS_GameScreen::DrawMemPanel()
{
	memPanel = new HS_MemPanel(TFT, HS_Coords(GPU_RAM_PANEL_X, GPU_RAM_PANEL_Y, 0, 0), gameScreenTheme);
	memPanel->DrawPanel(true);
}

void HS_GameScreen::DrawGPUCoreLoadPanel()
{
	gpuCoreLoadWidget = new HS_Dial_Widget(TFT);
	DrawBoxWithBorderAndDropShadow(HS_Coords(GPU_CORE_LOAD_PANEL_X, GPU_CORE_LOAD_PANEL_Y, 93, 99), gameScreenTheme);
	gpuCoreLoadWidget->DrawDialScale(TFT, LOAD_DIAL_MIN, LOAD_DIAL_MAX, 24, gpuCoreLoadDial_CurrentRingColor);
	UpdateGPUCoreLoad(0.0);
}

void HS_GameScreen::UpdateGPUCoreLoad(double load)
{
	int angle = map(load, 0, 100, LOAD_DIAL_MIN, LOAD_DIAL_MAX);

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
		gpuCoreLoadWidget->DrawDialScale(TFT, LOAD_DIAL_MIN, LOAD_DIAL_MAX, 24, gpuCoreLoadDial_CurrentRingColor);
	}
	gpuCoreLoadWidget->PlotDial(GPU_CORE_LOAD_DIAL_X, GPU_CORE_LOAD_DIAL_Y, angle, "Core", load, gameScreenTheme.panelDropShadowColor);
}

void HS_GameScreen::DrawClockSpeedPanel()
{
	HS_Coords coords(GPU_CLOCKS_PANEL_X, GPU_CLOCKS_PANEL_Y, 162, 90);

	DrawBoxWithBorderAndDropShadow(coords, gameScreenTheme);
	TFT->fillRect(coords.x + 3, coords.y + 3, 17, coords.h - 6, gameScreenTheme.panelHeaderColor);
	TFT->drawFastVLine(coords.x + 20, coords.y + 3, coords.h - 6, gameScreenTheme.panelBorderColor);
	TFT->drawFastVLine(coords.x + 21, coords.y + 3, coords.h - 6, gameScreenTheme.panelBorderColor);

	TFT->setTextDatum(TL_DATUM);
	TFT->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelHeaderColor);

	TFT->drawString("C", coords.x + 6, coords.y + 7);
	TFT->drawString("l", coords.x + 7, coords.y + 20);
	TFT->drawString("o", coords.x + 6, coords.y + 33);
	TFT->drawString("c", coords.x + 6, coords.y + 46);
	TFT->drawString("k", coords.x + 6, coords.y + 59);
	TFT->drawString("s", coords.x + 6, coords.y + 72);

	TFT->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelBGColor);
	TFT->drawString("Core", coords.x + 25, coords.y + 9);
	TFT->drawString("Memory", coords.x + 25, coords.y + 38);
	TFT->drawString("Shader", coords.x + 25, coords.y + 67);

	DrawBoxWithBorderAndDropShadow(HS_Coords(coords.x + 80, coords.y + 6, coords.w - 86, 20), gameScreenTheme);
	DrawBoxWithBorderAndDropShadow(HS_Coords(coords.x + 80, coords.y + 35, coords.w - 86, 20), gameScreenTheme);
	DrawBoxWithBorderAndDropShadow(HS_Coords(coords.x + 80, coords.y + 64, coords.w - 86, 20), gameScreenTheme);

	UpdateGPUClockField(0, GPU_CLOCKS_PANEL_CORE_CLOCK_YOFFSET);
	UpdateGPUClockField(0, GPU_CLOCKS_PANEL_MEMORY_CLOCK_YOFFSET);
	UpdateGPUClockField(0, GPU_CLOCKS_PANEL_SHADER_CLOCK_YOFFSET);
}


void HS_GameScreen::UpdateGPUCoreClock(double clock)
{
	UpdateGPUClockField(clock, GPU_CLOCKS_PANEL_CORE_CLOCK_YOFFSET);
}

void HS_GameScreen::UpdateGPUMemoryClock(double clock)
{
	UpdateGPUClockField(clock, GPU_CLOCKS_PANEL_MEMORY_CLOCK_YOFFSET);
}

void HS_GameScreen::UpdateGPUShaderClock(double clock)
{

	UpdateGPUClockField(clock, GPU_CLOCKS_PANEL_SHADER_CLOCK_YOFFSET);
}

void HS_GameScreen::UpdateGPUClockField(double clock, int yOffset)
{
	int x = GPU_CLOCKS_PANEL_X;
	int y = GPU_CLOCKS_PANEL_Y;
	
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->createSprite(67, 10);
	textPrinter_Sprite->fillSprite(gameScreenTheme.panelBGColor);
	char buf[12];
	sprintf(buf, "%.0f MHz", clock);
	textPrinter_Sprite->drawString(String(buf), 80, 0);
	textPrinter_Sprite->pushSprite(x + 83, y + yOffset);
	textPrinter_Sprite->deleteSprite();

}

void HS_GameScreen::DrawFPSPanel()
{
	HS_Coords localCoords(FPS_PANEL_X, FPS_PANEL_Y, 47, 45);
	DrawBoxWithBorderAndDropShadow(localCoords, gameScreenTheme);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->createSprite(localCoords.w - 10, 10);
	textPrinter_Sprite->fillSprite(gameScreenTheme.panelBGColor);
	textPrinter_Sprite->setTextColor(TFT_CYAN, gameScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("FPS", (localCoords.w / 2)-4, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 5, localCoords.y + 7);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelBGColor);

	DrawBoxWithBorderAndDropShadow(HS_Coords(localCoords.x + 5, localCoords.y + 22, localCoords.w - 10, 20), gameScreenTheme);
	UpdateFPS(0.0);
}

void HS_GameScreen::UpdateFPS(double fps)
{
	int x = FPS_PANEL_X;
	int y = FPS_PANEL_Y;
	int w = 27;

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->createSprite(w, 10);
	textPrinter_Sprite->setTextColor(TFT_CYAN, gameScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(gameScreenTheme.panelBGColor);
	textPrinter_Sprite->drawNumber(long(fps), w, 0);
	
	textPrinter_Sprite->pushSprite(x + 8, y + 27);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelBGColor);

}

void HS_GameScreen::Draw_Net_Panel()
{
	netPanel = new HS_NetPanel(TFT, HS_Coords(NET_PANEL_X, NET_PANEL_Y, 0, 0), gameScreenTheme);
	netPanel->DrawPanel();
}

void HS_GameScreen::DrawCPULoadPanel()
{
	HS_Coords localCoords(CPU_LOAD_PANEL_X, CPU_LOAD_PANEL_Y, 47, 45);
	DrawBoxWithBorderAndDropShadow(localCoords, gameScreenTheme);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->createSprite(localCoords.w - 10, 10);
	textPrinter_Sprite->setTextColor(0x3708, gameScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(gameScreenTheme.panelBGColor);
	textPrinter_Sprite->drawString("CPU", (localCoords.w / 2) - 6, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 5, localCoords.y + 7);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelBGColor);

	DrawBoxWithBorderAndDropShadow(HS_Coords(localCoords.x + 5, localCoords.y + 22, localCoords.w - 10, 20), gameScreenTheme);
	UpdateCPULoad(0.0);

}

void HS_GameScreen::UpdateCPULoad(double load)
{
	int x = CPU_LOAD_PANEL_X;
	int y = CPU_LOAD_PANEL_Y;
	int w = 27;

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->createSprite(w, 10);
	textPrinter_Sprite->setTextColor(0x3708, gameScreenTheme.panelBGColor);
	textPrinter_Sprite->fillSprite(gameScreenTheme.panelBGColor);
	textPrinter_Sprite->drawFloat(load,1, w, 0);

	textPrinter_Sprite->pushSprite(x + 8, y + 27);
	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->setTextColor(gameScreenTheme.textColor, gameScreenTheme.panelBGColor);

}