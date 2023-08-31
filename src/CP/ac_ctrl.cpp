
#include "ac_ctrl.h"
ACControl g_car;
void ACControl::Init()
{
    LOG_PRINT("ACControl Init()\n");
    g_car.set1.FreshAirDamp.DO_Open = DO_FAS_Open;
    g_car.set1.FreshAirDamp.DO_Close = DO_FAS_Close;
    g_car.set1.FreshAirDamp.Init();
}

void ACControl::ProcessMode()
{
    LOG_PRINT("Enter ACControl::ProcessMode()\n");

    U8 flag = -1;
    flag = g_car.SelfTestMode();
    // LOG_PRINT("selfTestFlag:%d\n", flag);
    // LOG_PRINT("freshAirDampResistor:%d",g_car.set1.FreshAirDamp.resistor_feedback);
    // LOG_PRINT("Compressor frequency:%d\n",g_car.set1.Compressor_1.freq_HZ);
    g_car.SetTemp(1, 2, 3);
    g_car.Td = 4;
}

BOOL ACControl::SelfTestMode()
{
    LOG_PRINT("Enter ACControl::SelfTestMode() SelfCheckTime=%d\n", set1.FreshAirDamp.SelfCheckTime.GetRunTime());
    set1.FreshAirDamp.DoSelfCheck();
    if (set1.FreshAirDamp.SelfCheckState == 2)
    {
        return TRUE;
    }
    else if (set1.FreshAirDamp.SelfCheckTime.GetRunTime() > 60)
    {
        return FALSE;
    }
}

void ACControl::ToVent()
{
    g_car.ToMode(SET_VENT);
}

void ACControl::ToMode(SET_MODE mode)
{
}

void ACControl::SetTemp(S16 temp_Tf, S16 temp_Tg, S16 temp_Ts)
{
    g_car.Tf = temp_Tf;
    g_car.Tg = temp_Tg;
    g_car.Ts = temp_Ts;
}

/*

selfTestFinished

firstboot



*/
