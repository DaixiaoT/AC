#ifndef __AC_COMPRESSOR_H__
#define __AC_COMPRESSOR_H__
#include "ac_include.h"

// 压缩机
class Compressor
{
public:
	U8 DI_feedback;
	BOOL firstStart;		// 首次启动，最小频率，40hz，变频器，运行1分钟。如果上电就低频率运行会导致润滑不足
	U8 DO_run;				// 压缩机控制输出位  (在DO中的位置)
	U16 freq_HZ;			//变频器频率
	U16 freq_State;			//变频器状态
	S16 HighPressureValue; 	//高压传感器值
	S16 LowPressureValue;  	//低压传感器值
	U16 freq_Out;			//输出频率
	U16 current_out;		//输出电流
	U16 voltage_out;		//输出电压

	U32 lastSetFrequencyTime;			//上次设置频率时间
	S16 HP_Value;			//高压传感器值
	S16 LP_Value;			//低压传感器值
	BOOL HP_lock;			//高压锁死，未报故障，做逻辑判断用
	BOOL LP_lock;			//低压锁死，未报故障，做逻辑判断用
	U8 HP_trigger;			// 触发6次制冷系统高压压力异常预警
	U8 LP_trigger;			// 触发6次制冷系统低压压力异常预警
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
