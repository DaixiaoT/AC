
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
    LOG_PRINT("selfTestFlag:%d\n", flag);
    LOG_PRINT("freshAirDampResistor:%d",g_car.set1.FreshAirDamp.resistor_feedback);
    LOG_PRINT("Compressor frequency:%d\n",g_car.set1.Compressor_1.freq_HZ);
}

BOOL ACControl::SelfTestMode()
{
    LOG_PRINT("Enter ACControl::SelfTestMode() SelfCheckTime=%d\n",set1.FreshAirDamp.SelfCheckTime.GetRunTime());
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

/*

selfTestFinished

firstboot



*/
