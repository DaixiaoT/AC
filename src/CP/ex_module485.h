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

// 缓存区结构体
typedef struct
{
	U16 Num;
	U8 data[256];
} SLINK_UART;

typedef struct 
{
	 CommObj *com;
	 U32 tx_tick;
	 U32 rx_tick;
}RS485_Module;

/*--------------------------------------------------Valve----------------------------------------------*/

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

class Valve
{

public:
	Valve()
	{
		tx_tick = 0;
		rx_tick = 0;
		Add = 1;
		com = &comm_uart4;
	}
	// void config(const char* name1,U8 add,RS485_Module*cfg){		
	// 	name = name1;
	// 	com=cfg->com;
	// 	Add=add;
	// 	tx_tick=cfg->tx_tick;
	// 	rx_tick=cfg->rx_tick;	
	// }
	U8 Add;
	const char* name;
	CommObj *com;
	U32 tx_tick;
	U32 rx_tick;
	BOOL Timeout();
	U8 WriteReadCmd(ReadCmd *data);	  // 发送读命令
	U8 WriteSuperheatCmd(Valve_WriteSuperheat *data); // 发送写过热度命令
	U8 ReadData(Valve_ReadData *data);		  // 读取膨胀阀数据
	
private:
	
	U32 time_out_seconds;
	SLINK_UART valve_uart_receive;
};

/*--------------------------------------------------Valve----------------------------------------------*/

/*--------------------------------------------------Freqency----------------------------------------------*/

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

/*--------------------------------------------------Freqency----------------------------------------------*/



extern Valve valve[VALVE_NUM];
extern Freq freq[FREQ_NUM];


void module_init();
void RS485_TASK();

static BOOL wait_en();

#endif //__EX_MODULE485_H__
