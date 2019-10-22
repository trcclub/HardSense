#pragma once
#include "HS_ScreenBase.h"
#include "Widgets/HS_Dial_Widget.h"

/*  Light Blue
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2
*/

#define TEXT_COLOR TFT_WHITE
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2


class HS_GameScreen :
	public HS_ScreenBase
{
private:
	TFT_eSprite* graphGrid = NULL;

	void Draw_Temp_Panel();

public:
	HS_GameScreen(TFT_eSPI* newTFT);
	~HS_GameScreen();
	void UpdateScreen(String value);
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);

};

