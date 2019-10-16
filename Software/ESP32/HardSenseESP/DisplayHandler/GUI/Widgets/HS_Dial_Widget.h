#pragma once
#include "HS_WidgetBase.h"

class HS_Dial_Widget : HS_WidgetBase
{
private:
	TFT_eSprite* dial;
	TFT_eSprite* needle;
	void CreateNeedle();
	void DrawEmptyDial(String label, float val);

public:
	HS_Dial_Widget(TFT_eSPI TFT);
	~HS_Dial_Widget();
	void DrawDialScale(TFT_eSPI TFT, int16_t start_angle, int16_t end_angle, int16_t increment, uint16_t ringColor);
	void PlotDial(int16_t x, int16_t y, int16_t angle, String label, float val);
};

