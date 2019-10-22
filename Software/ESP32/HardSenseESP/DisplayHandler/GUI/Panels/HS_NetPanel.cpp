#include "HS_NetPanel.h"

HS_NetPanel::HS_NetPanel() : HS_PanelBase()
{
}


HS_NetPanel::~HS_NetPanel()
{
}

void HS_NetPanel::DrawPanel()
{
	//DrawBoxWithBorderAndDropShadow(TFT, x, y, 113, 68, BOX_BORDER_COLOR, PANEL_BGCOLOR, BOX_DROP_SHADOW);
	//HS_Coords tmpCoords = coords;
	//tmpCoords.w = 113;
	//tmpCoords.h = 68;
	//DrawBoxWithBorderAndDropShadow(TFT, tmpCoords, theme);
	DrawBoxWithBorderAndDropShadow(*TFT, coords, theme);

	TFT->fillRect(coords.x + 3, coords.y + 3, 107, 17, theme.panelHeaderColor);
	TFT->setTextColor(TFT_WHITE, theme.panelHeaderColor);
	TFT->setTextDatum(MC_DATUM);
	TFT->drawString("NET", coords.x + 59, coords.y + 13);
	TFT->drawFastHLine(coords.x + 3, coords.y + 20, 107, theme.panelBorderColor);
	TFT->drawFastHLine(coords.x + 3, coords.y + 21, 107, theme.panelBorderColor);

	TFT->drawFastHLine(coords.x + 3, coords.y + 43, 107, theme.panelBorderColor);
	TFT->drawFastHLine(coords.x + 3, coords.y + 44, 107, theme.panelBorderColor);

	TFT->drawTriangle(coords.x + 14, coords.y + 27, coords.x + 21, coords.y + 38, coords.x + 7, coords.y + 38, TFT_GREEN);
	TFT->drawTriangle(coords.x + 14, coords.y + 60, coords.x + 21, coords.y + 49, coords.x + 7, coords.y + 49, TFT_RED);

	textPrinter_Sprite->setTextColor(TFT_WHITE, theme.panelBGColor);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	Update_Net_UpLoadSpeed(0.0);
	Update_Net_DownloadSpeed(0.0);
}

void HS_NetPanel::Update_Net_UpLoadSpeed(double uSpeed)
{
	textPrinter_Sprite->createSprite(81, 16);
	textPrinter_Sprite->fillSprite(theme.panelBGColor);
	textPrinter_Sprite->drawString(GetSpeedString(uSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(coords.x + 26, coords.y + 25);
	textPrinter_Sprite->deleteSprite();
}

void HS_NetPanel::Update_Net_DownloadSpeed(double dSpeed)
{
	textPrinter_Sprite->createSprite(81, 16);
	textPrinter_Sprite->fillSprite(theme.panelBGColor);
	textPrinter_Sprite->drawString(GetSpeedString(dSpeed), 81, 0);
	textPrinter_Sprite->pushSprite(coords.x + 26, coords.y + 48);
	textPrinter_Sprite->deleteSprite();
}

String HS_NetPanel::GetSpeedString(double speed)
{
	char buf[32];
	if (speed > 1000)
	{
		speed = speed / 1000;
		sprintf(buf, "%.1f MBs", speed);
	}
	else
	{
		sprintf(buf, "%.1f KBs", speed);
	}

	return String(buf);
}