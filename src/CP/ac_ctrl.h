#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"
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

public:
    void Init();
    void ProcessMode();
    BOOL SelfTestMode();
};

extern ACControl g_car;


#endif //__AC_CRTL_H__
