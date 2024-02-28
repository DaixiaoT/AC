#include "ac_ctrl.h"
#include "trdp/trdp_app.h"
#include "ac_ai.h"
#include "ac_trdp.h"
#include "ac_fan.h"
#include "ptu/ptu_monitor.h"
ACControl g_car;

void ACControl::Init()
{
	LOG_PRINT("ACControl Init()\n");
	selfTestFlag = FALSE;

	set1.FreshAirDamp.name = "�·���";
	set1.FreshAirDamp.Init();
	set1.FreshAirDamp.DampOpenAllTime = g_parm.FreshAirDampAllOpenTime_s * 100;
	set1.FreshAirDamp.DO_Open = DO_FAS_Open;
	set1.FreshAirDamp.DO_Close = DO_FAS_Close;

	// �ͷ��1
	set1.Ventilator_1.DO_run = DO_SAF1_Contactor;
	set1.Ventilator_1.DI_feedback = DI_SAF1_Feedback;

	// �ͷ��2
	set1.Ventilator_2.DO_run = DO_SAF2_Contactor;
	set1.Ventilator_2.DI_feedback = DI_SAF2_Feedback;

	// �������1
	set1.Condenser1.DO_run = DO_CDF1_Contactor;
	set1.Condenser1.DI_feedback = DI_CDF1_Feedback;

	// �������2
	set1.Condenser2.DO_run = DO_CDF2_Contactor;
	set1.Condenser2.DI_feedback = DI_CDF2_Feedback;

	// ѹ����1
	set1.Compressor_1.DO_run = DO_CP1_Contactor;
	set1.Compressor_1.DI_feedback = DI_CP1_Feedback;

	// ѹ����2
	set1.Compressor_2.DO_run = DO_CP2_Contactor;
	set1.Compressor_2.DI_feedback = DI_CP2_Feedback;

	// �����1
	set1.heater1.DO_run = DO_HEATER1_Contactor;
	set1.heater1.DI_feedback = DI_HEATER1_Feedback;

	// �����2
	set1.heater2.DO_run = DO_HEATER2_Contactor;
	set1.heater2.DI_feedback = DI_HEATER2_Feedback;

	set1.Compressor_1.timer.init("ѹ����1", 0, g_runInfo.timer_u1.Cmp1, FALSE, 0);
	set1.Compressor_2.timer.init("ѹ����2", 0, g_runInfo.timer_u1.Cmp2, FALSE, 0);
}

// �����·��¶ȣ�����ʹ��TRDP�·��¶�
S16 ACControl::car_Outtemp()
{
	if (trdp.Get_OutTemp() == 255)
		return get_Tf1();
	return ((trdp.Get_OutTemp() - 40) * 10 / 2);
}

// �����ط��¶ȣ�����ʹ�ÿ������������¶�
S16 ACControl::car_Intemp()
{
	if (get_mix_Tg() == TEMP_BAD)
		return ((trdp.Get_InTemp() == 255) ? TEMP_BAD : ((trdp.Get_InTemp() - 45) * 10 / 2));
	return get_mix_Tg();
}

