#ifndef __EX_MODULE485_H__
#define __EX_MODULE485_H__
#include "ac_include.h"

#define VALVE_NUM (2)
#define FREQ_NUM (2)




typedef struct
{
	U8 MB_ADDR;	 // �ӻ���ַ��
	U8 FUNC;	 // ������
	U16 REG_ADD; // �Ĵ�����ʼ��ַ
	U16 NUM;	 // �Ĵ�������
	U8 CRC16_H;
	U8 CRC16_L;
} ReadCmd; // ������

/*--------------------------------------------------Valve----------------------------------------------*/

class Valve
{

public:
	U32 time_out_seconds;
	BOOL Timeout();

private:
};

typedef struct
{
	U8 StartCode;  // �ӻ���ַ��
	U8 Func;	   // ������
	U8 REG_ADD;	   // �Ĵ�����ʼ��ַ
	U16 Superheat; // HI(���ȶ�)-U1
	U8 crc16_H;
	U8 crc16_L;
} Valve_WriteSuperheat; // д����

typedef struct
{
	U8 StartCode;	  // �ӻ���ַ��
	U8 Func;		  // ������
	U8 NUM;			  // ������ռ�ֽ���
	U16 MorkState;	  // Run/Stop ����
	U16 ValveState;	  // ״̬λ
	U16 Res[7];		  // Ԥ��
	U16 WarnState;	  // ����״̬
	U16 Superheat;	  // ��ǰ���ȶ�
	U16 Saturation;	  // ��ǰ�����¶�
	U16 Pressure;	  // ��ǰѹ��
	U16 Temp;		  // ��ǰ�¶�
	U16 Res1[1];	  // Ԥ��
	U16 ValveOpening; // ���ͷ�����
	U16 Crc16;
} Valve_ReadData; // ������

typedef struct
{
	ReadCmd read_cmd;
	Valve_WriteSuperheat valve_write_cmd;
	Valve_ReadData valve_read_data;
} Valve_Module;

/*--------------------------------------------------Valve----------------------------------------------*/

/*--------------------------------------------------Freqency----------------------------------------------*/

class Freq
{
};

typedef struct
{
	U8 StartCode; // �ӻ���ַ��
	U8 Func;	  // ������
	U16 REG_ADD;  // �Ĵ�����ʼ��ַ
	U16 Freq;	  // ���Ƶ��
	U8 Crc16_H;
	U8 Crc16_L;
} Freq_WriteFrequency;

typedef struct
{
	U8 StartCode;  // �ӻ���ַ��
	U8 Func;	   // ������
	U16 REG_ADD;   // �Ĵ�����ʼ��ַ
	U16 WorkState; // ����״̬
	U8 Crc16_H;
	U8 Crc16_L;
} Freq_WriteWorkState;

typedef struct
{
	U8 StartCode;	  // �ӻ���ַ��
	U8 Func;		  // ������
	U8 NUM;			  // ������ռ�ֽ���
	U16 SetOutFreq;	  // ���趨���Ƶ��
	U16 InputVoltage; // �����ѹ
	U16 InputCurrent; // �������
	U16 InputFreq;	  // ����Ƶ��
	U16 Temp;		  // ɢ�����¶�
	U16 OutFreq;	  // ���Ƶ��
	U16 OutVoltage;	  // �����ѹ
	U16 OutCurrent;	  // �������
	U16 WorkState;	  // ����״̬
	U16 Res[2];		  // Ԥ��
	U16 BusVoltage;	  // ĸ�ߵ�ѹ
	U16 Crc16;
} Freq_ReadData;

typedef struct
{

	ReadCmd readcmd_data;
	Freq_WriteFrequency WRFreq_data;
	Freq_WriteWorkState WRFreq_mode;
	Freq_ReadData Freqrx_data;
} Freq_Module;

/*--------------------------------------------------Freqency----------------------------------------------*/

extern Valve valve[VALVE_NUM];
extern Freq freq[FREQ_NUM];


void RS485_TASK();



#endif //__EX_MODULE485_H__
