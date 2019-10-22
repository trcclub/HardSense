#pragma once
#include "HS_PanelBase.h"

class HS_NetPanel : public HS_PanelBase
{
private:
	

public:
	HS_NetPanel();
	~HS_NetPanel();

	void DrawPanel();
	void Update_Net_UpLoadSpeed(double uSpeed);
	void Update_Net_DownloadSpeed(double dSpeed);
	String GetSpeedString(double speed);
};

