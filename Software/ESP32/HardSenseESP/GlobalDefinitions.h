#pragma once
#ifndef HS_GLOBAL_DEFINITATIONS
#define HS_GLOBAL_DEFINITATIONS


#define AA_FONT_SMALL "SegoeUI-14"
#define AA_FONT_LARGE "SegoeUI-18"


enum DisplayCommands {
	ChangeScreen = 0,
	UpdateValue,
};

enum ScreenTypes {
	SplashScreen = 0,
	ConnectToNetwork,
	BluetoothConfigurator,
	Home,
	HomeB,
};


#endif // !HS_GLOBAL_DEFINITATIONS