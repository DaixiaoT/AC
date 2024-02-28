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

#define g_cool_D1 (g_parm.Cool_acu_D1_parm)
#define g_cool_D2 (g_parm.Cool_acu_D2_parm)
#define g_cool_D3 (g_parm.Cool_acu_D3_parm)
#define g_cool_D4 (g_parm.Cool_acu_D4_parm)
#define g_cool_D5 (g_parm.Cool_acu_D5_parm)
#define g_heat_D1 (g_parm.Heat_acu_D1_parm)
#define g_heat_D2 (g_parm.Heat_acu_D2_parm)

#define gCompressorStartStopGap		((180) * 1000)		// 压缩机启动到停止的最短时间
#define gFanStartStopGap			((10) * 1000)		// 通风机启动到停止的最短时间
#define gFanStopStartGap			((10) * 1000)		// 通风机停止到启动的最短时间
#define gCondensorFanStopStartGap	((10) * 1000)		// 冷凝风机最小启停时间
#define gHeaterStopStartGap			((10)*1000)
#define gHeaterStopCondensorStartGap ((5)*1000)			// 电加热停止到冷凝风机开启
#define gCondensorFanStopGap		((5) * 1000)		// 冷凝风机到通风机停机的时间间隔
#define gCompressorCondensorStopGap ((5) * 1000)		// 压缩机停机到冷凝风机停机的时间间隔


#define AC_RUN_INFO_MARK 0x92fc6e52 // 增加了运行记录索引

#pragma pack(push, 1)
struct AC_PARM
{
#if 0
	U32 mark;
	U8	ModeSwitchDelay_s;
	U16 FreshAirDampAllOpenTime_s;
	U16 Compressor_min_running_time;
	U8 Supply_run_to_condenser_run_waiting_time;
	S32 Passenger_No_parm;
	S32 iTempSetpoint1_parm;
	U16 Cool_acu_D1_parm;
	U16 Cool_acu_D2_parm;
	U16 Cool_acu_D3_parm;
	U16 Cool_acu_D4_parm;
	U16 Cool_acu_D5_parm;


	S16 Heat_acu_D1_parm;
	S16 Heat_acu_D2_parm;
	S32 si_lowX_parm;
	S32 si_lowY_parm;
	S32 si_highX_parm;
	S32 si_highY_parm;
	U8 test;
	U16 crc;
#endif
	U32 mark;
	U8 CarNo;
	U16 FreshAirDampAllOpenTime_s;	          // 新风阀全开时间(0.1秒)
	U16 FDampOpenCloseTimeAW0_s;      // 新风阀 全开->1档时间(0.1秒)
	U16 FDampOpenCloseTimeAW2_s;      // 新风阀 全开->2档时间(0.1秒)
	U16 FDampOpenCloseTimeStroVent_s; // 新风阀 全开->3档时间(0.1秒)

	U8 CarLoad_AW2; // 0->1档 百分比

	U8 VentCondensorStartGap_s;        // 通风机启动->冷凝风机启动(秒)（13）
	U8 CondensorCompressorStartGap_s;  // 冷凝风机启动->压缩机启动(秒)（14）
	U8 CompressorCompressorStartGap_s; // 压缩机和压缩机启动时间间隔
	U8 VentHeatStartGap_s;	           // 通风机和电加热启动时间间隔
	U8 CompressorCondensorStopGap_s;   // 压缩机停机到冷凝风机停机的时间间隔
	U8 VentHeatStopGap_s;	           // 通风机和电加热停止时间间隔
	U8 EmPowerDelayTime_s;	           // 紧急逆变器延时启动时间
	U8 EmVentKeepTime_m;	           // 紧急通风状态保持时间，45分钟

	U16 CompressorStartStopGap_s; // 压缩机启动到停止的最短时间
	U16 CompressorStopStartGap_s; // 压缩机停止到启动的最短时间
	U16 CoolTargetTemp;	      // 制冷模式目标温度(度)（22）0.1度
	U16 gInitCoolHeatKeepTime_m;  // 预冷最大时间(分钟)（24）
	U16 CompressorStartLPGap_s;   // 压缩机启动到检测低压的最短时间
	S16 Tf_adjust;	      // 新风温度校准标定
	S16 Tr_adjust;	      // 回风温度校准标定
	U16 HP_adjust;	      // 高压压力传感器校准标定
	U16 LP_adjust;	      // 低压压力传感器校准标定

	U8 FDampForceOpen; // 新风阀和废排风阀强制打开