// ���������߼�
void ACControl::ProcessMode()
{
	LOG_AC("CCU_HVAC_Car1Mode_U8:%d\n", g_ccutohavcdata.CCU_HVAC_Car1Mode_U8);
	LOG_AC("CCU_Lifesign_U32 mode:%d\n", g_ccutohavcdata.CCU_Lifesign_U32);

	// LOG_AC("autoRefreshFlag:%d\n", s_DeviceStatusAutoRefresh);
	// LOG_PRINT("Enter ACControl::ProcessMode()\n");
	// LOG_AC("\nTRDP Mode:%d\n", g_car.trdp.getMode());
	// 1.�Լ�
	U8 selfTestStatus = SelfTestMode();
	if (selfTestStatus != 2)
	{
		// return;
	}
	// 2.�ֶ�ģʽ��ȡ
	handMode = UpdateHandMode();
	LOG_AC("�ֶ�ģʽ����:%d\n", handMode);
	// 3.TRDPģʽ��ȡ
	trdpMode = g_car.trdp.Get_Mode();
	LOG_AC("TRDPģʽ:%d\n", trdpMode);

	// 4.����ֶ�ģʽΪ�Զ�����ʹ��TRDPģʽ�����TRDPģʽΪ�Զ�������ݴ������¶ȼ���ʵ���������ģʽ
	if (handMode == HAND_AUTO)
	{
		ctrlMode = (SET_MODE)trdpMode;
	}
	else
	{
		trdpMode = TRDP_STOP;
		ctrlMode = SET_FORCE_STOP;
	}
	LOG_AC("����ģʽ:%d\n", ctrlMode);

	// 5.����¶ȴ����������й��������ͨ��ģʽ�������ȡ�·�ط��ͷ��¶�ֵ
	if (Check_TempError())
	{
		LOG_AC("���������ϣ�����ͨ��ģʽ\n");
		ToVent();
		return;
	}
	else
	{
		set1.setTemp(car_Outtemp(), car_Intemp(), get_Ts1()); // �·硢�ط硢�ͷ�
	}

	LOG_AC("��ǰ�����¶ȣ��·磺%d,�ط磺%d,�ͷ磺%d\n", set1.Tf, set1.Tg, set1.Ts);

	// �����ֶ�����Ŀ���¶�
	GetHandTemp();
	set1.Td = handTemperature;

	if ((ctrlMode == SET_STOP) || (ctrlMode == SET_VENT) || (ctrlMode == SET_EM_VENT))
		PI_set1.PI_Ctrl(FALSE, (S32)set1.Td, (S32)set1.Tg, (S32)set1.Tf, (S32)set1.Ts);
	else
		PI_set1.PI_Ctrl(TRUE, (S32)set1.Td, (S32)set1.Tg, (S32)set1.Tf, (S32)set1.Ts);

	acu = set1.Ts - (S16)PI_set1.iKanalsollwert_S; // m_acu ��������������srs ����ͨ��Ƶ�ʻ��߿������
	LOG_AC("\nacu=%d\n", acu);
	switch (ctrlMode)
	{
	case TRDP_FULL_HEAT:
		ToFullHeat();
		break;
	case TRDP_HALF_HEAT:
		ToHalfHeat();
		break;
	case TRDP_COOL:
		ToFullCool();
		break;
	case TRDP_VENT:
		ToVent();
		break;
	case TRDP_AUTO:
		ToAuto();
		break;
	case TRDP_STOP:
		ToStop();
		break;
	case TRDP_FIRE:
		break;
	case TRDP_EMV:
		ToVent();
		break;
	case TRDP_WASH:
		break;

	default:
		break;
	}
}

