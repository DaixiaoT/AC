#ifndef __AC_SET_H__
#define __AC_SET_H__

#include "ac_include.h"
#include "ac_damp.h"
#include "ac_fan.h"
#include "ac_compressor.h"
#include "ex_module485.h"

class ACSet
{
public:
    Freq_Module freq1, freq2;
    Valve_Module valve1, valve2;
    FRESH_AIR_DAMP FreshAirDamp;           // �·���
    Fan Ventilator_1, Ventilator_2;        // ͨ���
    Compressor Compressor_1, Compressor_2; // ѹ����
};

#endif //__AC_SET_H__
