#include "HS_CPUScreen.h"


HS_CPUScreen::HS_CPUScreen(TFT_eSPI* newTFT) : HS_ScreenBase(newTFT)
{
	cpuScreenTheme.panelBGColor = PANEL_BGCOLOR;
	cpuScreenTheme.panelBorderColor = BOX_BORDER_COLOR;
	cpuScreenTheme.panelDropShadowColor = BOX_DROP_SHADOW;
	cpuScreenTheme.panelHeaderColor = PANEL_HCOLOR;
	cpuScreenTheme.textColor = TEXT_COLOR;


	TFT->loadFont(AA_FONT_14PT);
	TFT->fillScreen(cpuScreenTheme.panelBGColor);

	textPrinter_Sprite->setTextColor(TEXT_COLOR, PANEL_BGCOLOR);
	textPrinter_Sprite->unloadFont();
	textPrinter_Sprite->loadFont(AA_FONT_14PT);

	DrawPackagePanel();
	DrawPackageLoadGrid();
	InitCPUCorePanels();
}

HS_CPUScreen::~HS_CPUScreen()
{
	packageLoadGridSprite->unloadFont();
	packageLoadGridSprite->deleteSprite();
	delete(packageLoadGridSprite);

	packageLoadSprite->unloadFont();
	packageLoadSprite->deleteSprite();
	delete(packageLoadSprite);

	
	cpuCorePanelGridSprite->unloadFont();
	cpuCorePanelGridSprite->deleteSprite();
	delete(cpuCorePanelGridSprite);


	for (int i = 0; i < CPU_NUMBER_OF_CORES; i++)
	{
		cores[i].grid->unloadFont();
		cores[i].grid->deleteSprite();
		delete(cores[i].grid);
	}
	
}


void HS_CPUScreen::SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value))
{
	// a = CPU package power
	// b = CPU cores power
	// c = CPU package temperature
	// d = CPU package total load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/power/0,a|/intelcpu/0/power/1,b|/intelcpu/0/temperature/6,c|/intelcpu/0/load/0,d");

	// e = CPU 1 temperature
	// f = CPU 1 clock
	// g = CPU 1 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/0,e|/intelcpu/0/clock/1,f|/intelcpu/0/load/1,g");

	// h = CPU 2 temperature
	// i = CPU 2 clock
	// j = CPU 2 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/1,h|/intelcpu/0/clock/2,i|/intelcpu/0/load/2,j");

	// k = CPU 3 temperature
	// l = CPU 3 clock
	// m = CPU 3 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/2,k|/intelcpu/0/clock/3,l|/intelcpu/0/load/3,m");

	// n = CPU 4 temperature
	// o = CPU 4 clock
	// p = CPU 4 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/3,n|/intelcpu/0/clock/4,o|/intelcpu/0/load/4,p");

	// q = CPU 5 temperature
	// r = CPU 5 clock
	// s = CPU 5 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/4,q|/intelcpu/0/clock/5,r|/intelcpu/0/load/5,s");

	// t = CPU 6 temperature
	// u = CPU 6 clock
	// v = CPU 6 load
	AddItemToOutputQueue_func(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, "/intelcpu/0/temperature/5,t|/intelcpu/0/clock/6,u|/intelcpu/0/load/6,v");
}


void HS_CPUScreen::UpdateScreen(String value)
{
	String str(value);
	char key = str.charAt(0);
	String subStr = str.substring(str.indexOf(",") + 1);
	double dValue = subStr.toDouble();

	switch (key) {
	case 'a':
		UpdatePackagePower(dValue);
		break;
	case 'b':
		UpdatePackageCoresPower(dValue);
		break;
	case 'c':
		UpdatePackageTemp(dValue);
		break;
	case 'd':
		packageLoad = dValue;
		break;
	
		// e = CPU 1 temperature
		// f = CPU 1 clock
		// g = CPU 1 load
	case 'e':
		cores[0].temperature = dValue;
		break;
	case 'f':
		cores[0].clock = dValue;
		break;
	case 'g':
		cores[0].load = dValue;
		break;

		
		// h = CPU 2 temperature
		// i = CPU 2 clock
		// j = CPU 2 load
	case 'h':
		cores[1].temperature = dValue;
		break;
	case 'i':
		cores[1].clock = dValue;
		break;
	case 'j':
		cores[1].load = dValue;
		break;

		// k = CPU 3 temperature
		// l = CPU 3 clock
		// m = CPU 3 load
	case 'k':
		cores[2].temperature = dValue;
		break;
	case 'l':
		cores[2].clock = dValue;
		break;
	case 'm':
		cores[2].load = dValue;
		break;

		// n = CPU 4 temperature
		// o = CPU 4 clock
		// p = CPU 4 load
	case 'n':
		cores[3].temperature = dValue;
		break;
	case 'o':
		cores[3].clock = dValue;
		break;
	case 'p':
		cores[3].load = dValue;
		break;

		// q = CPU 5 temperature
		// r = CPU 5 clock
		// s = CPU 5 load
	case 'q':
		cores[4].temperature = dValue;
		break;
	case 'r':
		cores[4].clock = dValue;
		break;
	case 's':
		cores[4].load = dValue;
		break;

		// t = CPU 6 temperature
		// u = CPU 6 clock
		// v = CPU 6 load
	case 't':
		cores[5].temperature = dValue;
		break;
	case 'u':
		cores[5].clock = dValue;
		break;
	case 'v':
		cores[5].load = dValue;
		break;

	default:
		break;
	}
}


