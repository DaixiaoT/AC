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
	U8 test;
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
#pragma pack(push, 1)
struct SET_TIMER
{
	U32 Cmp1;
	U32 Cmp2;
	U32 Cmp3;
	U32 Cmp4;
	U32 CF1;
	U32 CF2;
	U32 CFbak1;
	U32 CFbak2;
	U32 SF1;
	U32 SF2;
	U32 SFbak1;
	U32 SFbak2;
	U32 Heater1;
	U32 Heater2;
	U32 Power;
	U32 OffMode;
	U32 VentMode;
	U32 FullHeatMode;
	U32 HalfHeatMode;
	U32 FullCoolMode;
	U32 HalfCoolMode;
	//------------错误故障记录时间-----------
	U32 Power_error;
	U32 Freshair_error;
	U32 Supplyair_error;
	U32 R1eturnair_error;
	U32 R2eturnair_error;
	U32 R3eturnair_error;
	U32 R4eturnair_error;
	U32 R5eturnair_error;
	U32 hP1_sensor_error;
	U32 hP2_sensor_error;
	U32 hP3_sensor_error;
	U32 hP4_sensor_error;
	U32 Co2_sensor_error;
	U32 Cmp1contactor_error;
	U32 Cmp2contactor_error;
	U32 Cmp3contactor_error;
	U32 Cmp4contactor_error;
	U32 Cmp1_highpressure;
	U32 Cmp2_highpressure;
	U32 Cmp3_highpressure;
	U32 Cmp4_highpressure;
	U32 Cmp1_lowpressure;
	U32 Cmp2_lowpressure;
	U32 Cmp3_lowpressure;
	U32 Cmp4_lowpressure;
	U32 Heart1contactor_error;
	U32 Heart2contactor_error;
	U32 condensor1contactor_error;  // 默
	U32 condensorR1contactor_error; // 备
	U32 condensor2contactor_error;  // 默
	U32 condensorR2contactor_error; // 备
	U32 fan1contactor_error;        // 默认
	U32 fanR1contactor_error;       // 备份
	U32 fan2contactor_error;        // 默认
	U32 fanR2contactor_error;       // 备份
	U32 Eafancurrent_error;	        // 废排
	U32 Eafheatover_error;
	U32 Fafancurrent_error; // 引流
	U32 Fafheatover_error;
	U32 Esfancurrent_error; // 排沙
	U32 HMI_timeout_error;
	U32 FI1_timeout_error;
	U32 FI2_timeout_error;
	U32 FI3_timeout_error;
	U32 FI4_timeout_error;
	U32 EEV1_timeout_error;
	U32 EEV2_timeout_error;
	U32 EEV3_timeout_error;
	U32 EEV4_timeout_error;
	U32 RA_damper_error;
};

struct OTHER_TIMER
{
	U32 Touch_Screen;
	U32 SelfTestMode;
	U32 MaintMode;
	U32 DiagnoseMode;
	U32 AutoMode;
};

struct KM_NUM
{
	U32 Cmp1_km;
	U32 Cmp2_km;
	U32 CF1_km;
	U32 CF2_km;
	U32 CFbak1_km;
	U32 CFbak2_km;
	U32 SF1_km;
	U32 SF2_km;
	U32 Heater1_km;
	U32 Heater2_km;
};

struct AC_RUN_INFO
{
	U32 mark;

	int err_log_num;
	U32 err_log_cur_addr;

	int run_log_num;
	U32 run_log_cur_addr;

	SET_TIMER timer_u1;
	SET_TIMER timer_u2;
	OTHER_TIMER timer_other;
	KM_NUM kM_U1;
	KM_NUM kM_U2;
#ifdef _DEBUG
	U32 ventTotalRunTime1; // 机组1，通风机累计运行时间
	U32 ventTotalRunTime2; // 机组2，通风机累计运行时间

	U32 exhaustTotalRunTime1; // 机组1，废排风机累计运行时间
	U32 exhaustTotalRunTime2; // 机组2，废排风机累计运行时间

	U32 condensorTotalRunTime11; // 机组1，冷凝风机累计运行时间
	U32 condensorTotalRunTime12; // 机组1，冷凝风机累计运行时间
	U32 condensorTotalRunTime21; // 机组2，冷凝风机累计运行时间
	U32 condensorTotalRunTime22; // 机组2，冷凝风机累计运行时间

	U32 compressorTotalRunTime11; // 机组1，压缩机1累计运行时间
	U32 compressorTotalRunTime12; // 机组1，压缩机2累计运行时间
	U32 compressorTotalRunTime21; // 机组2，压缩机1累计运行时间
	U32 compressorTotalRunTime22; // 机组2，压缩机2累计运行时间

	U32 heatTotalRunTime11; // 机组1，电加热器1累计运行时间
	U32 heatTotalRunTime12; // 机组1，电加热器2累计运行时间
	U32 heatTotalRunTime21; // 机组2，电加热器1累计运行时间
	U32 heatTotalRunTime22; // 机组2，电加热器2累计运行时间

	U32 RoomHeatTotalRunTime1; // 客室电加热1累计运行时间
	U32 RoomHeatTotalRunTime2; // 客室电加热2累计运行时间

	U32 hvac_energy_x10; // 空调能耗
#endif		         // _DEBUG

	U8 rtc[6]; // 8  B

	U32 sw_ver;

	U16 crc;
};
#pragma pack(pop)

extern AC_RUN_INFO g_runInfo;

extern AC_PARM g_parm;

#endif // !__AC_CONFIG_H__
