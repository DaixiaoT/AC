#ifndef _AC_DAMP_H_
#define _AC_DAMP_H_

#include "ac_include.h"

// �·緧
class FRESH_AIR_DAMP
{
public:
    const char* name;
    U8 DO_Open;
    U8 DO_Close;
    U8 checkNum;//>0ʱ�������Լ�
    U8 SelfCheckState;//����Լ�״̬
    U8 SelfCheckFinished;
    AC_TIMER SelfCheckTime;
    U16 resistor_feedback; // �·緧���跴��
    U32 DO_OpenTime;//DO_Open=1ʱ��ʼ��ʱ
    U32 DO_CloseTime;//DO_Close=1ʱ��ʼ��ʱ
    U8 checkError[2];//�Լ���ϼ���
    U16 test;
    U8  DampOpenAllTime;
    void DoSelfCheck();
    void RunOpen();
    void RunClose();
    BOOL OpenOK();
    BOOL CloseOK();
    void Init();
    // U16 getResistorFeedback();
    // void setResistorFeedback(U16 value);
private:
    

};


#endif // _AC_DAMP_H_