void HS_CPUScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > packageLoadGraphUpdateTime)
	{
		lastUpdate = millis();
		UpdatePackageLoadGraph();


		//cores[0]->UpdateChart();

		for (int i = 0; i < CPU_NUMBER_OF_CORES; i++)
		{
			cpuCorePanelGridSprite->pushSprite(cores[i].localCoords.x, cores[i].localCoords.y);
			UpdateCPUCorePanel(i);
			yield();
		}

		//cpuCorePanelGridSprite->pushSprite(cores[0].coords.x, cores[0].coords.y);
		//cores[0].UpdateChart();
	}
}

void HS_CPUScreen::HandleTouch(int x, int y)
{
	if (HiddenHomeScreen_Touched(x, y))
	{
		char buf[2];
		sprintf(buf, "%c", ScreenTypes::Home);
		AddItemToDisplayQueue(DisplayCommands::ChangeScreen, buf);
	}
}

void HS_CPUScreen::DrawPackagePanel()
{
	HS_Coords localCoords(CPU_PACKAGE_PANEL_X, CPU_PACKAGE_PANEL_Y, 320, 60);

	DrawBoxWithBorderAndDropShadow(localCoords, cpuScreenTheme);

	/*
	Header
	*/
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, 182, 53, cpuScreenTheme.panelHeaderColor);

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(cpuScreenTheme.textColor, cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->createSprite(128, 10);
	textPrinter_Sprite->fillSprite(cpuScreenTheme.panelHeaderColor);

	textPrinter_Sprite->drawString("Intel Core i7-3930K", 0, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 36, localCoords.y + 6);
	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 19, 182, cpuScreenTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, 182, cpuScreenTheme.panelBorderColor);

	TFT->drawFastVLine(localCoords.x + 185, localCoords.y + 3, 56, cpuScreenTheme.panelBorderColor);
	TFT->drawFastVLine(localCoords.x + 186, localCoords.y + 3, 56, cpuScreenTheme.panelBorderColor);

	/*
	Package
	*/
	textPrinter_Sprite->createSprite(60, 14);
	textPrinter_Sprite->fillSprite(cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Package:", 0, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 7, localCoords.y + 25);
	TFT->drawFastVLine(localCoords.x + 66, localCoords.y + 21, 37, cpuScreenTheme.panelBorderColor);
	TFT->drawFastVLine(localCoords.x + 67, localCoords.y + 21, 37, cpuScreenTheme.panelBorderColor);


	textPrinter_Sprite->deleteSprite();
	textPrinter_Sprite->createSprite(45, 14);

	/*
	Cores
	*/
	textPrinter_Sprite->fillSprite(cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Cores:", 0, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 75, localCoords.y + 25);
	TFT->drawFastVLine(localCoords.x + 127, localCoords.y + 21, 37, cpuScreenTheme.panelBorderColor);
	TFT->drawFastVLine(localCoords.x + 128, localCoords.y + 21, 37, cpuScreenTheme.panelBorderColor);

	/*
	Temp
	*/
	textPrinter_Sprite->fillSprite(cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString("Temp:", 0, 0);
	textPrinter_Sprite->pushSprite(localCoords.x + 133, localCoords.y + 25);


	textPrinter_Sprite->deleteSprite();

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 40, 182, cpuScreenTheme.panelBorderColor);

	UpdatePackagePower(0.0);
	UpdatePackageCoresPower(0.0);
	UpdatePackageTemp(0.0);
}

void HS_CPUScreen::UpdatePackagePower(double power)
{
	char buf[10];
	sprintf(buf, "%.1f W", power);
	UpdatePackageField(String(buf), 7, 44);
}

void HS_CPUScreen::UpdatePackageCoresPower(double power)
{
	char buf[10];
	sprintf(buf, "%.1f W", power);
	UpdatePackageField(String(buf), 71, 44);
}

void HS_CPUScreen::UpdatePackageTemp(double temp)
{
	char buf[10];
	sprintf(buf, "%.1f %s ", temp, degreesC_char);
	UpdatePackageField(String(buf), 130, 44);
}

void HS_CPUScreen::UpdatePackageField(String str, int xIn, int yIn)
{
	int x = CPU_PACKAGE_PANEL_X;
	int y = CPU_PACKAGE_PANEL_Y;

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(cpuScreenTheme.textColor, cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->createSprite(53, 10);
	textPrinter_Sprite->fillSprite(cpuScreenTheme.panelHeaderColor);
	textPrinter_Sprite->drawString(str, 53, 0);
	textPrinter_Sprite->pushSprite(x + xIn, y + yIn);
	textPrinter_Sprite->deleteSprite();
}

void HS_CPUScreen::DrawPackageLoadGrid()
{
	
	HS_Coords localCoords(CPU_PACKAGE_LOAD_GRID_X, CPU_PACKAGE_LOAD_GRID_Y, 134, 60);

	packageLoadGridSprite = new TFT_eSprite(TFT);
	packageLoadGridSprite->setColorDepth(16);
	packageLoadGridSprite->createSprite(localCoords.w - 6, localCoords.h - 6);
	packageLoadGridSprite->fillSprite(cpuScreenTheme.panelBGColor);

	packageLoadSprite = new TFT_eSprite(TFT);
	packageLoadSprite->setColorDepth(8);
	packageLoadSprite->createSprite(localCoords.w - 6, localCoords.h - 6);
	packageLoadSprite->setTextWrap(false);
	packageLoadSprite->fillScreen(TFT_TRANSPARENT);


	packageLoadGridSprite->drawFastHLine(0, 13, localCoords.w - 6, cpuScreenTheme.panelBorderColor);
	packageLoadGridSprite->drawFastHLine(0, 26, localCoords.w - 6, cpuScreenTheme.panelBorderColor);
	packageLoadGridSprite->drawFastHLine(0, 39, localCoords.w - 6, cpuScreenTheme.panelBorderColor);

	for (int x = 16; x < localCoords.w - 6; x += 16)
	{
		packageLoadGridSprite->drawFastVLine(x, 0, localCoords.h - 6,cpuScreenTheme.panelBorderColor);
	}

	UpdatePackageLoadGraph();
	
}

void HS_CPUScreen::UpdatePackageLoadGraph()
{
	HS_Coords localCoords(CPU_PACKAGE_LOAD_GRID_X, CPU_PACKAGE_LOAD_GRID_Y, 134, 60);
	int pixel = map(long(packageLoad), 0, 100, 1, packageLoadSprite->height()-1);

	packageLoadSprite->scroll(-1, 0);
	packageLoadSprite->drawFastVLine(packageLoadSprite->width()-1, 0, packageLoadSprite->height(), TFT_TRANSPARENT);

	packageLoadSprite->drawFastVLine(packageLoadSprite->width() - 1, (packageLoadSprite->height()-pixel), 2, TFT_YELLOW);
	

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(TERTIARY_TEXT_COLOR, cpuScreenTheme.panelBGColor);
	textPrinter_Sprite->createSprite(50, 10);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	char buf[12];
	sprintf(buf, "%.1f %%", packageLoad);
	textPrinter_Sprite->drawString(String(buf), 50, 0);


	packageLoadGridSprite->pushSprite(localCoords.x + 3, localCoords.y + 3);
	packageLoadSprite->pushSprite(localCoords.x + 3, localCoords.y + 2, TFT_TRANSPARENT);

	textPrinter_Sprite->pushSprite(localCoords.x + 11, localCoords.y + 19, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}

/*
void HS_CPUScreen::InitCPUCorePanels()
{
	cpuCorePanelGridSprite = new TFT_eSprite(TFT);
	cpuCorePanelGridSprite->setColorDepth(16);
	cpuCorePanelGridSprite->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	cpuCorePanelGridSprite->fillSprite(cpuScreenTheme.panelBGColor);

	cpuCorePanelGridSprite->drawFastHLine(0, 30, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);
	cpuCorePanelGridSprite->drawFastHLine(0, 60, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);
	cpuCorePanelGridSprite->drawFastHLine(0, 90, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);

	for (int x = 10; x < CPU_CORE_GRID_WIDTH; x += 10)
	{
		cpuCorePanelGridSprite->drawFastVLine(x, 0, CPU_CORE_GRID_HEIGHT, cpuScreenTheme.panelBorderColor);
	}

	for (int i = 0; i < CPU_NUMBER_OF_CORES; i++)
	{
		DisplayFreeHeap();
		CreateCPUCorePanel(i);
		yield();
	}
}

void HS_CPUScreen::CreateCPUCorePanel(int core)
{
	HS_Coords localCoords(CPU_CORE_PANEL_START_X+3, CPU_CORE_PANEL_START_Y+4, CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);

	if (core > 2)
	{
		localCoords.y += CPU_CORE_PANEL_HEIGHT;
		localCoords.x += ((core - 3) * CPU_CORE_PANEL_WIDTH);
	}
	else {
		localCoords.x += ((core)*CPU_CORE_PANEL_WIDTH);
	}

	DrawBoxWithBorderAndDropShadow(HS_Coords(localCoords.x - 3, localCoords.y - 3, CPU_CORE_PANEL_WIDTH, CPU_CORE_PANEL_HEIGHT), cpuScreenTheme);
	cpuCorePanelGridSprite->pushSprite(localCoords.x, localCoords.y);
	cores[core] = new HS_CPU_Core_Panel(TFT, localCoords, cpuScreenTheme);
}
*/

void HS_CPUScreen::InitCPUCorePanels()
{
	cpuCorePanelGridSprite = new TFT_eSprite(TFT);
	cpuCorePanelGridSprite->setColorDepth(16);
	cpuCorePanelGridSprite->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	cpuCorePanelGridSprite->fillSprite(cpuScreenTheme.panelBGColor);

	cpuCorePanelGridSprite->drawFastHLine(0, 22, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);
	cpuCorePanelGridSprite->drawFastHLine(0, 45, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);
	cpuCorePanelGridSprite->drawFastHLine(0, 67, CPU_CORE_GRID_WIDTH, cpuScreenTheme.panelBorderColor);

	for (int x = 10; x < CPU_CORE_GRID_WIDTH; x += 10)
	{
		cpuCorePanelGridSprite->drawFastVLine(x, 0, CPU_CORE_GRID_HEIGHT, cpuScreenTheme.panelBorderColor);
	}

	for (int i = 0; i < CPU_NUMBER_OF_CORES; i++)
	{
		CreateCPUCorePanel(i);
	}
}

void HS_CPUScreen::CreateCPUCorePanel(int core)
{
	HS_Coords localCoords(CPU_CORE_PANEL_START_X+3, CPU_CORE_PANEL_START_Y+3, CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);

	if (core > 2)
	{
		localCoords.y += CPU_CORE_PANEL_HEIGHT;
		localCoords.x += ((core - 3) * CPU_CORE_PANEL_WIDTH);
	}
	else {
		localCoords.x += ((core) *CPU_CORE_PANEL_WIDTH);
	}
	
	DrawBoxWithBorderAndDropShadow(HS_Coords(localCoords.x-3, localCoords.y-3, CPU_CORE_PANEL_WIDTH, CPU_CORE_PANEL_HEIGHT), cpuScreenTheme);
	cpuCorePanelGridSprite->pushSprite(localCoords.x, localCoords.y);


	cores[core] = CPU_CORE(localCoords);
	cores[core].grid = new TFT_eSprite(TFT);

	cores[core].grid->setColorDepth(8);
	cores[core].grid->createSprite(CPU_CORE_GRID_WIDTH, CPU_CORE_GRID_HEIGHT);
	cores[core].grid->fillSprite(TFT_TRANSPARENT);
}

void HS_CPUScreen::UpdateCPUCorePanel(int core)
{
	if (core < 0 || core >= CPU_NUMBER_OF_CORES) {
		return;
	}

	char buf[12];
	int pixel = map(long(cores[core].load), 0, 100, 1, CPU_CORE_GRID_WIDTH - 1);

	cores[core].grid->scroll(-1, 0);
	cores[core].grid->drawFastVLine(CPU_CORE_GRID_WIDTH - 1, 0, CPU_CORE_GRID_HEIGHT, TFT_TRANSPARENT);
	cores[core].grid->drawFastVLine(CPU_CORE_GRID_WIDTH - 1, (CPU_CORE_GRID_HEIGHT - pixel), 2, TFT_YELLOW);



	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(TERTIARY_TEXT_COLOR, cpuScreenTheme.panelBGColor);
	textPrinter_Sprite->setColorDepth(16);
	textPrinter_Sprite->createSprite(70, 42);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);

	sprintf(buf, "%.0f MHz", cores[core].clock);
	textPrinter_Sprite->drawString(String(buf), 70, 0);

	sprintf(buf, "%.1f %s ", cores[core].temperature, degreesC_char);
	textPrinter_Sprite->drawString(String(buf), 70, 14);

	sprintf(buf, "%.1f %%", cores[core].load);
	textPrinter_Sprite->drawString(String(buf), 70, 28);


	cpuCorePanelGridSprite->pushSprite(cores[core].localCoords.x, cores[core].localCoords.y);
	cores[core].grid->pushSprite(cores[core].localCoords.x, cores[core].localCoords.y, TFT_TRANSPARENT);

	textPrinter_Sprite->pushSprite(cores[core].localCoords.x + 3, cores[core].localCoords.y + 3, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}
