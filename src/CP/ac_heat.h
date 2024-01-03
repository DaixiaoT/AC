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
	AC_TIMER timer;
	void On();
	
	U8 DO_run;
	U8 DI_feedback;
private:
	
	
};


#endif 

