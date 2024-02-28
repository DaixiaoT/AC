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
	BOOL IsStopClear;		//ֹͣ�Ƿ�����
	U32 onTimeMark;
	U32 totalRunTime;		//������ʱ��		��λ��
	U32 thisRunTime;		//�����ϵ������ʱ�� ��λ��
	U32 onTime;
	U32 offTime;
	U32 onCnt;
};


#endif//__AC_TIMER_H__






