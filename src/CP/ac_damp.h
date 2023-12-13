#ifndef _AC_DAMP_H_
#define _AC_DAMP_H_

#include "ac_include.h"

// 新风阀
class FRESH_AIR_DAMP
{
public:
    const char* name;
    U8 DO_Open;
    U8 DO_Close;
    U8 checkNum;//>0时，进入自检
    U8 SelfCheckState;//风机自检状态
    U8 SelfCheckFinished;
    AC_TIMER SelfCheckTime;
    U16 resistor_feedback; // 新风阀电阻反馈
    U32 DO_OpenTime;//DO_Open=1时开始计时
    U32 DO_CloseTime;//DO_Close=1时开始计时
    U8 checkError[2];//自检故障计数
    U16 test;
    U8  DampOpenAllTime;
    void DoSelfCheck();
    void RunOpen();
    void RunClose();
    BOOL OpenOK();
    BOOL CloseOK();
    void Init();
    // U16 getResistorFeedback();
    // void setResistorFeedback(U16 value);
private:
    

};


#endif // _AC_DAMP_H_
