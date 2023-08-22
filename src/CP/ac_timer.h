#ifndef __AC_TIMER_H__
#define __AC_TIMER_H__

#include "ac_include.h"

class AC_TIMER
{
public:
	U32 GetRunTime();
    void AdjustRunTime();
    BOOL IsOn();
    void Start();
    
private:
    U8 state;
    U32 RunTime;
};

#endif //__AC_TIMER_H__
