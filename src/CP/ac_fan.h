#ifndef __AC_FAN_H__
#define __AC_FAN_H__
#include "ac_include.h"



#define SupplyAirFanContactorError 0x1		//送风机接触器故障
#define SupplyAirFanFeedbackError 0x2		//送风机反馈故障
#define CondenserFanContactorError 0x4		//冷凝风机接触器故障
#define CondenserFanRedundancyContactorError 0x8	//冷凝风机冗余接触器故障


class Fan {
public:
	BOOL isOn();

	void On();
	void Off(BOOL Force);
	BOOL isRun();
	BOOL getErr();
	U16 getSpeed();
	void setSpeed(U16 tempSpeed);

	void setRunStatus(BOOL tempStatus);
	BOOL getRunStatus();
public:
	U8 Fan_Type; //0-送风机，1-冷凝风机
	AC_TIMER timer;
	U8 DO_run;
	U8 DO_Redundance_run;
	U8 DI_feedback;
	u8 DI_Redundance_Feedback;
	const char* name;
	U16 speed;
	U8 Error_Flag;
	U8 FanNumber;
	U8 SetNumber;
	U32 offTime;

	AC_TIMER FaultTimer;

private:

	BOOL runStatus;
};









#endif//__AC_FAN_H__
