#ifndef __AC_COMPRESSOR_H__
#define __AC_COMPRESSOR_H__
#include "ac_include.h"

// ѹ����
class Compressor
{
public:
	U8 DI_feedback;
	BOOL firstStart;		// �״���������СƵ�ʣ�40hz����Ƶ��������1���ӡ�����ϵ�͵�Ƶ�����лᵼ���󻬲���
	U8 DO_run;				// ѹ�����������λ  (��DO�е�λ��)
	U16 freq_HZ;			//��Ƶ��Ƶ��
	U16 freq_State;			//��Ƶ��״̬
	S16 HighPressureValue; 	//��ѹ������ֵ
	S16 LowPressureValue;  	//��ѹ������ֵ
	U16 freq_Out;			//���Ƶ��
	U16 current_out;		//�������
	U16 voltage_out;		//�����ѹ

	U32 lastSetFrequencyTime;			//�ϴ�����Ƶ��ʱ��
	S16 HP_Value;			//��ѹ������ֵ
	S16 LP_Value;			//��ѹ������ֵ
	BOOL HP_lock;			//��ѹ������δ�����ϣ����߼��ж���
	BOOL LP_lock;			//��ѹ������δ�����ϣ����߼��ж���
	U8 HP_trigger;			// ����6������ϵͳ��ѹѹ���쳣Ԥ��
	U8 LP_trigger;			// ����6������ϵͳ��ѹѹ���쳣Ԥ��
	U32 HP_error_time;
	U32 LP_error_time;		
	AC_TIMER timer;


public:
	void setFreq(U16 HZ, BOOL Force);
	U16 getFrequency();
	U16 getFreqState();
	BOOL isOn();
	void Off();
	void On();
	void forceOff();
	BOOL isRun();
	BOOL getErr();


private:
	const char* name;


};

class CompSW
{
public:
};

#endif //__AC_COMPRESSOR_H__
