#pragma once
#include "HS_PanelBase.h"

class HS_NetPanel : public HS_PanelBase
{
private:
	int textSpriteH;
	int textSpriteW;
	String font = AA_FONT_18PT;
	int uploadFieldY;
	int downloadFieldY;
	void Update_Field(double speed, int yOffset);

public:
	HS_NetPanel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme);
	~HS_NetPanel();

	void DrawPanel(bool small = false);
	void Update_Net_UpLoadSpeed(double uSpeed);
	void Update_Net_DownloadSpeed(double dSpeed);
	String GetSpeedString(double speed);
};

