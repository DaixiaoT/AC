
#include "ac_damp.h"
#include "ac_ctrl.h"
U16 EEV_Valve::getSuperheat()
{
    return superheat;
}

//not complete
void FRESH_AIR_DAMP::DoSelfCheck()
{

    LOG_PRINT("Enter FRESH_AIR_DAMP::DoSelfCheck()State:%d\n", SelfCheckState);
    switch (SelfCheckState)
    {
    case 0:
        Open();
        if (Opened())
        {
            SelfCheckState = 1;
        }
        break;
    case 1:
        Close();
        if (Closed())
        {
            SelfCheckState = 2;
        }
    }
}
void FRESH_AIR_DAMP::Open()
{
    LOG_PRINT("Enter FRESH_AIR_DAMP::Open()\n");
    DO_SET(DO_Open);
    DO_CLR(DO_Close);
}

void FRESH_AIR_DAMP::Close()
{
    LOG_PRINT("Enter FRESH_AIR_DAMP::Close()\n");

    DO_CLR(DO_Open);
    DO_SET(DO_Close);
}

BOOL FRESH_AIR_DAMP::Opened()
{
    LOG_PRINT("Enter FRESH_AIR_DAMP::Opened()\n");

    return resistor_feedback >= 4500;
}

BOOL FRESH_AIR_DAMP::Closed()
{
    LOG_PRINT("Enter FRESH_AIR_DAMP::Closed()\n");

    return resistor_feedback <= 500;
}

// U16 FRESH_AIR_DAMP::getResistorFeedback()
// {
//     return resistor_feedback;
// }

// void FRESH_AIR_DAMP::setResistorFeedback(U16 value)
// {
//     resistor_feedback = value;
// }

void FRESH_AIR_DAMP::Init()
{
    LOG_PRINT("FreshAirDamp Init()\n");
    resistor_feedback=222;
}
