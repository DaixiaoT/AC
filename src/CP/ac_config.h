#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__
#include "ac_include.h"

//#define gRDampTimeOut (DampOpenAllTime + 100) // ((g_parm.RDampOpenTime_s+50)*100)  // �緧��ʱʱ��
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

#define gCompressorStartStopGap		((180) * 1000)		// ѹ����������ֹͣ�����ʱ��
#define gFanStartStopGap			((10) * 1000)		// ͨ���������ֹͣ�����ʱ��
#define gFanStopStartGap			((10) * 1000)		// ͨ���ֹͣ�����������ʱ��
#define gCondensorFanStopStartGap	((10) * 1000)		// ���������С��ͣʱ��
#define gHeaterStopStartGap			((10)*1000)
#define gHeaterStopCondensorStartGap ((5)*1000)			// �����ֹͣ�������������
#define gCondensorFanStopGap		((5) * 1000)		// ���������ͨ���ͣ����ʱ����
#define gCompressorCondensorStopGap ((5) * 1000)		// ѹ����ͣ�����������ͣ����ʱ����


#define AC_RUN_INFO_MARK 0x92fc6e52 // ���������м�¼����

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
	U16 FreshAirDampAllOpenTime_s;	          // �·緧ȫ��ʱ��(0.1��)
	U16 FDampOpenCloseTimeAW0_s;      // �·緧 ȫ��->1��ʱ��(0.1��)
	U16 FDampOpenCloseTimeAW2_s;      // �·緧 ȫ��->2��ʱ��(0.1��)
	U16 FDampOpenCloseTimeStroVent_s; // �·緧 ȫ��->3��ʱ��(0.1��)

	U8 CarLoad_AW2; // 0->1�� �ٷֱ�

	U8 VentCondensorStartGap_s;        // ͨ�������->�����������(��)��13��
	U8 CondensorCompressorStartGap_s;  // �����������->ѹ��������(��)��14��
	U8 CompressorCompressorStartGap_s; // ѹ������ѹ��������ʱ����
	U8 VentHeatStartGap_s;	           // ͨ����͵��������ʱ����
	U8 CompressorCondensorStopGap_s;   // ѹ����ͣ�����������ͣ����ʱ����
	U8 VentHeatStopGap_s;	           // ͨ����͵����ֹͣʱ����
	U8 EmPowerDelayTime_s;	           // �����������ʱ����ʱ��
	U8 EmVentKeepTime_m;	           // ����ͨ��״̬����ʱ�䣬45����

	U16 CompressorStartStopGap_s; // ѹ����������ֹͣ�����ʱ��
	U16 CompressorStopStartGap_s; // ѹ����ֹͣ�����������ʱ��
	U16 CoolTargetTemp;	      // ����ģʽĿ���¶�(��)��22��0.1��
	U16 gInitCoolHeatKeepTime_m;  // Ԥ�����ʱ��(����)��24��
	U16 CompressorStartLPGap_s;   // ѹ��������������ѹ�����ʱ��
	S16 Tf_adjust;	      // �·��¶�У׼�궨
	S16 Tr_adjust;	      // �ط��¶�У׼�궨
	U16 HP_adjust;	      // ��ѹѹ��������У׼�궨
	U16 LP_adjust;	      // ��ѹѹ��������У׼�궨

	U8 FDampForceOpen; // �·緧�ͷ��ŷ緧ǿ�ƴ�

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
	U8 cp1_contactor_err : 1;         // cp1�Ӵ�������
	U8 cp2_contactor_err : 1;         // cp2�Ӵ�������
	U8 cp3_contactor_err : 1;         // cp3�Ӵ�������
	U8 cp4_contactor_err : 1;         // cp4�Ӵ�������
	U8 heater1_contactor_err : 1;     // ����1�Ӵ�������
	U8 heater2_contactor_err : 1;     // ����2�Ӵ�������
	U8 condensor1_contactor_err : 1;  // �����Ӵ���1����
	U8 condensor1_Redundance_err : 1; // �����Ӵ�������1����

	U8 condensor2_contactor_err : 1;  // �����Ӵ���2����
	U8 condensor2_Redundance_err : 1; // �����Ӵ���2�������
	U8 supply1_contactor_err : 1;     // �ͷ�1�Ӵ�������
	U8 supply1_Redundance_err : 1;    // �ͷ�1�����ഥ������
	U8 supply2_contactor_err : 1;     // �ͷ�2�Ӵ�������
	U8 supply2_Redundance_err : 1;    // �ͷ�2�����ഥ������
	U8 supply1_state_err : 1;         // �ͷ�1�����Ϸ���
	U8 supply2_state_err : 1;         // �ͷ�2�����Ϸ���

	U8 ESF1_Contactor_err : 1; // ��ɳ����Ӵ������Ϸ���
	U8 EAF1_Contactor_err : 1; // ���ŷ���Ӵ������Ϸ���
	U8 FAF1_Contactor_err : 1; // ��������Ӵ������Ϸ���
	U8 FPPV_err : 1;	   // �·�PPV����
	U8 EPPV_err : 1;	   // ����PPV����
	U8 Power_3P_error : 1;	   // 3��PPV����
	U8 cp1_hp_err : 1;	   // cp1��ѹ����
	U8 cp2_hp_err : 1;	   // cp2��ѹ����

	U8 cp3_hp_err : 1;   // cp3��ѹ����
	U8 cp4_hp_err : 1;   // cp4��ѹ����
	U8 cp1_lp_err : 1;   // cp1��ѹ����
	U8 cp2_lp_err : 1;   // cp2��ѹ����
	U8 cp3_lp_err : 1;   // cp3��ѹ����
	U8 cp4_lp_err : 1;   // cp4��ѹ����
	U8 HVAC_hp1_err : 1; // cp1 ��������ѹ����
	U8 HVAC_hp2_err : 1; // cp2 ��������ѹ����

	U8 HVAC_hp3_err : 1;	    // cp3 ��������ѹ����
	U8 HVAC_hp4_err : 1;	    // cp4 ��������ѹ����
	U8 SA_temperature_err : 1;  // �ͷ� ��������ѹ����
	U8 FA_temperature_err : 1;  // �·� ��������ѹ����
	U8 RA1_temperature_err : 1; // ����1��������ѹ����
	U8 RA2_temperature_err : 1; // ����2 ��������ѹ����
	U8 RA3_temperature_err : 1; // ����3 ��������ѹ����
	U8 RA4_temperature_err : 1; // ����4 ��������ѹ����

	U8 RA5_temperature_err : 1; // ����5 ��������ѹ����
	U8 CO2_sensor_err : 1;	    // CO2 ��������ѹ����
	U8 HMI_timeout_err : 1;	    // ������ ����������
	U8 cp1_FI_timeout_err : 1;  // cp1��Ƶ������
	U8 cp2_FI_timeout_err : 1;  // cp2��Ƶ������
	U8 cp3_FI_timeout_err : 1;  // cp3��Ƶ������
	U8 cp4_FI_timeout_err : 1;  // cp4��Ƶ������
	U8 cp1_EEV_timeout_err : 1; // cp1���ͷ�����

	U8 cp2_EEV_timeout_err : 1; // cp2���ͷ�����
	U8 cp3_EEV_timeout_err : 1; // cp3���ͷ�����
	U8 cp4_EEV_timeout_err : 1; // cp4���ͷ�����
	U8 RA_damper_err : 1;	    // �ط緧����
	U8 res_err : 4;

}; // 7�ֽ�

// ���һ��д256�ֽ�
struct AC_ERR_Record
{	           // Ŀǰ����Ϊ��������
	U16 mark;  // 2 B
	U8 rtc[6]; // 8 B
	U8 res[2]; // Ԥ��
	ERR_Record_DATA u1_err;
	ERR_Record_DATA u2_err;

	U8 res_e[230]; // Ŀǰflash���һ��д256�ֽڣ���Ҫ�Ż�
	U16 crc;
};

#define RUN_DATA_LEN 245
struct AC_RUN_Record
{	           // ����Ϊ��������
	U16 mark;  // 2 B
	U8 rtc[6]; // 8 B

	U8 data[RUN_DATA_LEN]; // ���246�ֽ� �������ά������豸״̬���ݰ�
	U8 data_type;          // 0:���м�¼   ���㣺������¼
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
#define gPi_iP_Verst_TimeLimit (500) // PI ������ aiZ_Wait_PI_MODE_FGR ����ʱ������0ʱ iP_Verst����
#define gPi_iP_Verst_TimeOut (300)	 // PI ������ aiZ_Wait_PI_MODE_FGR ����ʱ����0ʱ iP_Verst����

#define g_MIN_KANAL_Parm (20)		  // T_MIN_KANAL_FGR �ز�ֵ
#define g_MAX_KANAL_Parm (20)		  // T_MAX_KANAL_FGR �ز�ֵ
#define g_Passenger_No (g_parm.Passenger_No_parm) // Passenger_No �˿�����
#define g_TempSetpoint (g_parm.iTempSetpoint1_parm) // ����
#define g_si_lowX (g_parm.si_lowX_parm)
#define g_si_lowY (g_parm.si_lowY_parm)
#define g_si_highX (g_parm.si_highX_parm)
#define g_si_highY (g_parm.si_highY_parm)














#endif // !__AC_CONFIG_H__