void ACControl::ToFullHeat()
{
	heatingTimer.Start();
	coolingTimer.Stop();
	ctrlMode = SET_FULL_HEAT;
	LOG_AC("ȫ��ģʽ\n");
	BOOL heat1Err = set1.heater1.checkError();
	BOOL heat2Err = set1.heater2.checkError();

	// ���ͷ��
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	// �ж��ͷ���Ƿ���
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		LOG_AC("�ͷ������ʧ��\n");
		ToStop();
		return;
	}

	// �ر�ѹ����
	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	// �ж�ѹ�����Ƿ�رճɹ�
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("ѹ�����ر�ʧ��\n");
		return;
	}
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < gCompressorCondensorStopGap ||
		TimeGap(set1.Compressor_2.timer.getOffTime()) < gCompressorCondensorStopGap)
	{
		LOG_AC("ѹ����ͣ�����������ͣ��ʱ����5s\n");
		return;
	}
	set1.Condenser1.Off(FALSE);
	if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.Off(FALSE);

	// �ж���������Ƿ�رճɹ�
	if (set1.Condenser1.isRun() || set1.Condenser2.isRun())
	{
		LOG_AC("��������ر�ʧ��\n");
		return;
	}

	// �жϵ�����Ƿ����
	if (heat1Err && heat2Err)
	{
		LOG_AC("��̨����ȹ���\n");
	}

	// �����ͷ��ת��
	set1.Ventilator_1.setSpeed(7500);
	set1.Ventilator_2.setSpeed(7500);

	set1.FreshAirDamp.RunClose();

	set1.heater1.On();
	if (TimeGap(set1.heater1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.heater2.On();

	if (!set1.heater1.isRun() && !set1.heater2.isRun())
	{
		LOG_AC("����ȿ���ʧ��\n");
	}
}

// �Լ������Ѳ���
BOOL ACControl::SelfTestMode()
{
	LOG_AC("�Լ��У� ��ʼʱ�䣺%d, ����ʱ����%d\n", selfTestStartTime / 1000, selfTestLastTime / 1000);
	BOOL finishStatus = set1.FreshAirDamp.SelfCheckFinished;
	if (!selfTestFlag)
	{
		selfTestStartTime = sys_time();
		selfTestFlag = TRUE;
		return 0;
	}
	else if (!finishStatus)
	{
		selfTestLastTime = sys_time() - selfTestStartTime;
		set1.FreshAirDamp.DoSelfCheck();
		return 1;
	}
	else if (set1.FreshAirDamp.SelfCheckState == 4)
	{
		LOG_AC("�Լ������ͨ���Լ�\n");
		return 2;
	}
	else
	{
		LOG_AC("�Լ�δͨ��\n");
		return 3;
	}
}

void ACControl::ToHalfHeat()
{
	heatingTimer.Start();
	coolingTimer.Stop();
	LOG_AC("����ģʽ\n");
	ctrlMode = SET_HALF_HEAT;
	BOOL heat1Err = set1.heater1.checkError();
	BOOL heat2Err = set1.heater2.checkError();

	// ���ͷ��
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	// �ж��ͷ���Ƿ���
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		ToStop();
		LOG_AC("�ͷ������ʧ��\n");
		return;
	}

	// �ر�ѹ����
	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	// �ж�ѹ�����Ƿ�رճɹ�
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("ѹ�����ر�ʧ��\n");
		return;
	}
	set1.Condenser1.Off(FALSE);
	if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.Off(FALSE);

	// �ж���������Ƿ�رճɹ�
	if (set1.Condenser1.isRun() || set1.Condenser2.isRun())
	{
		LOG_AC("��������ر�ʧ��\n");
		return;
	}

	// �жϵ�����Ƿ����
	if (heat1Err && heat2Err)
	{
		LOG_AC("��̨����ȹ���\n");
	}

	set1.FreshAirDamp.RunClose();
}

