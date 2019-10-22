#include "HS_NetPanel.h"

HS_NetPanel::HS_NetPanel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme) : HS_PanelBase(newTFT, newCoords, newTheme)
{
}


HS_NetPanel::~HS_NetPanel()
{
}

void HS_NetPanel::DrawPanel(bool small)
{
	if (small)
	{
		textPrinter_Sprite->unloadFont();
		textPrinter_Sprite->loadFont(AA_FONT_14PT);
		coords.w = 93;
		coords.h = 51;
		textSpriteH = 10;
		textSpriteW = 61;
		uploadFieldY = 20;
		downloadFieldY = 36;
	}
	else {
		coords.w = 113;
		coords.h = 68;
		textSpriteH = 14;
		textSpriteW = 81;
		uploadFieldY = 25;
		downloadFieldY = 48;
	}

	DrawBoxWithBorderAndDropShadow(coords, theme);
	TFT->fillRect(coords.x + 3, coords.y + 3, coords.w - 6, textSpriteH + 3, theme.panelHeaderColor);

	textPrinter_Sprite->createSprite(coords.w - 6, textSpriteH);
	textPrinter_Sprite->fillSprite(theme.panelHeaderColor);
	textPrinter_Sprite->setTextDatum(TC_DATUM);
	textPrinter_Sprite->setTextColor(theme.textColor,theme.panelHeaderColor);
	textPrinter_Sprite->drawString("NET",(coords.w - 6)/2,0);
	textPrinter_Sprite->pushSprite(coords.x + 3, coords.y + 4);
	textPrinter_Sprite->deleteSprite();
	
	if (small) {
		TFT->drawFastHLine(coords.x + 3, coords.y + 16, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 17, coords.w - 6, theme.panelBorderColor);

		TFT->drawFastHLine(coords.x + 3, coords.y + 32, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 33, coords.w - 6, theme.panelBorderColor);

		TFT->drawTriangle(coords.x + 14, coords.y + 20, coords.x + 20, coords.y + 29, coords.x + 8, coords.y + 29, TFT_GREEN);
		TFT->drawTriangle(coords.x + 14, coords.y + 45, coords.x + 20, coords.y + 36, coords.x + 8, coords.y + 36, TFT_RED);

	}
	else {
		TFT->drawFastHLine(coords.x + 3, coords.y + 20, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 21, coords.w - 6, theme.panelBorderColor);

		TFT->drawFastHLine(coords.x + 3, coords.y + 43, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 44, coords.w - 6, theme.panelBorderColor);

		TFT->drawTriangle(coords.x + 14, coords.y + 27, coords.x + 21, coords.y + 38, coords.x + 7, coords.y + 38, TFT_GREEN);
		TFT->drawTriangle(coords.x + 14, coords.y + 60, coords.x + 21, coords.y + 49, coords.x + 7, coords.y + 49, TFT_RED);
	}

	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelBGColor);
	Update_Net_UpLoadSpeed(0.0);
	Update_Net_DownloadSpeed(0.0);
}

void HS_NetPanel::Update_Net_UpLoadSpeed(double uSpeed)
{
	Update_Field(uSpeed, uploadFieldY);
}

void HS_NetPanel::Update_Net_DownloadSpeed(double dSpeed)
{
	Update_Field(dSpeed, downloadFieldY);
}

void HS_NetPanel::Update_Field(double speed, int yOffset)
{
	textPrinter_Sprite->createSprite(textSpriteW, textSpriteH);
	textPrinter_Sprite->fillSprite(theme.panelBGColor);
	textPrinter_Sprite->drawString(GetSpeedString(speed), textSpriteW, 0);
	textPrinter_Sprite->pushSprite((coords.x + coords.w)-(textSpriteW+5), coords.y + yOffset);
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