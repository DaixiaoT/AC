
#include "ai_update.h"

AI_CTRL ctrl_AI[AI_NUM];

void AI_init()
{
}

void AI_Updata()
{
}

U16 ReadFreshAirDampFeedback()
{
    return (U16)ctrl_AI[3].getValue();
}

U16 ReadHighPressureValue_1()
{
    return ctrl_AI[6].getValue();
}
U16 ReadHighPressureValue_2()
{
    return ctrl_AI[7].getValue();
}

void ai_init()
{
}

