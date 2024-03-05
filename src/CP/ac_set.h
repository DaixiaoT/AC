#ifndef __AC_SET_H__
#define __AC_SET_H__

#include "ac_include.h"
#include "ac_damp.h"
#include "ac_fan.h"
#include "ac_compressor.h"


#include "485/485_freq.h"
#include "485/485_valve.h"


#include "ac_heat.h"
class ACSet
{
public:
    Heater heater1, heater2;                                   // 电加热
    Freq_Module freq1_485module, freq2_485module;              // 变频器485通信模块
    Valve_Module valve1_485module, valve2_485module;           // 膨胀阀485通信模块
    FRESH_AIR_DAMP FreshAirDamp;                               // 新风门
    Fan Ventilator_1, Ventilator_2;                            // 通风机
    Compressor Compressor_1, Compressor_2;                     // 压缩机
    //EEV_Valve EEV1, EEV2;                                    // 膨胀阀
    Fan Condenser1, Condenser2;                                // 冷凝风机
    Valve valve[VALVE_NUM];
    Freq freq[FREQ_NUM];

    S16 Td; //目标温度
    S16 Tf; //新风温度
    S16 Ts; //送风温度
    S16 Tg; //回风温度


public:
    void setTemp(S16 tempTf, S16 tempTr, S16 tempTs);
    
};

#endif //__AC_SET_H__
