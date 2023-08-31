#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"

enum SET_MODE{
    SET_FORCE_STOP=0,  // ǿ��ͣ�� 
    SET_STOP,	       // ˳��ͣ��
	SET_VENT,	       // ͨ��
	SET_EM_VENT,       // ����ͨ��
	SET_HALF_COOL,     // ����
	SET_FULL_COOL,     // ȫ��
	SET_HALF_HEAT,     // ����
	SET_FULL_HEAT,     // ȫ��
	SET_INIT_COOL,     // Ԥ��
	SET_INIT_HEAT,     // Ԥ��
	SET_FORCE_VENT,    // ǿ��ͨ��
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
