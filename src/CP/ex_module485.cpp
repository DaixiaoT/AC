#include "ex_module485.h"

Valve_module valve[VALVE_NUM];

// BOOL valve1_timeout()
// {
//     return TimeGap(valve[0].time_out) > 30000;
// }
// BOOL valve2_timeout()
// {
//     return TimeGap(valve[1].time_out) > 30000;
// }

BOOL Valve_module::valve1_timeout()
{
    return TimeGap(valve[0].time_out) > 30000;
}

BOOL Valve_module::valve2_timeout()
{
    return TimeGap(valve[1].time_out) > 30000;
}
