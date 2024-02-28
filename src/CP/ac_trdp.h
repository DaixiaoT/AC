#ifndef __AC_TRDP_H__
#define __AC_TRDP_H__
#include "ac_include.h"

#define TRDP_OK_BIT 0x1
#define TRDP_CP_EN_BIT 0x2
#define TRDP_CAR_WASH_BIT 0x4
#define TRDP_SLV1_OK_BIT 0x8
#define TRDP_SLV2_OK_BIT 0x10
#define TRDP_SET_TD_EN 0x20
#define TRDP_OPEN_DOOR_BIT 0x40
#define TRDP_CLOSE_DOOR_BIT 0x80
#define TRDP_ALL_CLOSE_BIT 0x100
#define TRDP_TEMP_IN 0x200
#define TRDP_TEMP_OUT 0x400
#define TRDP_SET_TEMP 0x800
#define TRDP_SET_MODE 0x1000
#define TRDP_CARLOAD 0x2000
#define TRDP_MODE_TEMP_BIT 0x4000
#define TRDP_EM_FAN 0x8000
#define TRDP_EM_COOL 0x10000
#define TRDP_OK_BIT_F TRDP_OK_BIT
#define TRDP_CP_EN_BIT_F TRDP_CP_EN_BIT
#define TRDP_CAR_WASH_BIT_F TRDP_CAR_WASH_BIT
#define TRDP_SLV1_OK_BIT_F TRDP_SLV1_OK_BIT
#define TRDP_SLV2_OK_BIT_F TRDP_SLV2_OK_BIT
#define TRDP_OPEN_DOOR_BIT_F TRDP_OPEN_DOOR_BIT
#define TRDP_CLOSE_DOOR_BIT_F TRDP_CLOSE_DOOR_BIT
#define TRDP_ALL_CLOSE_BIT_F TRDP_ALL_CLOSE_BIT
#define TRDP_MODE_TEMP_BIT_F TRDP_MODE_TEMP_BIT

#define TRDP_TEMP_IN_F TRDP_TEMP_IN
#define TRDP_TEMP_OUT_F TRDP_TEMP_OUT
#define TRDP_SET_TEMP_F TRDP_SET_TEMP
#define TRDP_SET_MODE_F TRDP_SET_MODE
#define TRDP_CARLOAD_F TRDP_CARLOAD
#define TRDP_EM_FAN_F TRDP_EM_FAN
#define TRDP_EM_COOL_F TRDP_EM_COOL
#define TRDP_SET_TD_EN_F TRDP_SET_TD_EN

#define TRDP_FIRE_IN_F TRDP_FIRE_IN	          // 内部火灾强制
#define TRDP_HEAT_DISABLE_F TRDP_HEAT_DISABLE // 电加热不启动

enum TRDP_MODE {
    TRDP_FULL_HEAT = 1,   //全热
    TRDP_HALF_HEAT,     //半热
    TRDP_COOL,          //制冷
    TRDP_VENT,          //通风
    TRDP_AUTO,          //自动
    TRDP_STOP,          //停止
    TRDP_FIRE,          //火灾
    TRDP_EMV,           //紧急通风
    TRDP_WASH           //洗车
};

class TRDP_IN
{
public:
    void Set_Mode(TRDP_MODE m);
    void Set_OK(BOOL b);
    BOOL OK();
    BOOL Get_modeTemp();
    S16 Get_InTemp();
    S16 Get_OutTemp();
    void Set_OutTemp(U8 t);
    void Set_Compressor_Enable(BOOL b);
    void Set_TempMode_Enable(BOOL b);
    TRDP_MODE Get_Mode();
    BOOL Get_Compressor_Enable();
private:
    TRDP_MODE mode, mode_f;	   // TCMS模式
    S16 Td, Td_f;	   // 目标温度
    S16 INTemp, INTemp_f;	   // 室内温度
    S16 OUTTemp, OUTTemp_f;	   // 室内温度
    S16 Uic, Uic_f;	   // UIC偏移
               // U8        carLoad, carLoad_f; // 车辆载荷，用于控制风门开度，0空载~100满载
    U8 sivRunNum, sivRunNum_f; // SIV运行数量
    U32 flag, flag_f, flag_fv;
    BOOL carfway, carbway; // 车辆前进后退
    U16 car_speed;
};













#endif // !__AC_TRDP_H__

