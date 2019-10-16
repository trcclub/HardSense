#include "HS_ScreenBase.h"

HS_ScreenBase::HS_ScreenBase(TFT_eSPI *newTFT)
{
	TFT = newTFT;
}

HS_ScreenBase::~HS_ScreenBase()
{

}

void HS_ScreenBase::UpdateScreen(char* value)
{

}

char* HS_ScreenBase::GetSensorList()
{

}