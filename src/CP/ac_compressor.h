#ifndef __AC_COMPRESSOR_H__
#define __AC_COMPRESSOR_H__
#include "ac_include.h"

// ѹ����
class Compressor
{
public:
	U16 freq_HZ;			//��Ƶ��Ƶ��
	U16 freq_State;			//��Ƶ��״̬
	S16 HighPressureValue; 	//��ѹ������ֵ
	S16 LowPressureValue;  	//��ѹ������ֵ
	U16 freq_Out;			//���Ƶ��
	U16 current_out;		//�������
	U16 voltage_out;		//�����ѹ

public:
	U16 getFrequency();
	U16 getFreqState();
};

class CompSW
{
public:
};

#endif //__AC_COMPRESSOR_H__
