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


#endif // !HS_GLOBAL_DEFINITATIONS