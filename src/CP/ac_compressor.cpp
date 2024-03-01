
#include "ac_compressor.h"
#include "debug_var.h"
#include "ac_ctrl.h"

void Compressor::setFreq(U16 HZ, BOOL Force)
{
	if (TimeGap(lastSetFrequencyTime) < 1000)
	{
		// ����Ƶ�ʲ���1s
		return;
	}
	if ((HZ == 0) && (isRun()) && (Force != TRUE))
		return;
	if (isRun() && (HZ != 0))
	{
		if ((firstStart == TRUE))
		{
			// ��Ƶ3���ӣ��ڼ䲻��Ҫ�жϸߵ�ѹ���������н�Ƶ����
			// LOG_PRINT("��Ƶ3���ӣ�Ƶ��40HZ ��ǰ��Ƶʱ��=%d\r\n", TimeGap(lastOnTime));
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

	LOG_AC("\nѹ����%d����,ԭ��%d,������%d,ʱ����%d\n", CompressorNumber, Error_Flag, FaultTimer.getOnCnt(), FaultTimer.getThisRunTime());
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
		LOG_AC("ѹ�����ϴ�����������ʱ��������180s:%d\n", TimeGap(timer.getOnTime()) / 1000);

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
	LOG_AC("ѹ�������������źţ�%d------------\n", g_car.trdp.Get_Compressor_Enable());
	// ѹ�������������ź�
	//  if(!g_car.trdp.Get_Compressor_Enable()){
	//  	return;
	//  }

	// ѹ������ͣʱ����
	if ((TimeGap(timer.getOffTime()) < gCompressorStartStopGap) && !isOn())
	{
		LOG_AC("ѹ�����ϴιرյ�����ʱ��������180s��%d\n", TimeGap(timer.getOffTime()) / 1000);
		return;
	}

	timer.Start();
	DO_SET(DO_run);
}
