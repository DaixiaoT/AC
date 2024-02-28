
#include "ac_heat.h"
#include "debug_var.h"
void Heater::Off()
{
	if(!isOn()){
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gHeaterStopStartGap) && isOn()) {
		LOG_AC("������ϴ�����������ʱ��������10s\n");
		return;
	}
	
	
	timer.Stop();
	DO_CLR(DO_run);

	
	
}

//δ���
BOOL Heater::checkError() {

	return FALSE;
}

BOOL Heater::isOn()
{
	return DO_STAT(DO_run);
}

BOOL Heater::isRun()
{
	if (g_debug_var.bit.feedback_ignore) {
		return isOn();
	}
	return DI_STAT(DI_feedback);

}

BOOL Heater::isRunErr() {
	return isOn() && !isRun()  ;
}

void Heater::On()
{
	if(isOn()){
		return;
	}
	if ((TimeGap(timer.getOffTime()) < gHeaterStopStartGap) && !isOn()) {
		LOG_AC("������ϴιرյ�����ʱ��������10s\n");
		return;
	}
	DO_SET(DO_run);
	timer.Start();
}

