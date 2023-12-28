#include "ac_fan.h"




void Fan::On()
{
	DO_SET(DO_run);
	
}

void Fan::Off(BOOL Force)
{
	if (!isRun()) {
		return;
	}
	if ((!Force) &&(TimeGap(lastOnTime) < gFanStartStopGap))
	{
		return;
	}
	if (getErr() != FanError_Null) {
		DO_CLR(DO_run);
		setOffTime(sys_time());
		return;
	}
	setRunStatus(FALSE);
	DO_CLR(DO_run);
	setOffTime(sys_time());
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

U32 Fan::getOffTime()
{
	return this->lastOffTime;
}

void Fan::setOffTime(U32 tempOffTime)
{
	this->lastOffTime = tempOffTime;
}

void Fan::setRunStatus(BOOL tempStatus)
{
	this->runStatus = tempStatus;
}

BOOL Fan::getRunStatus()
{
	return this->runStatus;
}
