#include "../ac_include.h"
#include "485_valve.h"
Valve valve[VALVE_NUM];

BOOL Valve::Timeout()
{
    return TimeGap(time_out_seconds) > 30000;
}

U8 Valve::WriteReadCmd(ReadCmd *data)
{
    // LOG_PRINT("Enter Valve::WriteReadCmd()\n");
    data->MB_ADDR = Add;
    data->FUNC = 0x03;
    data->REG_ADD = EndianSwitch(0x64);
    data->NUM = EndianSwitch(16);
    U16 crc;
    crc = crc16((U8 *)data, sizeof(ReadCmd) - 2);
    data->CRC16_H = HI(crc);
    data->CRC16_L = LO(crc);
    rx_tick = sys_time();
    com->Write((char *)data, sizeof(ReadCmd));
    return 1;
}

U8 Valve::WriteSuperheatCmd(Valve_WriteSuperheat *data)
{
    // LOG_PRINT("Enter Valve::WriteSuperheatCmd()\n");
    data->StartCode = Add;
    data->Func = 0x06;
    data->REG_ADD = EndianSwitch(0);
    U16 crc;
    crc = crc16((U8 *)data, sizeof(Valve_WriteSuperheat) - 2);
    data->crc16_H = HI(crc);
    data->crc16_L = LO(crc);
    rx_tick = sys_time();
    com->Write((char *)data, sizeof(Valve_WriteSuperheat));
    return 1;
}

U8 Valve::ReadData(Valve_ReadData *data)
{
    // LOG_PRINT("Enter Valve::ReadData()\n");
    U16 crc;
    mem_set((U8 *)valve_uart_receive.data, 0, 256);
    valve_uart_receive.Num = 0;
    valve_uart_receive.Num = com->TryRead((char *)valve_uart_receive.data, 256);
    if (valve_uart_receive.Num <= 0)
    {
        return 1;
    }
    if (valve_uart_receive.data[0] != Add || valve_uart_receive.data[1] != 0x03)
    {
        return 2;
    }
    crc = crc16(valve_uart_receive.data, valve_uart_receive.Num - 2);
    if (HI(crc) == valve_uart_receive.data[valve_uart_receive.Num - 2] && LO(crc) == valve_uart_receive.data[valve_uart_receive.Num - 1])
    {
        mem_copy((U8 *)data, valve_uart_receive.data, sizeof(Valve_ReadData));
        time_out_seconds = sys_time();
        return 3;
    }
    return 4;
}

U16 Valve::getSuperheat()
{
    return superheat;
}








