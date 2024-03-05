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
    Heater heater1, heater2;                                   // �����
    Freq_Module freq1_485module, freq2_485module;              // ��Ƶ��485ͨ��ģ��
    Valve_Module valve1_485module, valve2_485module;           // ���ͷ�485ͨ��ģ��
    FRESH_AIR_DAMP FreshAirDamp;                               // �·���
    Fan Ventilator_1, Ventilator_2;                            // ͨ���
    Compressor Compressor_1, Compressor_2;                     // ѹ����
    //EEV_Valve EEV1, EEV2;                                    // ���ͷ�
    Fan Condenser1, Condenser2;                                // �������
    Valve valve[VALVE_NUM];
    Freq freq[FREQ_NUM];

    S16 Td; //Ŀ���¶�
    S16 Tf; //�·��¶�
    S16 Ts; //�ͷ��¶�
    S16 Tg; //�ط��¶�


public:
    void setTemp(S16 tempTf, S16 tempTr, S16 tempTs);
    
};

#endif //__AC_SET_H__
