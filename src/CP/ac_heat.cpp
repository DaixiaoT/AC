
#include "ac_heat.h"

void Heater::Off()
{
	DO_CLR(DO_run);
	this->lastOffTime = sys_time();
	totalRunTime += (lastOffTime - lastOnTime);
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
	return isOn() && !isRun() && TimeGap(this->lastOnTime)>3000 ;
}

void Heater::On()
{
	DO_SET(DO_run);
	lastOnTime = sys_time();
}
U32 Heater::getTotalRunTime() {
	return this->totalRunTime;
}
void Heater::setTotalRunTime(U32 tempTotalRunTime) {
	this->totalRunTime = tempTotalRunTime;
}
