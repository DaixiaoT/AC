
#include "ac_compressor.h"
#include "debug_var.h"
#include "ac_ctrl.h"

void Compressor::setFreq(U16 HZ, BOOL Force)
{
	if (TimeGap(lastSetFrequencyTime) < 1000)
	{
		// 设置频率不足1s
		return;
	}
	if ((HZ == 0) && (isRun()) && (Force != TRUE))
		return;
	if (isRun() && (HZ != 0))
	{
		if ((firstStart == TRUE))
		{
			// 锁频3分钟，期间不需要判断高低压传感器进行降频处理
			// LOG_PRINT("锁频3分钟，频率40HZ 当前锁频时间=%d\r\n", TimeGap(lastOnTime));
			freq_HZ = 4000;
			HP_lock = FALSE;
			LP_lock = FALSE;
			return;
		}
	
	}
	if (HZ != 0)
	{
		lastSetFrequencyTime = sys_time();
	}
	freq_HZ = HZ;

	return;
}

U16 Compressor::getFrequency()
{
	if (force_flag & MD_FREQ_FREQ)
	{
		return freq_HZ_F;
	}
	else
	{
		return freq_HZ;
	}
}

U16 Compressor::getFreqState()
{
	if (force_flag & MD_FREQ_STATE)
	{
		return freq_State_F;
	}
	else
	{
		return freq_State;
	}
}

void Compressor::forceFrequency(BOOL b, U16 HZ)
{
	if (b)
	{
		force_flag |= MD_FREQ_FREQ;
		freq_HZ_F = HZ;
	}
	else
	{
		force_flag &= (~MD_FREQ_FREQ);
	}
}

void Compressor::forceState(BOOL b, U16 state)
{
	if (b)
	{
		force_flag |= MD_FREQ_STATE;
		freq_State_F = state;
	}
	else
	{
		force_flag &= (~MD_FREQ_STATE);
	}
}

void Compressor::forceOff()
{
}

BOOL Compressor::isRun()
{
	if (g_debug_var.bit.feedback_ignore)
	{
		return isOn();
	}
	return DI_STAT(DI_feedback);
}

BOOL Compressor::getErr()
{
	Error_Flag &= 0x7;
	if ((DI_STAT(DI_CP1_LP) == 0 && CompressorNumber == 1) || (DI_STAT(DI_CP2_LP) == 0 && CompressorNumber == 2))
	{
		Error_Flag |= compressorLowPressureError;
	}
	else
	{
		Error_Flag &= (~compressorLowPressureError);
	}
	if ((DI_STAT(DI_CP1_HP) == 0 && CompressorNumber == 1) || (DI_STAT(DI_CP2_HP) == 0 && CompressorNumber == 2))
	{
		Error_Flag |= compressorHighPressureError;
	}
	else
	{
		Error_Flag &= (~compressorHighPressureError);
	}
	if ((DI_STAT(DI_CP1_Feedback) == 0 && DO_STAT(DO_CP1_Contactor) == 1 && CompressorNumber == 1) || (DI_STAT(DI_CP2_Feedback) == 0 && DO_STAT(DO_CP2_Contactor) == 1 && CompressorNumber == 2))
	{
		Error_Flag |= CompressorContactorError;
	}
	else
	{
		Error_Flag &= (~CompressorContactorError);
	}

	LOG_AC("\n压缩机%d故障,原因：%d,次数：%d,时长：%d\n", CompressorNumber, Error_Flag, FaultTimer.getOnCnt(), FaultTimer.getThisRunTime());
	return Error_Flag | 0x0;
}

BOOL Compressor::isOn()
{
	return DO_STAT(DO_run);
}

void Compressor::Off()
{
	if (!isOn())
	{
		return;
	}
	if ((TimeGap(timer.getOnTime()) < gCompressorStartStopGap) && isOn())
	{
		LOG_AC("压缩机上次启动到现在时间间隔不足180s:%d\n", TimeGap(timer.getOnTime()) / 1000);

		return;
	}
	setFreq(0, TRUE);
	if (isOn())
	{
		timer.Stop();
	}
	DO_CLR(DO_run);
}

void Compressor::On()
{
	if (isOn())
	{
		return;
	}
	LOG_AC("压缩机允许启动信号：%d------------\n", g_car.trdp.Get_Compressor_Enable());
	// 压缩机允许启动信号
	//  if(!g_car.trdp.Get_Compressor_Enable()){
	//  	return;
	//  }

	// 压缩机启停时间间隔
	if ((TimeGap(timer.getOffTime()) < gCompressorStartStopGap) && !isOn())
	{
		LOG_AC("压缩机上次关闭到现在时间间隔不足180s：%d\n", TimeGap(timer.getOffTime()) / 1000);
		return;
	}

	timer.Start();
	DO_SET(DO_run);
}
