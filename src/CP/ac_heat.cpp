
#include "ac_heat.h"
#include "debug_var.h"
void Heater::Off()
{
	if(!isOn()){
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gHeaterStopStartGap) && isOn()) {
		LOG_AC("电加热上次启动到现在时间间隔不足10s\n");
		return;
	}
	
	
	timer.Stop();
	DO_CLR(DO_run);

	
	
}

//未完成
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
		LOG_AC("电加热上次关闭到现在时间间隔不足10s\n");
		return;
	}
	DO_SET(DO_run);
	timer.Start();
}

