#include "ac_fan.h"
#include "debug_var.h"

BOOL Fan::isOn()
{
	return DO_STAT(DO_run);
}

void Fan::On()
{	
	if(isOn()){
		return;
	}
	if ((TimeGap(timer.getOffTime()) < gFanStopStartGap) && !isOn()) {
		LOG_AC("风机上次关闭到现在时间间隔不足10s\n");
		return;
	}
	DO_SET(DO_run);
	timer.Start();
}

void Fan::Off(BOOL Force)
{
	if(!isOn()){
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gFanStartStopGap)&& isOn()) {
		LOG_AC("风机上次启动到现在时间间隔不足10s\n");
		return;
	}
	setRunStatus(FALSE);
	
	timer.Stop();


	DO_CLR(DO_run);
	
}

BOOL Fan::isRun()
{
	if (g_debug_var.bit.feedback_ignore) {
		return isOn();
	}
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
