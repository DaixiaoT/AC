#include "ac_timer.h"

U32 AC_TIMER::GetRunTime()
{
    return RunTime;
}

void AC_TIMER::AdjustRunTime()
{
    if (!IsOn())
    {
        return;
    }
    U8 gap = TimeGap(RunTime) / 1000;
    if (gap >= 1)
    {
        RunTime++;
    }
}

BOOL AC_TIMER::IsOn()
{
    return state;
}

void AC_TIMER::Start()
{
    if(IsOn()){
        AdjustRunTime();
        return;
    }
    state=TRUE;
}
