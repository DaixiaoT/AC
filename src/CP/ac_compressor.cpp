
#include "ac_compressor.h"

void Compressor::setFreq(U16 HZ, BOOL Force)
{
	if (TimeGap(lastSetFrequencyTime) < 1000) {
		//设置频率不足1s
		return;
	}
	if ((HZ == 0) && (isRun()) && (Force != TRUE))
		return;
	if (isRun() && (HZ != 0))
	{
		if ((firstStart == TRUE))
		{ // 锁频3分钟，期间不需要判断高低压传感器进行降频处理
			LOG_PRINT("锁频3分钟，频率40HZ 当前锁频时间=%d\r\n", TimeGap(lastOnTime));
			freq_HZ = 4000;
			HP_lock = FALSE;
			LP_lock = FALSE;
			return;
		}
		if ((HP_Value > 0) && ((HP_Value >= 220) || ((HP_Value >= 200) && (HP_lock == TRUE))) && (Force != TRUE))
		{
			if (HP_lock == FALSE)
				HP_trigger = HP_trigger + 1;
			HP_lock = TRUE;
			if (TimeGap(HP_error_time) < 30 * 1000)
				return;
			if (freq_HZ <= 3000)
				return;
			freq_HZ = freq_HZ - 500;
			HP_error_time = sys_time();
			LP_lock = FALSE;
			return;
		}
		else if ((LP_Value != -100) && (Force != TRUE) && (LP_Value <= 15) || ((Force != TRUE) && (LP_Value != -100) && (LP_Value <= 17) && (LP_lock == TRUE)))
		{

			LOG_PRINT("name：%s LP_Value =%d\r\n", name, LP_Value);
			if (LP_lock == FALSE)
				LP_trigger = LP_trigger + 1;
			LP_lock = TRUE;
			if (TimeGap(LP_error_time) < 60000)
				return;
			if (freq_HZ <= 3000)
				return;
			freq_HZ = freq_HZ - 500;
			LP_error_time = sys_time();
			HP_lock = FALSE;
			return;
		}
	}
	if (HZ != 0) {
		lastSetFrequencyTime = sys_time();
	}
	freq_HZ = HZ;
	HP_lock = FALSE;
	LP_lock = FALSE;
	
	return;
}

U16 Compressor::getFrequency()
{
    return freq_HZ;
}

U16 Compressor::getFreqState()
{
    return freq_State;
}

void Compressor::forceOff()
{

}

BOOL Compressor::isRun()
{
	return DI_STAT(DI_feedback);
}

BOOL Compressor::getErr()
{
	return 0;
}

U32 Compressor::getTotalRunTime()
{
	return this->totalRunTime;
}

void Compressor::setTotalRunTime(U32 tempTotalRunTime)
{
	this->totalRunTime = tempTotalRunTime;
}

void Compressor::Off()
{
    
	//if (TimeGap(lastOnTime) < gCompressorStartStopGap) {
	//	LOG_AC("压缩机上次启动到现在时间间隔不足180s");
	//	//上次启动时间到现在不足180秒
	//	return;
	//}
	setFreq(0, TRUE);
	//if (TimeGap(lastSetFrequencyTime) < 1000) {
	//	//设置频率时间不足1s
	//	return;
	//}
	
	if (DI_STAT(DI_feedback)) {
		lastOffTime = sys_time();
		totalRunTime += lastOffTime - lastOnTime;
	}
	
	DO_CLR(DO_run);
    
}

void Compressor::On()
{
	//压缩机允许启动信号
	
	//压缩机启停时间间隔

	
	if (!DI_STAT(DO_run)) {
		this->lastOnTime = sys_time();
	}
	DO_SET(DO_run);
	


}
