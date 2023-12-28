#include "ac_timer.h"

U32 AC_TIMER::GetTotalRunTime()
{
    return totalRunTime;
}

U32 AC_TIMER::GetThisRunTime()
{
    return thisRunTime;
}

U32 AC_TIMER::GetLastOpenTime()
{
    return lastOpenTime;
}



BOOL AC_TIMER::IsOn()
{
    return status;
}

void AC_TIMER::Start()
{
    if(IsOn()){
        //
        return;
    }
    status=TRUE;
}

void AC_TIMER::Stop()
{
    if (!IsOn()) {
        return;
    }
    status = FALSE;

    if (IsStopClear)
    {
        Clear();
    }
    else
    {
        totalRunTime += (int)(TimeGap(onTimeMark) / 1000);
        thisRunTime += (int)(TimeGap(onTimeMark) / 1000);
    }
}

void AC_TIMER::Clear()
{
    totalRunTime = 0;
    thisRunTime = 0;
    onTime = sys_time();
}

void AC_TIMER::Init(const char* temp_name, U32 temp_thisRunTime, U32 temp_totalRunTime, BOOL temp_IsStopClear)
{
    name = temp_name;
    status = FALSE;
    totalRunTime = temp_totalRunTime;
    thisRunTime = temp_thisRunTime;
    IsStopClear = temp_IsStopClear;
}
