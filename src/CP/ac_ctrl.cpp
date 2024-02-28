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

	set1.FreshAirDamp.name = "新风门";
	set1.FreshAirDamp.Init();
	set1.FreshAirDamp.DampOpenAllTime = g_parm.FreshAirDampAllOpenTime_s * 100;
	set1.FreshAirDamp.DO_Open = DO_FAS_Open;
	set1.FreshAirDamp.DO_Close = DO_FAS_Close;

	// 送风机1
	set1.Ventilator_1.DO_run = DO_SAF1_Contactor;
	set1.Ventilator_1.DI_feedback = DI_SAF1_Feedback;

	// 送风机2
	set1.Ventilator_2.DO_run = DO_SAF2_Contactor;
	set1.Ventilator_2.DI_feedback = DI_SAF2_Feedback;

	// 冷凝风机1
	set1.Condenser1.DO_run = DO_CDF1_Contactor;
	set1.Condenser1.DI_feedback = DI_CDF1_Feedback;

	// 冷凝风机2
	set1.Condenser2.DO_run = DO_CDF2_Contactor;
	set1.Condenser2.DI_feedback = DI_CDF2_Feedback;

	// 压缩机1
	set1.Compressor_1.DO_run = DO_CP1_Contactor;
	set1.Compressor_1.DI_feedback = DI_CP1_Feedback;

	// 压缩机2
	set1.Compressor_2.DO_run = DO_CP2_Contactor;
	set1.Compressor_2.DI_feedback = DI_CP2_Feedback;

	// 电加热1
	set1.heater1.DO_run = DO_HEATER1_Contactor;
	set1.heater1.DI_feedback = DI_HEATER1_Feedback;

	// 电加热2
	set1.heater2.DO_run = DO_HEATER2_Contactor;
	set1.heater2.DI_feedback = DI_HEATER2_Feedback;

	set1.Compressor_1.timer.init("压缩机1", 0, g_runInfo.timer_u1.Cmp1, FALSE, 0);
	set1.Compressor_2.timer.init("压缩机2", 0, g_runInfo.timer_u1.Cmp2, FALSE, 0);
}

// 车辆新风温度，优先使用TRDP新风温度
S16 ACControl::car_Outtemp()
{
	if (trdp.Get_OutTemp() == 255)
		return get_Tf1();
	return ((trdp.Get_OutTemp() - 40) * 10 / 2);
}

// 车辆回风温度，优先使用控制器传感器温度
S16 ACControl::car_Intemp()
{
	if (get_mix_Tg() == TEMP_BAD)
		return ((trdp.Get_InTemp() == 255) ? TEMP_BAD : ((trdp.Get_InTemp() - 45) * 10 / 2));
	return get_mix_Tg();
}

