#ifndef __485_FREQ_H__
#define __485_FREQ_H__
#include "485_module.h"
#define FREQ_NUM (2)



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

class Freq
{
public:
	// BOOL Timeout();
	Freq()
	{
		tx_tick = 0;
		rx_tick = 0;
		Add = 1;
		time_out_seconds = 0;
		com = &comm_uart4;
	}
	// void config(const char* name1,U8 add,RS485_Module*cfg){		
	// 	name = name1;
	// 	com=cfg->com;
	// 	Add=add;
	// 	tx_tick=cfg->tx_tick;
	// 	rx_tick=cfg->rx_tick;	
	// 	}
	U8 Add;
	const char* name;
	CommObj *com;
	U32 tx_tick;
	U32 rx_tick;
	BOOL Timeout();
	U8 WriteReadCmd(ReadCmd *data);				   // 发送读命令
	U8 WriteStateCmd(Freq_WriteWorkState *data);	   // 发送写状态命令
	U8 WriteFrequencyCmd(Freq_WriteFrequency *data); // 发送写频率命令
	U8 ReadData(Freq_ReadData *data);				   // 读取变频器数据
	SLINK_UART freq_uart_receive;					   // 缓存区变量

private:
	
	U32 time_out_seconds;
};
extern Freq freq[FREQ_NUM];




#endif
