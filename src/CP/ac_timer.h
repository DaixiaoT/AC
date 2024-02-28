#ifndef __AC_TIMER_H__
#define __AC_TIMER_H__

#include "sys_api.h"

class AC_TIMER {
public:


	BOOL IsOn(); 

	void ctrl(BOOL b);
	

	void Start();

	void Stop();
	void AdjustRunTime();

	void Clear();

	U32 getTotalRunTime();

	U32 getThisRunTime();

	U32 getOnCnt();
	U32 getOffTime();
	U32 getOnTime();


	void init(const char* Tname, U32 TthisRunTime, U32 TtotalRunTime, BOOL TIsStopClear, U32 TonCnt = 0);
private:
	const char* name;
	BOOL status;
	BOOL IsStopClear;		//停止是否清零
	U32 onTimeMark;
	U32 totalRunTime;		//总运行时间		单位秒
	U32 thisRunTime;		//本次上电的运行时间 单位秒
	U32 onTime;
	U32 offTime;
	U32 onCnt;
};


#endif//__AC_TIMER_H__






