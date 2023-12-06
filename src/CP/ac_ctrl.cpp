#include "ac_ctrl.h"
ACControl g_car;
void ACControl::Init()
{
    LOG_PRINT("ACControl Init()\n");
    // set1.FreshAirDamp.DO_Open = DO_FAS_Open;
    // set1.FreshAirDamp.DO_Close = DO_FAS_Close;
    set1.FreshAirDamp.Init();
}
 
void ACControl::ProcessMode()
{
    LOG_PRINT("Enter ACControl::ProcessMode()\n");
    LOG_PRINT("\ngcar.set1.FreshAirDamp:%d\n",g_car.set1.FreshAirDamp.SelfCheckState);
    if (set1.FreshAirDamp.SelfCheckState == 2) {
        set1.FreshAirDamp.SelfCheckState = 0;
    }
    SelfTestMode();
    // U8 flag = -1;
    // flag = SelfTestMode();
    // // LOG_PRINT("selfTestFlag:%d\n", flag);
    // g_car.set1.FreshAirDamp.SelfCheckState++;
    LOG_PRINT("freshAirDampResistor:%d\n",set1.FreshAirDamp.resistor_feedback);
    // // LOG_PRINT("Compressor frequency:%d\n",g_car.set1.Compressor_1.freq_HZ);
    // SetTemp(1, 2, 3);
    // g_car.Td = 4;
}

BOOL ACControl::SelfTestMode()
{
     LOG_PRINT("Enter ACControl::SelfTestMode() SelfCheckTime=%d\n", set1.FreshAirDamp.SelfCheckTime.GetRunTime());
     set1.FreshAirDamp.DoSelfCheck();
    // if (set1.FreshAirDamp.SelfCheckState == 2)
    // {
    //     return TRUE;
    // }
    // else if (set1.FreshAirDamp.SelfCheckTime.GetRunTime() > 60)
    // {
    //     return FALSE;
    // }
}

void ACControl::ToVent()
{
    // ToMode(SET_VENT);
}

void ACControl::ToMode(SET_MODE mode)
{
}

void ACControl::SetTemp(S16 temp_Tf, S16 temp_Tg, S16 temp_Ts)
{
    // g_car.Tf = temp_Tf;
    // g_car.Tg = temp_Tg;
    // g_car.Ts = temp_Ts;
}

/*

selfTestFinished

firstboot



*/
