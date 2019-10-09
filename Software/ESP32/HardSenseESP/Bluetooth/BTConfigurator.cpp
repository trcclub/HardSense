#include "BTConfigurator.h"


void BTConfigurator::HandleBluetooth()
{
	btSerial.begin("HardSenseESP");
}