// 控制器主逻辑
void ACControl::ProcessMode()
{
	LOG_AC("CCU_HVAC_Car1Mode_U8:%d\n", g_ccutohavcdata.CCU_HVAC_Car1Mode_U8);
	LOG_AC("CCU_Lifesign_U32 mode:%d\n", g_ccutohavcdata.CCU_Lifesign_U32);

	// LOG_AC("autoRefreshFlag:%d\n", s_DeviceStatusAutoRefresh);
	// LOG_PRINT("Enter ACControl::ProcessMode()\n");
	// LOG_AC("\nTRDP Mode:%d\n", g_car.trdp.getMode());
	// 1.自检
	U8 selfTestStatus = SelfTestMode();
	if (selfTestStatus != 2)
	{
		// return;
	}
	// 2.手动模式获取
	handMode = UpdateHandMode();
	LOG_AC("手动模式开关:%d\n", handMode);
	// 3.TRDP模式获取
	trdpMode = g_car.trdp.Get_Mode();
	LOG_AC("TRDP模式:%d\n", trdpMode);

	// 4.如果手动模式为自动，则使用TRDP模式，如果TRDP模式为自动，则根据传感器温度计算实际所需进入模式
	if (handMode == HAND_AUTO)
	{
		ctrlMode = (SET_MODE)trdpMode;
	}
	else
	{
		trdpMode = TRDP_STOP;
		ctrlMode = SET_FORCE_STOP;
	}
	LOG_AC("机组模式:%d\n", ctrlMode);

	// 5.检查温度传感器，如有故障则进入通风模式，否则获取新风回风送风温度值
	if (Check_TempError())
	{
		LOG_AC("传感器故障，进入通风模式\n");
		ToVent();
		return;
	}
	else
	{
		set1.setTemp(car_Outtemp(), car_Intemp(), get_Ts1()); // 新风、回风、送风
	}

	LOG_AC("当前机组温度：新风：%d,回风：%d,送风：%d\n", set1.Tf, set1.Tg, set1.Ts);

	// 更新手动开关目标温度
	GetHandTemp();
	set1.Td = handTemperature;

	if ((ctrlMode == SET_STOP) || (ctrlMode == SET_VENT) || (ctrlMode == SET_EM_VENT))
		PI_set1.PI_Ctrl(FALSE, (S32)set1.Td, (S32)set1.Tg, (S32)set1.Tf, (S32)set1.Ts);
	else
		PI_set1.PI_Ctrl(TRUE, (S32)set1.Td, (S32)set1.Tg, (S32)set1.Tf, (S32)set1.Ts);

	acu = set1.Ts - (S16)PI_set1.iKanalsollwert_S; // m_acu 是需求量，根据srs 给不通的频率或者开电加热
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
	LOG_AC("全热模式\n");
	BOOL heat1Err = set1.heater1.checkError();
	BOOL heat2Err = set1.heater2.checkError();

	// 打开送风机
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	// 判断送风机是否开启
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		LOG_AC("送风机开启失败\n");
		ToStop();
		return;
	}

	// 关闭压缩机
	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	// 判断压缩机是否关闭成功
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("压缩机关闭失败\n");
		return;
	}
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < gCompressorCondensorStopGap ||
		TimeGap(set1.Compressor_2.timer.getOffTime()) < gCompressorCondensorStopGap)
	{
		LOG_AC("压缩机停机到冷凝风机停机时间间隔5s\n");
		return;
	}
	set1.Condenser1.Off(FALSE);
	if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.Off(FALSE);

	// 判断冷凝风机是否关闭成功
	if (set1.Condenser1.isRun() || set1.Condenser2.isRun())
	{
		LOG_AC("冷凝风机关闭失败\n");
		return;
	}

	// 判断电加热是否故障
	if (heat1Err && heat2Err)
	{
		LOG_AC("两台电加热故障\n");
	}

	// 设置送风机转速
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
		LOG_AC("电加热开启失败\n");
	}
}

// 自检流程已测试
BOOL ACControl::SelfTestMode()
{
	LOG_AC("自检中： 开始时间：%d, 持续时长：%d\n", selfTestStartTime / 1000, selfTestLastTime / 1000);
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
		LOG_AC("自检结束，通过自检\n");
		return 2;
	}
	else
	{
		LOG_AC("自检未通过\n");
		return 3;
	}
}

