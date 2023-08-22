#ifndef __AC_COMPRESSOR_H__
#define __AC_COMPRESSOR_H__
#include "ac_include.h"

// 压缩机
class Compressor
{
public:
	U16 freq_HZ;			//变频器频率
	U16 freq_State;			//变频器状态
	S16 HighPressureValue; 	//高压传感器值
	S16 LowPressureValue;  	//低压传感器值
	U16 freq_Out;			//输出频率
	U16 current_out;		//输出电流
	U16 voltage_out;		//输出电压

public:
	U16 getFrequency();
	U16 getFreqState();
};

class CompSW
{
public:
};

#endif //__AC_COMPRESSOR_H__
