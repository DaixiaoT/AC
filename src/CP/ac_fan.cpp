#include "ac_fan.h"




void Fan::On()
{
	DO_SET(DO_run);
	timer.Start();
}

void Fan::Off(BOOL Force)
{
	
	setRunStatus(FALSE);
	DO_CLR(DO_run);
	timer.Stop();
}

BOOL Fan::isRun()
{
	return DI_STAT(DI_feedback);
}

FanError Fan::getErr()
{
	return this->errLock;
}

U16 Fan::getSpeed()
{
	return this->speed;
}

void Fan::setSpeed(U16 tempSpeed)
{
	this->speed = tempSpeed;
}





void Fan::setRunStatus(BOOL tempStatus)
{
	this->runStatus = tempStatus;
}

BOOL Fan::getRunStatus()
{
	return this->runStatus;
}
