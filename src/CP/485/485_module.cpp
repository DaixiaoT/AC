#include "485_module.h"
#include "485_freq.h"
#include "485_valve.h"
#include "../ac_ctrl.h"
static const U16 crctable[256] = {
    (U16)0x0000,
    (U16)0xc1c0,
    (U16)0x81c1,
    (U16)0x4001,
    (U16)0x01c3,
    (U16)0xc003,
    (U16)0x8002,
    (U16)0x41c2,
    (U16)0x01c6,
    (U16)0xc006,
    (U16)0x8007,
    (U16)0x41c7,
    (U16)0x0005,
    (U16)0xc1c5,
    (U16)0x81c4,
    (U16)0x4004,
    (U16)0x01cc,
    (U16)0xc00c,
    (U16)0x800d,
    (U16)0x41cd,
    (U16)0x000f,
    (U16)0xc1cf,
    (U16)0x81ce,
    (U16)0x400e,
    (U16)0x000a,
    (U16)0xc1ca,
    (U16)0x81cb,
    (U16)0x400b,
    (U16)0x01c9,
    (U16)0xc009,
    (U16)0x8008,
    (U16)0x41c8,
    (U16)0x01d8,
    (U16)0xc018,
    (U16)0x8019,
    (U16)0x41d9,
    (U16)0x001b,
    (U16)0xc1db,
    (U16)0x81da,
    (U16)0x401a,
    (U16)0x001e,
    (U16)0xc1de,
    (U16)0x81df,
    (U16)0x401f,
    (U16)0x01dd,
    (U16)0xc01d,
    (U16)0x801c,
    (U16)0x41dc,
    (U16)0x0014,
    (U16)0xc1d4,
    (U16)0x81d5,
    (U16)0x4015,
    (U16)0x01d7,
    (U16)0xc017,
    (U16)0x8016,
    (U16)0x41d6,
    (U16)0x01d2,
    (U16)0xc012,
    (U16)0x8013,
    (U16)0x41d3,
    (U16)0x0011,
    (U16)0xc1d1,
    (U16)0x81d0,
    (U16)0x4010,
    (U16)0x01f0,
    (U16)0xc030,
    (U16)0x8031,
    (U16)0x41f1,
    (U16)0x0033,
    (U16)0xc1f3,
    (U16)0x81f2,
    (U16)0x4032,
    (U16)0x0036,
    (U16)0xc1f6,
    (U16)0x81f7,
    (U16)0x4037,
    (U16)0x01f5,
    (U16)0xc035,
    (U16)0x8034,
    (U16)0x41f4,
    (U16)0x003c,
    (U16)0xc1fc,
    (U16)0x81fd,
    (U16)0x403d,
    (U16)0x01ff,
    (U16)0xc03f,
    (U16)0x803e,
    (U16)0x41fe,
    (U16)0x01fa,
    (U16)0xc03a,
    (U16)0x803b,
    (U16)0x41fb,
    (U16)0x0039,
    (U16)0xc1f9,
    (U16)0x81f8,
    (U16)0x4038,
    (U16)0x0028,
    (U16)0xc1e8,
    (U16)0x81e9,
    (U16)0x4029,
    (U16)0x01eb,
    (U16)0xc02b,
    (U16)0x802a,
    (U16)0x41ea,
    (U16)0x01ee,
    (U16)0xc02e,
    (U16)0x802f,
    (U16)0x41ef,
    (U16)0x002d,
    (U16)0xc1ed,
    (U16)0x81ec,
    (U16)0x402c,
    (U16)0x01e4,
    (U16)0xc024,
    (U16)0x8025,
    (U16)0x41e5,
    (U16)0x0027,
    (U16)0xc1e7,
    (U16)0x81e6,
    (U16)0x4026,
    (U16)0x0022,
    (U16)0xc1e2,
    (U16)0x81e3,
    (U16)0x4023,
    (U16)0x01e1,
    (U16)0xc021,
    (U16)0x8020,
    (U16)0x41e0,
    (U16)0x01a0,
    (U16)0xc060,
    (U16)0x8061,
    (U16)0x41a1,
    (U16)0x0063,
    (U16)0xc1a3,
    (U16)0x81a2,
    (U16)0x4062,
    (U16)0x0066,
    (U16)0xc1a6,
    (U16)0x81a7,
    (U16)0x4067,
    (U16)0x01a5,
    (U16)0xc065,
    (U16)0x8064,
    (U16)0x41a4,
    (U16)0x006c,
    (U16)0xc1ac,
    (U16)0x81ad,
    (U16)0x406d,
    (U16)0x01af,
    (U16)0xc06f,
    (U16)0x806e,
    (U16)0x41ae,
    (U16)0x01aa,
    (U16)0xc06a,
    (U16)0x806b,
    (U16)0x41ab,
    (U16)0x0069,
    (U16)0xc1a9,
    (U16)0x81a8,
    (U16)0x4068,
    (U16)0x0078,
    (U16)0xc1b8,
    (U16)0x81b9,
    (U16)0x4079,
    (U16)0x01bb,
    (U16)0xc07b,
    (U16)0x807a,
    (U16)0x41ba,
    (U16)0x01be,
    (U16)0xc07e,
    (U16)0x807f,
    (U16)0x41bf,
    (U16)0x007d,
    (U16)0xc1bd,
    (U16)0x81bc,
    (U16)0x407c,
    (U16)0x01b4,
    (U16)0xc074,
    (U16)0x8075,
    (U16)0x41b5,
    (U16)0x0077,
    (U16)0xc1b7,
    (U16)0x81b6,
    (U16)0x4076,
    (U16)0x0072,
    (U16)0xc1b2,
    (U16)0x81b3,
    (U16)0x4073,
    (U16)0x01b1,
    (U16)0xc071,
    (U16)0x8070,
    (U16)0x41b0,
    (U16)0x0050,
    (U16)0xc190,
    (U16)0x8191,
    (U16)0x4051,
    (U16)0x0193,
    (U16)0xc053,
    (U16)0x8052,
    (U16)0x4192,
    (U16)0x0196,
    (U16)0xc056,
    (U16)0x8057,
    (U16)0x4197,
    (U16)0x0055,
    (U16)0xc195,
    (U16)0x8194,
    (U16)0x4054,
    (U16)0x019c,
    (U16)0xc05c,
    (U16)0x805d,
    (U16)0x419d,
    (U16)0x005f,
    (U16)0xc19f,
    (U16)0x819e,
    (U16)0x405e,
    (U16)0x005a,
    (U16)0xc19a,
    (U16)0x819b,
    (U16)0x405b,
    (U16)0x0199,
    (U16)0xc059,
    (U16)0x8058,
    (U16)0x4198,
    (U16)0x0188,
    (U16)0xc048,
    (U16)0x8049,
    (U16)0x4189,
    (U16)0x004b,
    (U16)0xc18b,
    (U16)0x818a,
    (U16)0x404a,
    (U16)0x004e,
    (U16)0xc18e,
    (U16)0x818f,
    (U16)0x404f,
    (U16)0x018d,
    (U16)0xc04d,
    (U16)0x804c,
    (U16)0x418c,
    (U16)0x0044,
    (U16)0xc184,
    (U16)0x8185,
    (U16)0x4045,
    (U16)0x0187,
    (U16)0xc047,
    (U16)0x8046,
    (U16)0x4186,
    (U16)0x0182,
    (U16)0xc042,
    (U16)0x8043,
    (U16)0x4183,
    (U16)0x0041,
    (U16)0xc181,
    (U16)0x8180,
    (U16)0x4040,
};
U16 crc16(U8 *pStart, U16 size)
{
    U16 crc = 0xFFFF;
    U16 dat;
    while (size--)
    {
        dat = (U16)*pStart++;
        crc = (U16)((crc << 8) ^ crctable[((crc >> 8) ^ dat) & (U16)0xFF]);
    }
    return crc;
}

