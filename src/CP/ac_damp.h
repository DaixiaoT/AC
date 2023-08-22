#ifndef _AC_DAMP_H_
#define _AC_DAMP_H_

#include "ac_include.h"

// �·緧
class FRESH_AIR_DAMP
{
public:
    U8 DO_Open;
    U8 DO_Close;
    U16 resistor_feedback; // �·緧���跴��
    U8 SelfCheckState;
    U8 SelfCheckFinished;
    AC_TIMER SelfCheckTime;
    void DoSelfCheck();
    void Open();
    void Close();
    BOOL Opened();
    BOOL Closed();
    void Init();
};

class EEV_Valve
{
public:
    U16 superheat;
    U16 getSuperheat();
};

#endif // _AC_DAMP_H_
