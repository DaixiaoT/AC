#ifndef __AC_FAN_H__
#define __AC_FAN_H__
#include "ac_include.h"

typedef enum
{
	FanError_Null = 0, // 没故障
	FanError_Feekback, // 没有反馈故障
	FanError_Release,  // 断开后释放故障
	FanError_Other,    // 其他故障
} FanError;
class Fan {
public:

	void On();
	void Off(BOOL Force);
	BOOL isRun();
	FanError getErr();
	U16 getSpeed();
	void setSpeed(U16 tempSpeed);

	void setRunStatus(BOOL tempStatus);
	BOOL getRunStatus();
public:
	AC_TIMER timer;
	U8 DO_run;
	U8 DI_feedback;
	const char* name;
	U16 speed;


	U32 offTime;
	FanError errLock;

private:

	BOOL runStatus;
};









#endif//__AC_FAN_H__
