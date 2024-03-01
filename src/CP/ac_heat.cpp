
#include "ac_heat.h"
#include "debug_var.h"
void Heater::Off()
{
	if (!isOn())
	{
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gHeaterStopStartGap) && isOn())
	{
		LOG_AC("������ϴ�����������ʱ��������10s\n");
		return;
	}

	timer.Stop();
	DO_CLR(DO_run);
}

// δ���
BOOL Heater::checkError()
{

	return FALSE;
}

BOOL Heater::isOn()
{
	return DO_STAT(DO_run);
}

BOOL Heater::isRun()
{
	if (g_debug_var.bit.feedback_ignore)
	{
		return isOn();
	}
	return DI_STAT(DI_feedback);
}

BOOL Heater::isRunErr()
{
	return isOn() && !isRun();
}

void Heater::On()
{
	if (isOn())
	{
		return;
	}
	if ((TimeGap(timer.getOffTime()) < gHeaterStopStartGap) && !isOn())
	{
		LOG_AC("������ϴιرյ�����ʱ��������10s\n");
		return;
	}
	DO_SET(DO_run);
	timer.Start();
}

BOOL Heater::getErr()
{
	Error_Flag &= 0x1;

	if ((DI_STAT(DI_HEATER1_Feedback) == 0 && DO_STAT(DO_HEATER1_Contactor) == 1 && HeaterNumber == 1) || (DI_STAT(DI_HEATER2_Feedback) == 0 && DO_STAT(DO_HEATER2_Contactor) == 1 && HeaterNumber == 2))
	{
		Error_Flag |= HeaterContactorError;
	}
	else
	{
		Error_Flag &= (~HeaterContactorError);
	}

	LOG_AC("\n�����%d����,ԭ��%d,������%d\n", HeaterNumber, Error_Flag, FaultTimer.getOnCnt());
	return Error_Flag | 0x0;
}
