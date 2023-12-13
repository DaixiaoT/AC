#include "ac_ctrl.h"
ACControl g_car;
void ACControl::Init()
{
	LOG_PRINT("ACControl Init()\n");

	set1.FreshAirDamp.name = "新风门";
	set1.FreshAirDamp.Init();
	set1.FreshAirDamp.DampOpenAllTime = g_parm.FreshAirDampAllOpenTime_s * 100;
	set1.FreshAirDamp.DO_Open = DO_FAS_Open;
	set1.FreshAirDamp.DO_Close = DO_FAS_Close;

	
}

//控制器主逻辑
void ACControl::ProcessMode()
{
	LOG_PRINT("Enter ACControl::ProcessMode()\n");

	//1.自检
	SelfTestMode();
	//2.模式获取
	handMode = UpdateHandMode();
	LOG_AC("\nhandMode:%d\n", handMode);

}

//自检流程未测试
BOOL ACControl::SelfTestMode()
{

	LOG_PRINT("Enter ACControl::SelfTestMode() SelfCheckTime=%d\n", set1.FreshAirDamp.SelfCheckTime.GetRunTime());
	set1.FreshAirDamp.DoSelfCheck();
	if (set1.FreshAirDamp.SelfCheckFinished == TRUE && set1.FreshAirDamp.SelfCheckState == 4) {
		LOG_AC("\n自检结束，通过自检\n");
	}
	else {
		LOG_AC("\n自检未通过\n");
	}

}

void ACControl::ToVent()
{
	// ToMode(SET_VENT);
}

void ACControl::ToMode(AC_MODE mode)
{
}

void ACControl::SetTemp(S16 temp_Tf, S16 temp_Tg, S16 temp_Ts)
{
	// g_car.Tf = temp_Tf;
	// g_car.Tg = temp_Tg;
	// g_car.Ts = temp_Ts;
}

HAND_MODE ACControl::UpdateHandMode()
{
	static U8 s_prev_mode = HAND_OFF;
	static U32 s_time = 0;
	static U8 flag = 0;
	U8 tempMode = 0;
	tempMode= DI_STAT(DI_MODE_SWITCH_POWER) | (DI_STAT(DI_MODE_SWITCH_MT) << 1) | (DI_STAT(DI_MODE_SWITCH_AT) << 2);
	if ((s_prev_mode != tempMode) && (flag == 0)) {
		s_time = sys_time();
		flag = 1;
	}
	if (TimeGap(s_time) > (g_parm.ModeSwitchDelay_s * 1000)) {
		s_prev_mode = tempMode;
	}

	/*
	TRDP Mode()未初始化
	*/		



	return (HAND_MODE)s_prev_mode;
}
	
/*

selfTestFinished

firstboot



*/
