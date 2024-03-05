#ifndef __485_VALVE_H__
#define __485_VALVE_H__
#include "485_module.h"
#define VALVE_NUM (2)

typedef struct
{
    U8 StartCode;  // 从机地址码
    U8 Func;       // 功能码
    U8 REG_ADD;    // 寄存器起始地址
    U16 Superheat; // HI(过热度)-U1
    U8 crc16_H;
    U8 crc16_L;
} Valve_WriteSuperheat; // 写命令

typedef struct
{
    U8 StartCode;     // 从机地址码
    U8 Func;          // 功能码
    U8 NUM;           // 数据所占字节数
    U16 MorkState;    // Run/Stop 输入
    U16 ValveState;   // 状态位
    U16 Res[7];       // 预留
    U16 WarnState;    // 报警状态
    U16 Superheat;    // 当前过热度
    U16 Saturation;   // 当前饱和温度
    U16 Pressure;     // 当前压力
    U16 Temp;         // 当前温度
    U16 Res1[1];      // 预留
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
    const char *name;
    CommObj *com;
    U32 tx_tick;
    U32 rx_tick;
    BOOL Timeout();
    U8 WriteReadCmd(ReadCmd *data);                   // 发送读命令
    U8 WriteSuperheatCmd(Valve_WriteSuperheat *data); // 发送写过热度命令
    U8 ReadData(Valve_ReadData *data);                // 读取膨胀阀数据
    U16 getSuperheat();

private:
    U16 superheat;
    U32 time_out_seconds;
    SLINK_UART valve_uart_receive;
};

extern Valve valve[VALVE_NUM];

#endif
