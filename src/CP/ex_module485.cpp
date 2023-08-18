#include "ex_module485.h"

Valve valve[VALVE_NUM];




void RS485_TASK(){

}

BOOL Valve::Timeout()
{
    return TimeGap(time_out_seconds)>30000;
}