void ACControl::ToHalfHeat()
{
	heatingTimer.Start();
	coolingTimer.Stop();
	LOG_AC("半热模式\n");
	ctrlMode = SET_HALF_HEAT;
	BOOL heat1Err = set1.heater1.checkError();
	BOOL heat2Err = set1.heater2.checkError();

	// 打开送风机
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	// 判断送风机是否开启
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		ToStop();
		LOG_AC("送风机开启失败\n");
		return;
	}

	// 关闭压缩机
	set1.Compressor_1.Off();
	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.Off();

	// 判断压缩机是否关闭成功
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("压缩机关闭失败\n");
		return;
	}
	set1.Condenser1.Off(FALSE);
	if (TimeGap(set1.Condenser1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.Off(FALSE);

	// 判断冷凝风机是否关闭成功
	if (set1.Condenser1.isRun() || set1.Condenser2.isRun())
	{
		LOG_AC("冷凝风机关闭失败\n");
		return;
	}

	// 判断电加热是否故障
	if (heat1Err && heat2Err)
	{
		LOG_AC("两台电加热故障\n");
	}

	set1.FreshAirDamp.RunClose();
}

void ACControl::ToStop()
{
	LOG_AC("停机模式\n");
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

	// 判断压缩机是否关闭成功
	if (set1.Compressor_1.isRun() || set1.Compressor_2.isRun())
	{
		LOG_AC("压缩机关闭失败\n");
		return;
	}

	if (TimeGap(set1.Compressor_1.timer.getOffTime()) < gCompressorCondensorStopGap ||
		TimeGap(set1.Compressor_2.timer.getOffTime()) < gCompressorCondensorStopGap)
	{
		LOG_AC("压缩机停机到冷凝风机停机时间间隔5s\n");
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
		LOG_AC("冷凝风机关闭失败\n");
		return;
	}

	if (TimeGap(set1.Condenser1.timer.getOffTime()) < gCondensorFanStopGap ||
		TimeGap(set1.Condenser2.timer.getOffTime()) < gCondensorFanStopGap)
	{
		LOG_AC("送风机停机到冷凝风机停机时间间隔5s\n");
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
	LOG_AC("通风模式\n");
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
	LOG_AC("\n全冷模式\n");
	ctrlMode = SET_FULL_COOL;
	BOOL condenser1Err = set1.Condenser1.getErr();
	BOOL condenser2Err = set1.Condenser2.getErr();
	BOOL compressor1Err = set1.Compressor_1.getErr();
	BOOL compressor2Err = set1.Compressor_2.getErr();

	// 打开送风机
	set1.Ventilator_1.On();
	if (TimeGap(set1.Ventilator_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Ventilator_2.On();

	set1.Ventilator_1.setSpeed(7500);
	set1.Ventilator_2.setSpeed(7500);
	// 判断送风机是否开启
	if (!set1.Ventilator_1.isRun() && !set1.Ventilator_2.isRun())
	{
		LOG_AC("送风机开启失败！\n");
		ToStop();
		return;
	}

	// 关闭电加热
	set1.heater1.Off();
	if (TimeGap(set1.heater1.timer.getOffTime()) < 2000)
	{
		return;
	}
	set1.heater2.Off();

	if (set1.heater1.isRun() || set1.heater2.isRun())
	{
		LOG_AC("电加热关闭失败\n");
		return;
	}

	// 冷凝风机开启
	set1.Condenser1.On();
	if (TimeGap(set1.Condenser1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Condenser2.On();

	// 冷凝风机故障
	if (!set1.Condenser1.isRun() && !set1.Condenser2.isRun())
	{
		ToVent();
		LOG_AC("冷凝风机开启失败！\n");
		return;
	}

	// 压缩机故障
	if (compressor1Err && compressor2Err)
	{
		LOG_AC("两个压缩机均故障\n");
		// 两个压缩机均故障
	}

	/*if ((!compressor1Err) && (set1.Compressor_1.timer.getTotalRunTime() < set1.Compressor_2.timer.getTotalRunTime())) {
		LOG_AC("一号压缩机开启\n");
		set1.Compressor_1.On();
	}
	else {*/
	// LOG_AC("二号压缩机开启\n");
	set1.Compressor_1.On();
	if (TimeGap(set1.Compressor_1.timer.getOnTime()) < 2000)
	{
		return;
	}
	set1.Compressor_2.On();
	//}
}

void ACControl::ToAuto()
{//系统首次进入制冷制热模式也需要等待10min
	LOG_AC("自动模式\n");
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
	if (TimeGap(s_handTemp_time) > (g_parm.ModeSwitchDelay_s * 1000)) // 5秒延时
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
			LOG_PRINT("sw 开关输入错误，设点温度保持:%d\n", handTemperature);
		}

	LOG_PRINT("手动设定温度 :%d\n", handTemperature);
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
	TRDP Mode()未初始化
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
