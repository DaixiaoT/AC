
#include "ac_heat.h"

void Heater::Off()
{
	DO_CLR(DO_run);
	timer.Stop();
	
}

//Î´Íê³É
BOOL Heater::checkError() {

	return FALSE;
}

BOOL Heater::isOn()
{
	return DO_STAT(DO_run);
}

BOOL Heater::isRun()
{
	return DI_STAT(DI_feedback);

}

BOOL Heater::isRunErr() {
	return isOn() && !isRun()  ;
}

void Heater::On()
{
	DO_SET(DO_run);
	timer.Start();
}

