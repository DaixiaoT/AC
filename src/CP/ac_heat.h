#ifndef _AC_HEAT_H_
#define _AC_HEAT_H_
#include "ac_include.h"

class Heater
{
public:
	
	void Off();
	BOOL checkError();
	BOOL isOn();
	BOOL isRun();
	BOOL isRunErr();
	void On();
	U32 getTotalRunTime();
	void setTotalRunTime(U32 tempTotalRunTime);
	U8 DO_run;
	U8 DI_feedback;
private:
	
	U32 lastOnTime;
	U32 lastOffTime;
	U32 totalRunTime;
};


#endif 

