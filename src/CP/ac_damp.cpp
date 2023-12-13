
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
	case 0: //�״��ж�λ�ã�
		if (CloseOK())//����ǹ��ŵģ���ôִ�п��ļ��
		{
			RunOpen();//��
			SelfCheckState = 1;
			s_damp_flg = TRUE;
			return;
		}
		SelfCheckState = 2;//����ǿ��ŵģ���ôִ�йصļ��
		return;
	case 1: //�״��ж��ǹز�ִ�п�֮��ķ������
		if (OpenOK())//����ǿ������OK��
		{
			SelfCheckState = 0;//׼������λ��
			return;
		}
		else if (TimeGap(DO_OpenTime) >= gRDampTimeOut)
		{ // ��ʱ, ֹͣ���Ŷ���
			RunClose();
			checkError[0] += (!OpenOK());
			SelfCheckState = 6;//����ʱ�޷���
			LOG_AC("\n�����ų�ʱ�޷���\n");
		}
		return;

	case 2:  //�ر�
		RunClose();
		SelfCheckState = 3;
		return;
	case 3:  //�رյȴ�����
		if (CloseOK())
		{
			if (s_damp_flg == FALSE)
			{
				RunOpen();
				SelfCheckState = 5;//��OK��ȥ��
				return;
			}
			RunClose();
			checkNum = 0;
			SelfCheckState = 4; // �Լ�OK,stop check
			checkError[0] = 0;
			return;
		}
		if (TimeGap(DO_CloseTime) >= gRDampTimeOut)
		{
			RunClose();
			checkError[0] += (!CloseOK());
			SelfCheckState = 6; //�رշ�����ʱ stop check
			LOG_AC("\n�ط��ų�ʱ�޷���\n");
		}
		return;
	case 5:   //�����ȴ�����
		if (OpenOK())
		{
			RunClose();
			SelfCheckState = 3; // ��������OK
			s_damp_flg = TRUE;
			return;
		}
		else if (TimeGap(DO_OpenTime) >= gRDampTimeOut)
		{ // ��ʱ, ִ�йط��Ŷ���
			RunClose();
			checkError[0] += (!OpenOK());
			SelfCheckState = 6; //����������ʱ6
			LOG_AC("\n2-�����ų�ʱ�޷���\n");
		}
		return;

	case 4:
		SelfCheckFinished = TRUE;
		LOG_AC("�Լ��豸�ɹ��޹���\r\n");
		break;
	case 6:
		SelfCheckFinished = TRUE;
		LOG_AC("�Լ��豸�й���\r\n");
		break;
	default:
		break;
	}
}
void FRESH_AIR_DAMP::RunOpen()
{
	LOG_PRINT("\n�򿪷���\n");
    DO_SET(DO_Open);
    DO_CLR(DO_Close);
	DO_OpenTime = sys_time();
}

void FRESH_AIR_DAMP::RunClose()
{
	LOG_PRINT("\n�رշ���\n");

    DO_CLR(DO_Open);
    DO_SET(DO_Close);
	DO_CloseTime = sys_time();
}

BOOL FRESH_AIR_DAMP::OpenOK()
{
	LOG_PRINT("\n�жϷ����Ƿ���\n");

    return resistor_feedback >= 4500;
}

BOOL FRESH_AIR_DAMP::CloseOK()
{
    LOG_PRINT("\n�жϷ����Ƿ�ر�\n");

    return resistor_feedback <= 500;
}



void FRESH_AIR_DAMP::Init()
{
	SelfCheckFinished = FALSE;
	checkNum = 0;
	SelfCheckState = 0;
	checkError[0] = checkError[1] = 0;
	resistor_feedback = 0; // �緧1���跴��
} 
