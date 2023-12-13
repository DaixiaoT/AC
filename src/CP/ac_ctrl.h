#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"

enum HAND_MODE{
    HAND_HAND=2,              //返強返強
    HAND_AUTO=5,              //返強徭強
    HAND_OFF=0,               //返強購字
};

enum AC_MODE {
    
};







class TRDP_IN
{
public:

private:
};

class ACControl
{
public:
    ACSet set1;
    TRDP_IN trdp;
    HAND_MODE handMode;

    AC_MODE prev_ctrlMode;
    S16 Td;
    S16 Tf;
    S16 Ts;
    S16 Tg;

public:
    void Init();
    void ProcessMode();
    BOOL SelfTestMode();
    void ToVent();
    void ToMode(AC_MODE mode);
    void SetTemp(S16 Tf, S16 Tg, S16 Ts);

private:
    HAND_MODE UpdateHandMode();

};

extern ACControl g_car;


#endif //__AC_CRTL_H__
