#ifndef __EX_MODULE485_H__
#define __EX_MODULE485_H__
#include "ac_include.h"

#define VALVE_NUM (2)
#define FREQ_NUM (2)




typedef struct
{
	U8 MB_ADDR;	 // 从机地址码
	U8 FUNC;	 // 功能码
	U16 REG_ADD; // 寄存器起始地址
	U16 NUM;	 // 寄存器个数
	U8 CRC16_H;
	U8 CRC16_L;
} ReadCmd; // 读命令

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
	U8 StartCode;  // 从机地址码
	U8 Func;	   // 功能码
	U8 REG_ADD;	   // 寄存器起始地址
	U16 Superheat; // HI(过热度)-U1
	U8 crc16_H;
	U8 crc16_L;
} Valve_WriteSuperheat; // 写命令

typedef struct
{
	U8 StartCode;	  // 从机地址码
	U8 Func;		  // 功能码
	U8 NUM;			  // 数据所占字节数
	U16 MorkState;	  // Run/Stop 输入
	U16 ValveState;	  // 状态位
	U16 Res[7];		  // 预留
	U16 WarnState;	  // 报警状态
	U16 Superheat;	  // 当前过热度
	U16 Saturation;	  // 当前饱和温度
	U16 Pressure;	  // 当前压力
	U16 Temp;		  // 当前温度
	U16 Res1[1];	  // 预留
	U16 ValveOpening; // 膨胀阀开度
	U16 Crc16;
} Valve_ReadData; // 读数据

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
	U8 StartCode; // 从机地址码
	U8 Func;	  // 功能码
	U16 REG_ADD;  // 寄存器起始地址
	U16 Freq;	  // 输出频率
	U8 Crc16_H;
	U8 Crc16_L;
} Freq_WriteFrequency;

typedef struct
{
	U8 StartCode;  // 从机地址码
	U8 Func;	   // 功能码
	U16 REG_ADD;   // 寄存器起始地址
	U16 WorkState; // 工作状态
	U8 Crc16_H;
	U8 Crc16_L;
} Freq_WriteWorkState;

typedef struct
{
	U8 StartCode;	  // 从机地址码
	U8 Func;		  // 功能码
	U8 NUM;			  // 数据所占字节数
	U16 SetOutFreq;	  // 读设定输出频率
	U16 InputVoltage; // 输入电压
	U16 InputCurrent; // 输入电流
	U16 InputFreq;	  // 输入频率
	U16 Temp;		  // 散热器温度
	U16 OutFreq;	  // 输出频率
	U16 OutVoltage;	  // 输出电压
	U16 OutCurrent;	  // 输出电流
	U16 WorkState;	  // 工作状态
	U16 Res[2];		  // 预留
	U16 BusVoltage;	  // 母线电压
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