static Freq_Module *s_freq[FREQ_NUM] = {
    &g_car.set1.freq1_485module,
    &g_car.set1.freq2_485module,
};
static Valve_Module *s_valve[VALVE_NUM] = {
    &g_car.set1.valve1_485module,
    &g_car.set1.valve2_485module };


#define WAIT_TIME 200
static BOOL read_wait()
{
    for (int i = 0; i < FREQ_NUM; i++)
    {
        if (TimeGap(freq[i].rx_tick) < WAIT_TIME)
            return FALSE;
    }
    for (int i = 0; i < VALVE_NUM; i++)
    {
        if (TimeGap(valve[i].rx_tick) < WAIT_TIME)
            return FALSE;
    }
    return TRUE;
}

static BOOL write_wait()
{
    for (int i = 0; i < FREQ_NUM; i++)
    {
        if (TimeGap(freq[i].tx_tick) < WAIT_TIME)
        {
            return FALSE;
        }
    }
    for (int i = 0; i < VALVE_NUM; i++)
    {
        if (TimeGap(valve[i].tx_tick) < WAIT_TIME)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// 满足最低发送或接收延时
static BOOL wait_en()
{
    return (read_wait() && write_wait());
}

void module_init()
{
    // LOG_PRINT("Enter module_init()\n");
    for (int i = 0; i < FREQ_NUM; i++)
    {
        freq[i].Add = 51 + i;
        // freq[i].config(m_info[i].name, m_info[i].Add, m_info[i].message); // 变频器
    }
    for (int k = 0; k < VALVE_NUM; k++)
    {
        valve[k].Add = 53 + k;
        // valve[k].config(m_info[k + 2].name, m_info[k + 2].Add, m_info[k + 2].message); // 阀门
    }
}
enum State
{
    enum_Freq_WriteReadCmd,         //变频器发送读取命令
    enum_Freq_ReadData,             //变频器读取数据
    enum_Freq_WriteFrequency,       //变频器写频率
    enum_Freq_WriteWorkState,       //变频器写工作状态
    enum_Valve_WriteReadCmd,        //膨胀阀发送读取命令
    enum_Valve_ReadData,            //膨胀阀读取数据
    enum_Valve_WriteSuperheat       //膨胀阀写过热度
};

void RS485_TASK()
{
    static int s_step = enum_Freq_WriteReadCmd;//设定初始状态为——发送变频器读取命令
    static int s_device_num = 0;               //设定初始设备号为0
    U8 temp = 0;
    // LOG_PRINT("Enter RS485_TASK() s_step:%d\n", s_step);
    switch (s_step)
    {
    case enum_Freq_WriteReadCmd:
        if (wait_en() == FALSE)//等待发送读写命令延时各200ms
            return;
        if (freq[s_device_num].WriteReadCmd(&s_freq[s_device_num]->readcmd_data) == 1)
        {
            s_step = enum_Freq_ReadData;//发送完变频器读取命令后，接收变频器数据，状态更新为变频器读取数据
        }
        break;
    case enum_Freq_ReadData:
        if (wait_en() == FALSE)
            return;
        // LOG_PRINT("Freq_ReadData,temp=%d\n", temp);
        temp = freq[s_device_num].ReadData(&s_freq[s_device_num]->Freqrx_data);

        if (temp == 3)//读取到的变频器数据正常
        {
            s_step = enum_Freq_WriteFrequency;//切换到变频器写频率状态
        }else if (temp == 1)//读取到的变频器数据字节数小于等于0
        {
            s_step = enum_Freq_WriteReadCmd;//继续发送变频器读取数据命令
        }   
        //temp == 2     收到数据的地址/功能码错误
        //temp == 4     其他错误

        break;
    case enum_Freq_WriteFrequency:
        if (wait_en() == FALSE)
            return;
        if (freq[s_device_num].WriteFrequencyCmd(&s_freq[s_device_num]->WRFreq_data) == 1)
        {
            s_step = enum_Freq_WriteWorkState;//切换状态为变频器写工作状态
        }
        break;
    case enum_Freq_WriteWorkState:
        if (wait_en() == FALSE)
            return;
        if (freq[s_device_num].WriteStateCmd(&s_freq[s_device_num]->WRFreq_mode) == 1)
        {
            s_step = enum_Valve_WriteReadCmd;//切换状态为膨胀阀写读取命令
        }
        break;
    case enum_Valve_WriteReadCmd:
        if (wait_en() == FALSE)
            return;
        if (valve[s_device_num].WriteReadCmd(&s_valve[s_device_num]->read_cmd) == 1)
        {
            s_step = enum_Valve_ReadData;
        }
        break;
    case enum_Valve_ReadData:
        if (wait_en() == FALSE)
            return;
        temp = valve[s_device_num].ReadData(&s_valve[s_device_num]->valve_read_data);
        // LOG_PRINT("Valve_ReadData,temp=%d\n", temp);

        if (temp == 3)
        {
            s_step = enum_Valve_WriteSuperheat;//切换状态为膨胀阀写过热度
        }
        else if (temp == 1)
        {
            s_step = enum_Valve_WriteReadCmd;
        }
        break;
    case enum_Valve_WriteSuperheat:
        if (wait_en() == FALSE)
            return;
        if (valve[s_device_num].WriteSuperheatCmd(&s_valve[s_device_num]->valve_write_cmd))
        {
            s_step = enum_Freq_WriteReadCmd;
            s_device_num = (s_device_num + 1) % 2;
        }
        break;
    }
}




