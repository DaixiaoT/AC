#ifndef _AC_HEAT_H_
#define _AC_HEAT_H_
#include "ac_include.h"

#define HeaterContactorError 0x1 // 接触器故障

class Heater
{
public:
	void Off();
	BOOL checkError();
	BOOL isOn();
	BOOL isRun();
	BOOL isRunErr();
	void On();
	BOOL getErr();
	AC_TIMER timer;
	U8 Error_Flag;
	U8 HeaterNumber;
	U8 SetNumber;
	U8 DO_run;
	U8 DI_feedback;
	AC_TIMER FaultTimer;

private:
};

#endif
