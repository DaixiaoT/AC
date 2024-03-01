#include "ac_fan.h"
#include "debug_var.h"

BOOL Fan::isOn()
{
	return DO_STAT(DO_run);
}

void Fan::On()
{
	if (isOn())
	{
		return;
	}
	if ((TimeGap(timer.getOffTime()) < gFanStopStartGap) && !isOn())
	{
		LOG_AC("风机上次关闭到现在时间间隔不足10s\n");
		return;
	}
	DO_SET(DO_run);
	timer.Start();
}

void Fan::Off(BOOL Force)
{
	if (!isOn())
	{
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gFanStartStopGap) && isOn())
	{
		LOG_AC("风机上次启动到现在时间间隔不足10s\n");
		return;
	}
	setRunStatus(FALSE);

	timer.Stop();

	DO_CLR(DO_run);
}

BOOL Fan::isRun()
{
	if (g_debug_var.bit.feedback_ignore)
	{
		return isOn();
	}
	return DI_STAT(DI_feedback);
}

BOOL Fan::getErr()
{
	Error_Flag &= 0x15;
	if ((DI_STAT(DI_SAF1_State) == 0 && FanNumber == 1) || (DI_STAT(DI_SAF2_Feedback) == 0 && FanNumber == 2))
	{
		Error_Flag |= SupplyAirFanFeedbackError;
	}
	else
	{
		Error_Flag &= (~SupplyAirFanFeedbackError);
	}
	if ((DI_STAT(DI_SAF1_Feedback) == 0 && DO_STAT(DO_SAF1_Contactor) == 1 && FanNumber == 1 && Fan_Type == 0) || (DI_STAT(DI_SAF2_Feedback) == 0 && DO_STAT(DO_SAF2_Contactor) == 1 && FanNumber == 2 && Fan_Type == 0))
	{
		Error_Flag |= SupplyAirFanContactorError;
	}
	else
	{
		Error_Flag &= (~SupplyAirFanContactorError);
	}
	if ((DI_STAT(DI_CDF1_Feedback) == 0 && DO_STAT(DO_CDF1_Contactor) == 1 && FanNumber == 1 && Fan_Type == 1) || (DI_STAT(DI_CDF2_Feedback) == 0 && DO_STAT(DO_CDF2_Contactor) == 1 && FanNumber == 2 && Fan_Type == 1))
	{
		Error_Flag |= CondenserFanContactorError;
	}
	else
	{
		Error_Flag &= (~CondenserFanContactorError);
	}

	if ((DI_STAT(DI_CDF1_Redundance_Feedback) == 0 && DO_STAT(DO_CDF1_Redundance) == 1 && FanNumber == 1) || (DI_STAT(DI_CDF2_Redundance_Feedback) == 0 && DO_STAT(DO_CDF2_Redundance) == 1 && FanNumber == 2))
	{
		Error_Flag |= CondenserFanRedundancyContactorError;
	}
	else
	{
		Error_Flag &= (~CondenserFanRedundancyContactorError);
	}
	if (Fan_Type == 0)
	{
		LOG_AC("\n送风机%d故障,原因：%d,次数：%d,时长：%d\n", FanNumber, Error_Flag, FaultTimer.getOnCnt(), FaultTimer.getThisRunTime());
	}
	else
	{
		LOG_AC("\n冷凝风机%d故障,原因：%d,次数：%d,时长：%d\n", FanNumber, Error_Flag, FaultTimer.getOnCnt(), FaultTimer.getThisRunTime());
	}
	return Error_Flag | 0x0;
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
