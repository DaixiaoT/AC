#ifndef _AC_DAMP_H_
#define _AC_DAMP_H_

#include "ac_include.h"

// 新风阀
class FRESH_AIR_DAMP
{
public:
    U8 DO_Open;
    U8 DO_Close;
    U8 SelfCheckState;
    U8 SelfCheckFinished;
    AC_TIMER SelfCheckTime;
    U16 resistor_feedback; // 新风阀电阻反馈
    void DoSelfCheck();
    void Open();
    void Close();
    BOOL Opened();
    BOOL Closed();
    void Init();
    // U16 getResistorFeedback();
    // void setResistorFeedback(U16 value);
private:
    

};

class EEV_Valve
{
public:
    U16 getSuperheat();
    U16 superheat;
private:
    
};

#endif // _AC_DAMP_H_
