#pragma once
#include "HS_PanelBase.h"
class HS_MemPanel :
	public HS_PanelBase
{
private:
	int textSpriteH;
	int usedFieldY;
	int freeFieldY;

public:
	HS_MemPanel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme);
	~HS_MemPanel();

	void DrawPanel(bool small = false);
	void Update_Mem_Load(double dPercent, bool highPrecision = false);
	void Update_Mem_Used(double dGB);
	void Update_Mem_Free(double dGB);

};