	U8 ModeSwitchDelay_s;
	U8 TouchScreenDelay_s;
	U8 SelftestModeOffsetTime_s;
	U8 SelftestModeRandomTime_s;
	U16 SelftestEndToCoolTime_s; // Starting time of Cooling mode after selftestin

	U16 AutoModeOneCompressorTempOffset_t;
	U16 AutoModeTwoCompressorTempOffset_t;
	U16 AutoModeOneHeatTempOffset_t;
	U16 AutoModeTwoHeatTempOffset_t;
	U16 res;

	U16 CoolHeatSwitchVentTime_s;

	U8 TouchScreenCommTimeout_s;
	U8 SensorErrorDelay_s;

	U8 ComressorContactorErrTrigger;
	U16 ComressorContactorErrDelay;

	U8 ComressorHPErrTrigger;
	U16 ComressorHpErrDelay;

	U8 ComressorLPErrTrigger;
	U16 ComressorLPErrDelay;

	U8 ComressorCurrentErrTrigger;
	U16 ComressorCurrentErrDelay;

	U8 HeaterContactorErrTrigger;
	U16 HeaterContactorErrDelay;

	U8 HeaterOverheatErrTrigger;
	U16 HeaterOverheatErrDelay;

	U8 SupplyFanContactorErrTrigger;
	U16 SupplyFanContactorErrDelay;

	U8 CondensorFanContactorErrTrigger;
	U16 CondensorFanContactorErrDelay;

	U16 Compressor_error_Waiting_time;
	U16 Heater_error_Waiting_time;
	U16 Condensor_error_Waiting_time;
	U16 Supply_error_Waiting_time;
	U16 Temperature_setpoint;
	U16 Overheat_ventilation_temperature;
	U16 Overheat_ventilation_continuously;
	U16 Overheat_ventilation_Restore_temperature;
	U16 Overheat_ventilation_Restore_temperature_continuously;
	U16 Overheat_off_temperature;
	U16 Overheat_off_continuously;
	U16 Overheat_off_Restore_temperature;
	U16 Overheat_off_Restore_temperature_continuously;
	U8 Supply_run_to_condenser_run_waiting_time;
	U8 Condenser_run_to_Compressor_run_waiting_time;
	U8 Compressor_run_to_Compressor_run_waiting_time;
	U16 Compressor_min_running_time;
	U16 Compressor_min_stopping_time;
	U8 Min_off_time_of_other_devices;
	U8 Supply_run_to_heaters_run_waiting_time;
	U16 Max_supply_air_temperature;
	U16 Max_supply_air_temperature_Restore;
	U16 Supply_air_fan_running_time_after_off;
	S32 Passenger_No_parm;
	S32 iTempSetpoint1_parm;
	S32 si_lowX_parm;
	S32 si_lowY_parm;
	S32 si_highX_parm;
	S32 si_highY_parm;

	U16 Cool_acu_D1_parm;
	U16 Cool_acu_D2_parm;
	U16 Cool_acu_D3_parm;
	U16 Cool_acu_D4_parm;
	U16 Cool_acu_D5_parm;


	S16 Heat_acu_D1_parm;
	S16 Heat_acu_D2_parm;

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

	U8 rtc[6]; // 8  B

	U32 sw_ver;

	U16 crc;
};
#pragma pack(pop)

extern AC_RUN_INFO g_runInfo;

extern AC_PARM g_parm;
#define gPi_iP_Verst_TimeLimit (500) // PI 计算中 aiZ_Wait_PI_MODE_FGR 倒计时不等于0时 iP_Verst参数
#define gPi_iP_Verst_TimeOut (300)	 // PI 计算中 aiZ_Wait_PI_MODE_FGR 倒计时等于0时 iP_Verst参数

#define g_MIN_KANAL_Parm (20)		  // T_MIN_KANAL_FGR 回差值
#define g_MAX_KANAL_Parm (20)		  // T_MAX_KANAL_FGR 回差值
#define g_Passenger_No (g_parm.Passenger_No_parm) // Passenger_No 乘客数量
#define g_TempSetpoint (g_parm.iTempSetpoint1_parm) // 常量
#define g_si_lowX (g_parm.si_lowX_parm)
#define g_si_lowY (g_parm.si_lowY_parm)
#define g_si_highX (g_parm.si_highX_parm)
#define g_si_highY (g_parm.si_highY_parm)














#endif // !__AC_CONFIG_H__
