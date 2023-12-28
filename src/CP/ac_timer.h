#ifndef __AC_TIMER_H__
#define __AC_TIMER_H__

#include "ac_include.h"

class AC_TIMER
{
public:
    U32 GetTotalRunTime();
	U32 GetThisRunTime();
    U32 GetLastOpenTime();
    BOOL IsOn();
    void Start();
    void Stop();
    void Clear();
    void Init(const char* temp_name, U32 temp_thisRunTime, U32 temp_totalRunTime, BOOL temp_IsStopClear);
    
private:
    const char* name;
    BOOL IsStopClear;
    U32 onTimeMark;
    U8 status;
    U32 thisRunTime;
    U32 totalRunTime;
    U32 lastOpenTime;
    U32 onTime;
};

#endif //__AC_TIMER_H__
