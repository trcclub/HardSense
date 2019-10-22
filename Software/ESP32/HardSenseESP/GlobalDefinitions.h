#pragma once
#ifndef HS_GLOBAL_DEFINITATIONS
#define HS_GLOBAL_DEFINITATIONS

#define AA_FONT_10PT "SegoeUI-10"
#define AA_FONT_12PT "SegoeUI-10"
#define AA_FONT_14PT "SegoeUI-14"
#define AA_FONT_18PT "SegoeUI-18"


enum DisplayCommands {
	ChangeScreen = 0,
	UpdateValue,
};

enum ScreenTypes {
	SplashScreen = 0,
	ConnectToNetwork,
	BluetoothConfigurator,
	Home,
	Game,
};


struct HS_Theme {
	uint16_t panelHeaderColor;
	uint16_t panelBGColor;
	uint16_t panelBorderColor;
	uint16_t panelDropShadowColor;
	

	HS_Theme() {
		panelHeaderColor = 0;
		panelBGColor = 0;
		panelBorderColor = 0;
		panelDropShadowColor = 0;
	}
	HS_Theme(uint16_t hColor, uint16_t bgColor, uint16_t bColor, uint16_t dColor) {
		panelHeaderColor = hColor;
		panelBGColor = bgColor;
		panelBorderColor = bColor;
		panelDropShadowColor = dColor;
	}
	HS_Theme(const HS_Theme &orig) {
		panelHeaderColor = orig.panelHeaderColor;
		panelBGColor = orig.panelBGColor;
		panelBorderColor = orig.panelBorderColor;
		panelDropShadowColor = orig.panelDropShadowColor;
	}
};

struct HS_Coords {
	int x;
	int y;
	int w;
	int h;

	HS_Coords() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	HS_Coords(int inX, int inY, int inW, int inH) {
		x = inX;
		y = inY;
		w = inW;
		h = inH;
	}
	HS_Coords(const HS_Coords &orig) {
		x = orig.x;
		y = orig.y;
		w = orig.w;
		h = orig.h;
	}
};

#endif // !HS_GLOBAL_DEFINITATIONS