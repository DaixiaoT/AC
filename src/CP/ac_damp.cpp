
#include "ac_damp.h"
#include "ac_ctrl.h"
//U16 EEV_Valve::getSuperheat()
//{
//    return superheat;
//}

//not complete
void FRESH_AIR_DAMP::DoSelfCheck()
{

	static BOOL s_damp_flg = FALSE;

	LOG_AC("DoSelfCheck1 m_check_stat:%d  m_resistor_fb1 :%d  OpenTime:%d  CloseTime:%d \r\n", SelfCheckState, resistor_feedback, TimeGap(DO_OpenTime),TimeGap(DO_CloseTime));
	switch (SelfCheckState)
	{
	case 0: //首次判断位置，
		if (CloseOK())//如果是关着的，那么执行开的检测
		{
			RunOpen();//开
			SelfCheckState = 1;
			s_damp_flg = TRUE;
			return;
		}
		SelfCheckState = 2;//如果是开着的，那么执行关的检测
		return;
	case 1: //首次判断是关并执行开之后的反馈结果
		if (OpenOK())//如果是开检测是OK的
		{
			SelfCheckState = 0;//准备检测关位置
			return;
		}
		else if (TimeGap(DO_OpenTime) >= gRDampTimeOut)
		{ // 超时, 停止风门动作
			RunClose();
			checkError[0] += (!OpenOK());
			SelfCheckState = 6;//开超时无反馈
			LOG_AC("\n开风门超时无反馈\n");
		}
		return;

	case 2:  //关闭
		RunClose();
		SelfCheckState = 3;
		return;
	case 3:  //关闭等待反馈
		if (CloseOK())
		{
			if (s_damp_flg == FALSE)
			{
				RunOpen();
				SelfCheckState = 5;//关OK，去开
				return;
			}
			RunClose();
			checkNum = 0;
			SelfCheckState = 4; // 自检OK,stop check
			checkError[0] = 0;
			return;
		}
		if (TimeGap(DO_CloseTime) >= gRDampTimeOut)
		{
			RunClose();
			checkError[0] += (!CloseOK());
			SelfCheckState = 6; //关闭反馈超时 stop check
			LOG_AC("\n关风门超时无反馈\n");
		}
		return;
	case 5:   //开启等待反馈
		if (OpenOK())
		{
			RunClose();
			SelfCheckState = 3; // 开启反馈OK
			s_damp_flg = TRUE;
			return;
		}
		else if (TimeGap(DO_OpenTime) >= gRDampTimeOut)
		{ // 超时, 执行关风门动作
			RunClose();
			checkError[0] += (!OpenOK());
			SelfCheckState = 6; //开启反馈超时6
			LOG_AC("\n2-开风门超时无反馈\n");
		}
		return;

	case 4:
		SelfCheckFinished = TRUE;
		LOG_AC("自检设备成功无故障\r\n");
		break;
	case 6:
		SelfCheckFinished = TRUE;
		LOG_AC("自检设备有故障\r\n");
		break;
	default:
		break;
	}
}
void FRESH_AIR_DAMP::RunOpen()
{
	LOG_PRINT("\n打开风门\n");
    DO_SET(DO_Open);
    DO_CLR(DO_Close);
	DO_OpenTime = sys_time();
}

void FRESH_AIR_DAMP::RunClose()
{
	LOG_PRINT("\n关闭风门\n");

    DO_CLR(DO_Open);
    DO_SET(DO_Close);
	DO_CloseTime = sys_time();
}

BOOL FRESH_AIR_DAMP::OpenOK()
{
	LOG_PRINT("\n判断风门是否开启\n");

    return resistor_feedback >= 4500;
}

BOOL FRESH_AIR_DAMP::CloseOK()
{
    LOG_PRINT("\n判断风门是否关闭\n");

    return resistor_feedback <= 500;
}



void FRESH_AIR_DAMP::Init()
{
	SelfCheckFinished = FALSE;
	checkNum = 0;
	SelfCheckState = 0;
	checkError[0] = checkError[1] = 0;
	resistor_feedback = 0; // 风阀1电阻反馈
} 
