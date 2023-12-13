#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__
#include "ac_include.h"

//#define gRDampTimeOut (DampOpenAllTime + 100) // ((g_parm.RDampOpenTime_s+50)*100)  // 风阀超时时间
#define gRDampTimeOut 40000
#define AC_PARM_MARK 0x8fe1332e

#ifdef _SPI_FRAM_
#define AC_PARM_ADDR (0)
#define AC_RUN_INFO_ADDR (0x200)
#endif // _SPI_FRAM_

#pragma pack(push, 1)
struct AC_PARM
{
	U32 mark;
	U8	ModeSwitchDelay_s;
	U16 FreshAirDampAllOpenTime_s;
	U16 crc;
};
#pragma pack(pop)


#define AC_ERR_RECORD_MARK 0x9a8d
#define AC_RUN_RECORD_MARK 0xaa8d

// #include "trdp_data.h"
#pragma pack(push, 1)

struct ERR_Record_DATA
{
	U8 cp1_contactor_err : 1;         // cp1接触器故障
	U8 cp2_contactor_err : 1;         // cp2接触器故障
	U8 cp3_contactor_err : 1;         // cp3接触器故障
	U8 cp4_contactor_err : 1;         // cp4接触器故障
	U8 heater1_contactor_err : 1;     // 加热1接触器故障
	U8 heater2_contactor_err : 1;     // 加热2接触器故障
	U8 condensor1_contactor_err : 1;  // 冷凝接触器1故障
	U8 condensor1_Redundance_err : 1; // 冷凝接触器用余1故障

	U8 condensor2_contactor_err : 1;  // 冷凝接触器2故障
	U8 condensor2_Redundance_err : 1; // 冷凝接触器2沉余故障
	U8 supply1_contactor_err : 1;     // 送风1接触器故障
	U8 supply1_Redundance_err : 1;    // 送风1接用余触器故障
	U8 supply2_contactor_err : 1;     // 送风2接触器故障
	U8 supply2_Redundance_err : 1;    // 送风2接用余触器故障
	U8 supply1_state_err : 1;         // 送风1机故障反馈
	U8 supply2_state_err : 1;         // 送风2机故障反馈

	U8 ESF1_Contactor_err : 1; // 排沙风机接触器故障反馈
	U8 EAF1_Contactor_err : 1; // 废排风机接触器故障反馈
	U8 FAF1_Contactor_err : 1; // 引流风机接触器故障反馈
	U8 FPPV_err : 1;	   // 新风PPV故障
	U8 EPPV_err : 1;	   // 废排PPV故障
	U8 Power_3P_error : 1;	   // 3相PPV故障
	U8 cp1_hp_err : 1;	   // cp1高压故障
	U8 cp2_hp_err : 1;	   // cp2高压故障

	U8 cp3_hp_err : 1;   // cp3高压故障
	U8 cp4_hp_err : 1;   // cp4高压故障
	U8 cp1_lp_err : 1;   // cp1低压故障
	U8 cp2_lp_err : 1;   // cp2低压故障
	U8 cp3_lp_err : 1;   // cp3低压故障
	U8 cp4_lp_err : 1;   // cp4低压故障
	U8 HVAC_hp1_err : 1; // cp1 传感器高压故障
	U8 HVAC_hp2_err : 1; // cp2 传感器高压故障

	U8 HVAC_hp3_err : 1;	    // cp3 传感器高压故障
	U8 HVAC_hp4_err : 1;	    // cp4 传感器高压故障
	U8 SA_temperature_err : 1;  // 送风 传感器高压故障
	U8 FA_temperature_err : 1;  // 新风 传感器高压故障
	U8 RA1_temperature_err : 1; // 客室1传感器高压故障
	U8 RA2_temperature_err : 1; // 客室2 传感器高压故障
	U8 RA3_temperature_err : 1; // 客室3 传感器高压故障
	U8 RA4_temperature_err : 1; // 客室4 传感器高压故障

	U8 RA5_temperature_err : 1; // 客室5 传感器高压故障
	U8 CO2_sensor_err : 1;	    // CO2 传感器高压故障
	U8 HMI_timeout_err : 1;	    // 触摸屏 传感器故障
	U8 cp1_FI_timeout_err : 1;  // cp1变频器故障
	U8 cp2_FI_timeout_err : 1;  // cp2变频器故障
	U8 cp3_FI_timeout_err : 1;  // cp3变频器故障
	U8 cp4_FI_timeout_err : 1;  // cp4变频器故障
	U8 cp1_EEV_timeout_err : 1; // cp1膨胀阀故障

	U8 cp2_EEV_timeout_err : 1; // cp2膨胀阀故障
	U8 cp3_EEV_timeout_err : 1; // cp3膨胀阀故障
	U8 cp4_EEV_timeout_err : 1; // cp4膨胀阀故障
	U8 RA_damper_err : 1;	    // 回风阀故障
	U8 res_err : 4;

}; // 7字节

// 最大一次写256字节
struct AC_ERR_Record
{	           // 目前定义为故障数据
	U16 mark;  // 2 B
	U8 rtc[6]; // 8 B
	U8 res[2]; // 预留
	ERR_Record_DATA u1_err;
	ERR_Record_DATA u2_err;

	U8 res_e[230]; // 目前flash最大一次写256字节，需要优化
	U16 crc;
};

#define RUN_DATA_LEN 245
struct AC_RUN_Record
{	           // 定义为运行数据
	U16 mark;  // 2 B
	U8 rtc[6]; // 8 B

	U8 data[RUN_DATA_LEN]; // 最多246字节 保存的是维护软件设备状态数据包
	U8 data_type;          // 0:运行记录   非零：其他记录
	U16 crc;
};

#pragma pack(pop)


extern AC_PARM g_parm;

#endif // !__AC_CONFIG_H__
