#include "sys_api.h"
#include "485_module.h"
#include "debug_var.h"
#include "ac_ctrl.h"
#include "485_freq.h"
Freq freq[FREQ_NUM];




BOOL Freq::Timeout()
{
    return TimeGap(time_out_seconds) > 30000;
}

U8 Freq::WriteReadCmd(ReadCmd *data)
{
    // LOG_PRINT("Enter Freq::WriteReadCmd(),Add:%d\n", Add);
    data->MB_ADDR = Add;
    data->FUNC = 0x03;
    data->REG_ADD = EndianSwitch(0x0000);
    data->NUM = EndianSwitch(12);
    U16 crc;
    crc = crc16((U8 *)data, sizeof(ReadCmd) - 2);
    data->CRC16_H = HI(crc);
    data->CRC16_L = LO(crc);
    rx_tick = sys_time();
    com->Write((char *)data, sizeof(ReadCmd));
    return 1;
}

U8 Freq::WriteStateCmd(Freq_WriteWorkState *data)
{
    // LOG_PRINT("Enter Freq::WriteStateCmd()\n");
    data->StartCode = Add;
    data->Func = 0x06;
    data->REG_ADD = EndianSwitch(0x2000);
    U16 crc = 0;
    crc = crc16((U8 *)data, sizeof(Freq_WriteWorkState) - 2);
    data->Crc16_H = HI(crc);
    data->Crc16_L = LO(crc);
    memdump((U8 *)data, sizeof(Freq_WriteWorkState));
    com->Write((char *)data, sizeof(Freq_WriteWorkState));
    tx_tick = sys_time();
    return 1;
}

U8 Freq::WriteFrequencyCmd(Freq_WriteFrequency *data)
{
    // LOG_PRINT("Enter Freq::WriteFrequencyCmd()\n");
    data->StartCode = Add;
    data->Func = 0x06;
    data->REG_ADD = EndianSwitch(0x2001);
    U16 crc = 0;
    crc = crc16((U8 *)data, sizeof(Freq_WriteFrequency) - 2);
    data->Crc16_H = HI(crc);
    data->Crc16_L = LO(crc);
    com->Write((char *)data, sizeof(Freq_WriteFrequency));
    tx_tick = sys_time();
    return 1;
}

U8 Freq::ReadData(Freq_ReadData *data)
{
    // LOG_PRINT("Enter Freq::ReadData()\n");
    U16 crc;
    mem_set((U8 *)freq_uart_receive.data, 0, 256);
    freq_uart_receive.Num = 0;
    freq_uart_receive.Num = com->TryRead((char *)freq_uart_receive.data, 256);
    if (freq_uart_receive.Num <= 0)
    {
        return 1;
    }
    if (freq_uart_receive.data[0] != Add || freq_uart_receive.data[1] != 0x03)
    {
        return 2;
    }
    crc = crc16(freq_uart_receive.data, freq_uart_receive.Num - 2);
    if (HI(crc) == freq_uart_receive.data[freq_uart_receive.Num - 2] && LO(crc) == freq_uart_receive.data[freq_uart_receive.Num - 1])
    {
        mem_copy((U8 *)data, freq_uart_receive.data, sizeof(Freq_ReadData));
        time_out_seconds = sys_time();
        return 3;
    }
    return 4;
}


