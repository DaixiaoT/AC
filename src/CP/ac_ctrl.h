#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"

enum SET_MODE{
    SET_FORCE_STOP=0,  // 强制停机 
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
    S16 Td;
    S16 Tf;
    S16 Ts;
    S16 Tg;

public:
    void Init();
    void ProcessMode();
    BOOL SelfTestMode();
    void ToVent();
    void ToMode(SET_MODE mode);
    void SetTemp(S16 Tf, S16 Tg, S16 Ts);
};

extern ACControl g_car;


#endif //__AC_CRTL_H__
