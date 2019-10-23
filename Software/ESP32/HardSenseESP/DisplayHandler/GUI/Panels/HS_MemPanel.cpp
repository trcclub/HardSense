#include "HS_MemPanel.h"

HS_MemPanel::HS_MemPanel(TFT_eSPI* newTFT, HS_Coords newCoords, HS_Theme newTheme) : HS_PanelBase(newTFT, newCoords, newTheme)
{

}

HS_MemPanel::~HS_MemPanel()
{

}

void HS_MemPanel::DrawPanel(bool small)
{
	if (small)
	{
		textPrinter_Sprite->unloadFont();
		textPrinter_Sprite->loadFont(AA_FONT_14PT);
		coords.w = 93;
		coords.h = 51;
		textSpriteH = 10;
		usedFieldY = 20;
		freeFieldY = 36;
	}
	else {
		coords.w = 113;
		coords.h = 68;
		textSpriteH = 14;
		usedFieldY = 25;
		freeFieldY = 47;
	}

	DrawBoxWithBorderAndDropShadow(coords, theme);
	TFT->fillRect(coords.x + 3, coords.y + 3, coords.w - 6, textSpriteH + 3, theme.panelHeaderColor);

	textPrinter_Sprite->createSprite(45, textSpriteH);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextDatum(TL_DATUM);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelHeaderColor);
	textPrinter_Sprite->drawString("RAM", 0,0);
	textPrinter_Sprite->pushSprite(coords.x + 3, coords.y + 4, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();

	if (small) {
		TFT->drawFastHLine(coords.x + 3, coords.y + 16, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 17, coords.w - 6, theme.panelBorderColor);

		TFT->drawFastHLine(coords.x + 3, coords.y + 32, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 33, coords.w - 6, theme.panelBorderColor);
	}
	else {
		TFT->drawFastHLine(coords.x + 3, coords.y + 20, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 21, coords.w - 6, theme.panelBorderColor);

		TFT->drawFastHLine(coords.x + 3, coords.y + 43, coords.w - 6, theme.panelBorderColor);
		TFT->drawFastHLine(coords.x + 3, coords.y + 44, coords.w - 6, theme.panelBorderColor);
	}


	textPrinter_Sprite->setTextDatum(TL_DATUM);

	textPrinter_Sprite->createSprite(textPrinter_Sprite->textWidth("U"), textSpriteH);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(TFT_RED, theme.panelBGColor);
	textPrinter_Sprite->drawString("U", 0, 0);
	textPrinter_Sprite->pushSprite(coords.x + 5, coords.y + usedFieldY, TFT_TRANSPARENT);

	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(TFT_GREEN, theme.panelBGColor);
	textPrinter_Sprite->drawString("F", 0, 0);
	textPrinter_Sprite->pushSprite(coords.x + 5, coords.y + freeFieldY, TFT_TRANSPARENT);

	textPrinter_Sprite->deleteSprite();


	/*
	TFT->setTextColor(TFT_RED, homeScreenTheme.panelBGColor);
	TFT->drawString("U:", coords.x + 4, y + 25);


	TFT->setTextColor(TFT_GREEN, homeScreenTheme.panelBGColor);
	TFT->drawString("F:", x + 4, y + 47);
	*/


	/*
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
	*/
}

void HS_MemPanel::Update_Mem_Load(double dPercent, bool highPrecision)
{
	char buf[12];
	if (highPrecision) {
		sprintf(buf, "%.2f %%", dPercent);
	}
	else {
		sprintf(buf, "%.1f %%", dPercent);
	}	
	String text(buf);
	int spriteWidth = textPrinter_Sprite->textWidth(text);

	textPrinter_Sprite->createSprite(spriteWidth, textSpriteH);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelHeaderColor);
	textPrinter_Sprite->drawString(text, spriteWidth, 0);
	textPrinter_Sprite->pushSprite((coords.x + coords.w) - (spriteWidth + 5), coords.y + 4, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}

void HS_MemPanel::Update_Mem_Used(double dGB)
{
	char buf[12];
	sprintf(buf, "%.3f GB", dGB);
	String text(buf);
	int textWidth = textPrinter_Sprite->textWidth(text);

	textPrinter_Sprite->createSprite(textWidth, textSpriteH);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelBGColor);
	textPrinter_Sprite->drawString(String(buf), textWidth, 0);
	textPrinter_Sprite->pushSprite((coords.x + coords.w) - (textWidth + 5), coords.y + usedFieldY, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}

void HS_MemPanel::Update_Mem_Free(double dGB)
{
	char buf[12];
	sprintf(buf, "%.3f GB", dGB);
	String text(buf);
	int textWidth = textPrinter_Sprite->textWidth(text);
	textPrinter_Sprite->createSprite(textWidth, textSpriteH);
	textPrinter_Sprite->setTextDatum(TR_DATUM);
	textPrinter_Sprite->fillSprite(TFT_TRANSPARENT);
	textPrinter_Sprite->setTextColor(theme.textColor, theme.panelBGColor);
	textPrinter_Sprite->drawString(String(buf), textWidth, 0);
	textPrinter_Sprite->pushSprite((coords.x + coords.w) - (textWidth + 5), coords.y + freeFieldY, TFT_TRANSPARENT);
	textPrinter_Sprite->deleteSprite();
}