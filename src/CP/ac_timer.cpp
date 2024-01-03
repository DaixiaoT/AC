#include "ac_timer.h"

BOOL AC_TIMER::IsOn()
{
	return status;
}
void AC_TIMER::ctrl(BOOL b)
{
	if (b)
		Start();
	else
		Stop();

}
void AC_TIMER::Start()
{
	if (IsOn()) {
		AdjustRunTime();
		return;
	}
	status = TRUE;
	onTime = sys_time();
	onTimeMark = onTime;
	onCnt += 1;
}

void AC_TIMER::Stop()
{
	if (!IsOn()) {
		return;
	}
	status = FALSE;

	if (IsStopClear)
	{
		Clear();
	}
	else
	{
		totalRunTime += (int)(TimeGap(onTimeMark) / 1000);
		thisRunTime += (int)(TimeGap(onTimeMark) / 1000);
	}
	this->offTime = sys_time();
}

void AC_TIMER::AdjustRunTime()
{
	if (!IsOn())
		return;
	int gap = TimeGap(onTimeMark);
	if (gap >= 1000)
	{
		totalRunTime += (int)(gap / 1000);
		thisRunTime += (int)(gap / 1000);
		onTimeMark = sys_time();
	}
}

void AC_TIMER::Clear()
{
	totalRunTime = 0;
	thisRunTime = 0;
	onTime = sys_time();
}

U32 AC_TIMER::getTotalRunTime()
{
	return totalRunTime;
}

U32 AC_TIMER::getThisRunTime()
{
	return thisRunTime;
}

U32 AC_TIMER::getOnCnt()
{
	return onCnt;
}

U32 AC_TIMER::getOffTime()
{
	return this->offTime;
}

U32 AC_TIMER::getOnTime()
{
	return this->onTime;
}

void AC_TIMER::init(const char* tempName, U32 tempThisRunTime, U32 tempTotalRunTime, BOOL tempIsStopClear, U32 tempOnCnt)
{
	status = FALSE;
	totalRunTime = tempTotalRunTime;
	thisRunTime = tempThisRunTime;
	IsStopClear = tempIsStopClear;
	onCnt = tempOnCnt;
}



