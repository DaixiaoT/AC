#ifndef __DO_KM_CTRL_H__
#define __DO_KM_CTRL_H__
#include "ac_include.h"

// �̵����ؼ�

typedef enum
{
    KMErr_Null = 0, // û����
    KMErr_Feekback, // û�з�������
    KMErr_Release,  // �Ͽ����ͷŹ���
    KMErr_Other,    // ��������
} KMErr_t;

class km_ctrl
{
public:
    km_ctrl()
    {

    }

    /*
     *outNo:DOλ
     *inFeekback������DIλ
     *fb_type:�Ǵ������ͣ�����������
     *err_stop:����ʱ�Ƿ�ͣ��
     *delay_ms:������delay_ms����������
     *retry_ms�������ȴ�retry_ms����������
     *try_max_cnt:����������try_max_cnt���������й��Ϻ�����
     **/


    KMErr_t getErr()
    {
        return m_errlock;
    }

    BOOL get_errflg()
    {
        return m_err_flag;
    }

    U32 get_Ontime()
    {
        return m_on_time;
    }

    U32 get_Offtime()
    {
        return m_off_time;
    }

    BOOL isOnTime(U32 ms)
    {

    }

    BOOL isOffTime(U32 ms)
    {

    }

    void On()
    {
      
    }

    BOOL IsOn()
    {

    }

    void Off()
    {
    
    }

    void set_err(KMErr_t err)
    {
        m_errlock = err;
    }

private:
    char *m_name;
    U8 m_run_stat; // ��ǰ����״̬
    U8 m_outDo;
    U8 m_feekbackDi;
    U32 m_action_delay_ms;        // ����ʱ��ʱ���ʱ��
    U32 m_error_delay_ms;         // ����ʱ��ʱ���ʱ��

    BOOL m_err_stop; // �����Ƿ�ͣ��

    U32 m_on_time;
    U32 m_off_time;
    U32 m_err_time; // ����ʱ���ʱ
    BOOL m_err_flag;
    U8 m_errcnt;          // ���ϼ���
    KMErr_t m_errlock;    // ����������1
    U8 m_errlock_cnt_max; // ����������������

    void clear_error()
    {
        m_err_time = 0;
        m_errcnt = 0;
    }

    BOOL IsFeekbackOK()
    {
    }

    KMErr_t ckeckErr()
    {
    }
};

class BK_ctrl
{
public:
    BK_ctrl()
    {
    }

    /*
     *outNo:DOλ
     *inFeekback������DIλ
     *fb_type:�Ǵ������ͣ�����������
     *err_stop:����ʱ�Ƿ�ͣ��
     *delay_ms:������delay_ms����������
     *retry_ms�������ȴ�retry_ms����������
     *try_max_cnt:����������try_max_cnt���������й��Ϻ�����
     **/


    KMErr_t getErr()
    {
    }

    BOOL get_errflg()
    {
    }

    U32 get_Ontime()
    {
    }

    U32 get_Offtime()
    {
    }

    BOOL IsOn()
    {
    }

    BOOL isOnTime(U32 ms)
    {
    }

    BOOL isOffTime(U32 ms)
    {
    }

    void On()
    {
    }

    void Off()
    {
    }


private:
    

    void clear_error()
    {

    }

    BOOL IsFeekbackOK()
    {
        
    }

public:
    U32 m_km_num; // �Ӵ�������
    KMErr_t ckeckErr()
    {
        
    }
};

#endif //__DO_KM_CTRL_H__
