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
    Freq_Module freq1_485module, freq2_485module;              // 变频器485通信模块
    Valve_Module valve1_485module, valve2_485module;           // 膨胀阀485通信模块
    FRESH_AIR_DAMP FreshAirDamp;           // 新风门
    Fan Ventilator_1, Ventilator_2;        // 通风机
    Compressor Compressor_1, Compressor_2; // 压缩机
    //EEV_Valve EEV1, EEV2;                  // 膨胀阀
    Valve valve[VALVE_NUM];
    Freq freq[FREQ_NUM];

public:
    
};

#endif //__AC_SET_H__
