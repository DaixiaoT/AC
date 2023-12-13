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


extern AC_PARM g_parm;

#endif // !__AC_CONFIG_H__
