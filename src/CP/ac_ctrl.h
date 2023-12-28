#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
#include "ac_set.h"
#include "ac_pictrl.h"
enum HAND_MODE{
    HAND_HAND=2,              //�ֶ��ֶ�
    HAND_AUTO=5,              //�ֶ��Զ�
    HAND_OFF=0,               //�ֶ��ػ�
};



enum SET_MODE {
    SET_FORCE_STOP = 0, // ͣ��
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
    SET_EM_COOL,		//Ӧ��ģʽ
};











class ACControl
{
public:
    ACSet set1;
    TRDP_IN trdp;
    HAND_MODE handMode;
    TRDP_MODE trdpMode;
    PI_Control PI_set1;
    SET_MODE ctrlMode;
    SET_MODE prev_ctrlMode;
    S16 acu;//PI ������
    //S16 Td;//�趨�¶�
    //S16 Tf;//�·��¶�
    //S16 Ts;//�ͷ��¶�
    //S16 Tg;//�ط��¶�
    U8 handTemperature;
    U32 selfTestStartTime;
    U32 selfTestEndTime;
    BOOL selfTestFlag;
    U32 selfTestLastTime;
public:
    void Init();
    S16 car_Outtemp();
    S16 car_Intemp();
    void ProcessMode();
    BOOL SelfTestMode();
    void ToStop();
    void ToVent();
    void ToFullHeat();
    void ToHalfHeat();
    void ToHalfCool();
    void ToFullCool();
    void ToAuto();
    S16 get_Tf1();//�·��¶�
    S16 get_Ts1();//�ͷ��¶�
    S16 get_Tg1();//�ط��¶�1
    S16 get_Tg2();//�ط��¶�2
    S16 get_mix_Tf(); //����·�
    S16 get_mix_TS(); //����ͷ�
    S16 get_mix_Tg(); //��Ͽ���
    void GetHandTemp();//��ȡ�ֶ��趨�¶�ֵ
   
private:
    HAND_MODE UpdateHandMode();

private:
    

};

extern ACControl g_car;


#endif //__AC_CRTL_H__
