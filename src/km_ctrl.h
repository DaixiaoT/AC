#ifndef __DO_KM_CTRL_H__
#define __DO_KM_CTRL_H__
#include "ac_include.h"

// 继电器控件

typedef enum
{
    KMErr_Null = 0, // 没故障
    KMErr_Feekback, // 没有反馈故障
    KMErr_Release,  // 断开后释放故障
    KMErr_Other,    // 其他故障
} KMErr_t;

class km_ctrl
{
public:
    km_ctrl()
    {

    }

    /*
     *outNo:DO位
     *inFeekback：反馈DI位
     *fb_type:是触点类型，常开，常闭
     *err_stop:故障时是否停机
     *delay_ms:动作后delay_ms毫秒后检测错误
     *retry_ms：错误后等待retry_ms毫秒后检测错误
     *try_max_cnt:错误重试最try_max_cnt次数，仍有故障后锁死
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
    U8 m_run_stat; // 当前动作状态
    U8 m_outDo;
    U8 m_feekbackDi;
    U32 m_action_delay_ms;        // 动作时延时检测时间
    U32 m_error_delay_ms;         // 故障时延时检测时间

    BOOL m_err_stop; // 故障是否停机

    U32 m_on_time;
    U32 m_off_time;
    U32 m_err_time; // 故障时间计时
    BOOL m_err_flag;
    U8 m_errcnt;          // 故障计数
    KMErr_t m_errlock;    // 故障锁死，1
    U8 m_errlock_cnt_max; // 故障最大次数后锁死

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
     *outNo:DO位
     *inFeekback：反馈DI位
     *fb_type:是触点类型，常开，常闭
     *err_stop:故障时是否停机
     *delay_ms:动作后delay_ms毫秒后检测错误
     *retry_ms：错误后等待retry_ms毫秒后检测错误
     *try_max_cnt:错误重试最try_max_cnt次数，仍有故障后锁死
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
    U32 m_km_num; // 接触器次数
    KMErr_t ckeckErr()
    {
        
    }
};

#endif //__DO_KM_CTRL_H__
