#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"
#include "ac_pictrl.h"
enum HAND_MODE{
    HAND_HAND=2,              //手动手动
    HAND_AUTO=5,              //手动自动
    HAND_OFF=0,               //手动关机
};



enum SET_MODE {
    SET_FORCE_STOP = 0, // 停机
    SET_STOP,	       // 顺序停机
    SET_VENT,	       // 通风
    SET_EM_VENT,       // 紧急通风
    SET_HALF_COOL,     // 半冷
    SET_FULL_COOL,     // 全冷
    SET_HALF_HEAT,     // 半热
    SET_FULL_HEAT,     // 全热
    SET_INIT_COOL,     // 预冷
    SET_INIT_HEAT,     // 预热
    SET_FORCE_VENT,    // 强制通风
    SET_AUTO,
    SET_EM_COOL,		//应急模式
};











class ACControl
{
public:
    ACSet set1;
    TRDP_IN trdp;
    HAND_MODE handMode;
    TRDP_MODE trdpMode;
    PI_Control PI_set1;
    SET_MODE ctrlMode;
    SET_MODE prev_ctrlMode;
    S16 acu;//PI 计算结果
    //S16 Td;//设定温度
    //S16 Tf;//新风温度
    //S16 Ts;//送风温度
    //S16 Tg;//回风温度
    U8 handTemperature;
    U32 selfTestStartTime;
    U32 selfTestEndTime;
    BOOL selfTestFlag;
    U32 selfTestLastTime;
public:
    void Init();
    S16 car_Outtemp();
    S16 car_Intemp();
    void ProcessMode();
    BOOL SelfTestMode();
    void ToStop();
    void ToVent();
    void ToFullHeat();
    void ToHalfHeat();
    void ToHalfCool();
    void ToFullCool();
    void ToAuto();
    S16 get_Tf1();//新风温度
    S16 get_Ts1();//送风温度
    S16 get_Tg1();//回风温度1
    S16 get_Tg2();//回风温度2
    S16 get_mix_Tf(); //混合新风
    S16 get_mix_TS(); //混合送风
    S16 get_mix_Tg(); //混合客室
    void GetHandTemp();//获取手动设定温度值
   
private:
    HAND_MODE UpdateHandMode();

private:
    

};

extern ACControl g_car;


#endif //__AC_CRTL_H__
