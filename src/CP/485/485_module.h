#ifndef __485_MODULE_H__
#define __485_MODULE_H__
#include "sys_api.h"
#include "comm_obj.h"
#include "LVF_TYPE.h"
void module_init();
void RS485_TASK();

static BOOL wait_en();

U16 crc16(U8 *pStart, U16 size);


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











#endif


