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

/*
char DegreeSymbol_c = 0xB0;
String DegreeSymbol_str(DegreeSymbol_c);
String DegreesC_str = DegreeSymbol_str + "C";

*/
#endif // !HS_GLOBAL_DEFINITATIONS