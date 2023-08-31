#include "ac_api.h"
#include "ac_ctrl.h"
#include "ac_ai.h"
#include "ex_module485.h"
#include "trdp_app.h"
void ac_init()
{
    g_car.Init();
}

void ac_control(U32 run_sec)
{
    // LOG_PRINT("system on.........\n");
    s_now_time = sys_time(); // 记录时间
    // Up_my_sensor();
    RS485_TASK();
    ac_ModbusDataSwitch();
    ac_CarNumberUpdate();
    static U32 s_time = 0;
    if (TimeGap(s_time) < 500)
        return;
    s_time = sys_time();
    g_car.ProcessMode();

    // s_time=sys_time();
}
void ac_TRDP_to_Logic(U8 car)
{
}
void ac_CarNumberUpdate()
{
    // LOG_PRINT("Enter ac_CarNumberUpdate()\n");
    U8 car_number = 4;
    if (s_car_no == 0)
    {
        car_number = (DI_STAT(DI_CAR_ID1) << 2) | (DI_STAT(DI_CAR_ID2) << 1) | DI_STAT(DI_CAR_ID3);
    }
    if (car_number == 4)
    {
        s_car_no = 1;
    }
    else if (car_number == 7)
    {
        s_car_no = 4;
    }
    else
    {
        s_car_no = 0;
    }
    if (s_car_no != 0)
    {
        trdp_app_init(s_car_no);
    }
    s_trdp_time = sys_time();
    if ((s_car_no == 1) || (s_car_no == 4) && (TimeGap(s_trdp_time) >= 100) && (sys_time() > 30 * 1000))
    {
        ac_HVAC_to_TRDP(s_car_no);
        ac_TRDP_to_Logic(s_car_no);
        s_trdp_time = sys_time();
    }
}

void ac_HVAC_to_TRDP(U8 car)
{
}

void ac_ModbusDataSwitch()
{
    g_car.set1.freq1.WRFreq_data.Freq = EndianSwitch(g_car.set1.Compressor_1.getFrequency());
    g_car.set1.freq1.WRFreq_mode.WorkState = EndianSwitch(g_car.set1.Compressor_1.getFreqState());
    g_car.set1.valve1.valve_write_cmd.Superheat = EndianSwitch(g_car.set1.EEV1.getSuperheat());

    g_car.set1.freq2.WRFreq_data.Freq = EndianSwitch(g_car.set1.Compressor_2.getFrequency());
    g_car.set1.freq2.WRFreq_mode.WorkState = EndianSwitch(g_car.set1.Compressor_2.getFreqState());
    g_car.set1.valve2.valve_write_cmd.Superheat = EndianSwitch(g_car.set1.EEV2.getSuperheat());

    if (freq[0].Timeout() == FALSE)
    {
        g_car.set1.Compressor_1.freq_Out = EndianSwitch(g_car.set1.freq1.Freqrx_data.OutFreq);
        g_car.set1.Compressor_1.current_out = EndianSwitch(g_car.set1.freq1.Freqrx_data.OutCurrent);
        g_car.set1.Compressor_1.voltage_out = EndianSwitch(g_car.set1.freq1.Freqrx_data.OutVoltage);
    }
    else
    {
        g_car.set1.Compressor_1.freq_Out = 0;
        g_car.set1.Compressor_1.current_out = 0;
        g_car.set1.Compressor_1.voltage_out = 0;
    }
    if (freq[1].Timeout() == FALSE)
    {
        g_car.set1.Compressor_2.freq_Out = EndianSwitch(g_car.set1.freq2.Freqrx_data.OutFreq);
        g_car.set1.Compressor_2.current_out = EndianSwitch(g_car.set1.freq2.Freqrx_data.OutCurrent);
        g_car.set1.Compressor_2.voltage_out = EndianSwitch(g_car.set1.freq2.Freqrx_data.OutVoltage);
    }
    else
    {
        g_car.set1.Compressor_2.freq_Out = 0;
        g_car.set1.Compressor_2.current_out = 0;
        g_car.set1.Compressor_2.voltage_out = 0;
    }
}

void Up_my_sensor()
{
    //     DA_Update(g_car.set1.Ventilator_1.getSpeed(), g_car.set1.Ventilator_2.getSpeed()); // 数字量转模拟量
    //     g_car.set1.FreshAirDamp.resistor_feedback=ReadFreshAirDampFeedback();
    //     g_car.set1.Compressor_1.HighPressureValue = ReadHighPressureValue_1();
    //     g_car.set1.Compressor_2.HighPressureValue = ReadHighPressureValue_2();

    //     if (!valve[0].Timeout()) // 否则就报错multiple definition
    //     {
    //         g_car.set1.Compressor_1.LowPressureValue = EndianSwitch(g_car.set1.valve1.valve_read_data.Pressure);
    //     }
    //     else
    //     {
    //         g_car.set1.Compressor_1.LowPressureValue = -100;
    //     }
    //     if (!valve[1].Timeout())
    //     {
    //         g_car.set1.Compressor_2.LowPressureValue = EndianSwitch(g_car.set1.valve2.valve_read_data.Pressure);
    //     }
    //     else
    //     {
    //         g_car.set1.Compressor_2.LowPressureValue = -100;
    //     }
}