void ACControl::ToStop()
{
	LOG_AC("ͣ��ģʽ\n");
	ctrlMode = SET_STOP;
	set1.heater1.Off();
	if (TimeGap(set1.heater1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.heater2.Off();

	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	// �ж�ѹ�����Ƿ�رճɹ�
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("ѹ�����ر�ʧ��\n");
		return;
	}

	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < gCompressorCondensorStopGap ||
		TimeGap(set1.Compressor_2.timer.getOffTime()) < gCompressorCondensorStopGap)
	{
		LOG_AC("ѹ����ͣ�����������ͣ��ʱ����5s\n");
		return;
	}
	set1.Condenser1.Off(FALSE);
	if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.Off(FALSE);

	if (set1.Condenser1.isRun() || set1.Condenser2.isRun())
	{
		LOG_AC("��������ر�ʧ��\n");
		return;
	}

	if (TimeGap(set1.Condenser1.timer.getOffTime()) < gCondensorFanStopGap ||
		TimeGap(set1.Condenser2.timer.getOffTime()) < gCondensorFanStopGap)
	{
		LOG_AC("�ͷ��ͣ�����������ͣ��ʱ����5s\n");
		return;
	}

	set1.Ventilator_1.setSpeed(0);
	set1.Ventilator_2.setSpeed(0);
	set1.Ventilator_1.Off(FALSE);
	if (TimeGap(set1.Ventilator_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.Off(FALSE);
}

void ACControl::ToHalfCool()
{
}

void ACControl::ToVent()
{
	LOG_AC("ͨ��ģʽ\n");
	ctrlMode = SET_VENT;

	set1.heater1.Off();
	if (TimeGap(set1.heater1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.heater2.Off();

	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	if (!set1.Compressor_1.isRun() && !set1.Compressor_2.isRun())
	{
		set1.Condenser1.Off(FALSE);
		if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
		{
			return;
		}
		set1.Condenser2.Off(FALSE);
	}

	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();
	set1.Ventilator_1.setSpeed(7500);
	set1.Ventilator_2.setSpeed(7500);
}

void ACControl::ToFullCool()
{
	heatingTimer.Stop();
	coolingTimer.Start();
	LOG_AC("\nȫ��ģʽ\n");
	ctrlMode = SET_FULL_COOL;
	BOOL condenser1Err = set1.Condenser1.getErr();
	BOOL condenser2Err = set1.Condenser2.getErr();
	BOOL compressor1Err = set1.Compressor_1.getErr();
	BOOL compressor2Err = set1.Compressor_2.getErr();

	// ���ͷ��
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	set1.Ventilator_1.setSpeed(7500);
	set1.Ventilator_2.setSpeed(7500);
	// �ж��ͷ���Ƿ���
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		LOG_AC("�ͷ������ʧ�ܣ�\n");
		ToStop();
		return;
	}

	// �رյ����
	set1.heater1.Off();
	if (TimeGap(set1.heater1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.heater2.Off();

	if (set1.heater1.isRun() || set1.heater2.isRun())
	{
		LOG_AC("����ȹر�ʧ��\n");
		return;
	}

	// �����������
	set1.Condenser1.On();
	if (TimeGap(set1.Condenser1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.On();

	// �����������
	if (!set1.Condenser1.isRun() && !set1.Condenser2.isRun())
	{
		ToVent();
		LOG_AC("�����������ʧ�ܣ�\n");
		return;
	}

	// ѹ��������
	if (compressor1Err && compressor2Err)
	{
		LOG_AC("����ѹ����������\n");
		// ����ѹ����������
	}

	/*if ((!compressor1Err) && (set1.Compressor_1.timer.getTotalRunTime() < set1.Compressor_2.timer.getTotalRunTime())) {
		LOG_AC("һ��ѹ��������\n");
		set1.Compressor_1.On();
	}
	else {*/
	// LOG_AC("����ѹ��������\n");
	set1.Compressor_1.On();
	if (TimeGap(set1.Compressor_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.On();
	//}
}

void ACControl::ToAuto()
{//ϵͳ�״ν�����������ģʽҲ��Ҫ�ȴ�10min
	LOG_AC("�Զ�ģʽ\n");
	ctrlMode = SET_AUTO;
	if (acu > g_heat_D1 && set1.Tf > 160 && TimeGap(heatingTimer.getOffTime()) > 10*60 * 1000)
	{
		ToFullCool();
	}
	else if (acu > g_heat_D2 && TimeGap(coolingTimer.getOffTime()) > 10*60 * 1000)
	{
		ToHalfHeat();
	}
	else if (acu < g_heat_D2 && TimeGap(coolingTimer.getOffTime()) > 10*60 * 1000)
	{
		ToFullHeat();
	}
	else
	{
		ToVent();
	}
}

S16 ACControl::get_Tf1()
{
	return ctrl_AI[0].getValue();
}

S16 ACControl::get_Ts1()
{
	return ctrl_AI[1].getValue();
}

S16 ACControl::get_Tg1()
{
	return ctrl_AI[2].getValue();
}

S16 ACControl::get_Tg2()
{
	return ctrl_AI[4].getValue();
}

S16 ACControl::get_mix_Tf()
{
	return get_Tf1();
}

S16 ACControl::get_mix_TS()
{
	return get_Ts1();
}

S16 ACControl::get_mix_Tg()
{
	AI_CTRL *pT[1] = {ctrl_AI + 1};
	int sum = 0;
	int cnt = 0;
	for (int i = 0; i < 1; i++)
	{
		if (pT[i]->getValue() != TEMP_BAD)
		{
			cnt++;
			sum += pT[i]->getValue();
		}
	}

	if (cnt == 0)
		return TEMP_BAD;
	return (sum / cnt) - 25;
}

void ACControl::GetHandTemp()
{
	static U8 s_prev_temp = 0;
	static U32 s_handTemp_time = 0;
	U8 MTEMP = DI_STAT(DI_TEMP_SWITCH_4) |
			   (DI_STAT(DI_TEMP_SWITCH_3) << 1) |
			   (DI_STAT(DI_TEMP_SWITCH_2) << 2) |
			   (DI_STAT(DI_TEMP_SWITCH_1) << 3);
	// LOG_PRINT("sw MTEMP:%d\n", MTEMP);
	if (s_prev_temp != MTEMP)
	{
		s_prev_temp = MTEMP;
		s_handTemp_time = sys_time();
		return;
	}
	if (TimeGap(s_handTemp_time) > (g_parm.ModeSwitchDelay_s * 1000)) // 5����ʱ
		if (s_prev_temp == 8)										  // 160
			handTemperature = 200;
		else if (s_prev_temp == 4) // 170
			handTemperature = 200;
		else if (s_prev_temp == 12) // 180
			handTemperature = 200;
		else if (s_prev_temp == 2) // 190
			handTemperature = 200;
		else if (s_prev_temp == 10)
			handTemperature = 200;
		else if (s_prev_temp == 6)
			handTemperature = 210;
		else if (s_prev_temp == 14)
			handTemperature = 220;
		else if (s_prev_temp == 1)
			handTemperature = 230;
		else if (s_prev_temp == 9)
			handTemperature = 240;
		else if (s_prev_temp == 5)
			handTemperature = 250;
		else
		{
			handTemperature = 220;
			LOG_PRINT("sw ���������������¶ȱ���:%d\n", handTemperature);
		}

	LOG_PRINT("�ֶ��趨�¶� :%d\n", handTemperature);
}

HAND_MODE ACControl::UpdateHandMode()
{
	static U8 s_prev_mode = HAND_OFF;
	static U32 s_time = 0;
	static U8 flag = 0;
	U8 tempMode = 0;
	tempMode = DI_STAT(DI_MODE_SWITCH_POWER) | (DI_STAT(DI_MODE_SWITCH_MT) << 1) | (DI_STAT(DI_MODE_SWITCH_AT) << 2);
	if ((s_prev_mode != tempMode) && (flag == 0))
	{
		s_time = sys_time();
		flag = 1;
	}
	if (TimeGap(s_time) > (g_parm.ModeSwitchDelay_s * 1000))
	{
		s_prev_mode = tempMode;
	}

	/*
	TRDP Mode()δ��ʼ��
	*/

	return (HAND_MODE)s_prev_mode;
}

BOOL ACControl::AdjustRunTimeInfo()
{
	static U32 s_poweron_tick = 0;
	BOOL changed = FALSE;

	if ((TimeGap(s_poweron_tick) > 30 * 1000))
	{
		changed = TRUE;
		s_poweron_tick = sys_time();
		g_runInfo.timer_u1.Cmp1 = set1.Compressor_1.timer.getTotalRunTime();
		g_runInfo.timer_u1.Cmp2 = set1.Compressor_2.timer.getTotalRunTime();
	}

	return changed;
}

/*

selfTestFinished

firstboot


*/
