#include "ac_api.h"
#include "ac_ctrl.h"
#include "ai_update.h"
#include "ex_module485.h"

void ac_init()
{
    LOG_PRINT("Enter ac_init()\n");
}

void ac_control(U32 run_sec)
{
    LOG_PRINT("system on.........\n");
    static U32 s_now_time = 0;
    s_now_time = sys_time(); // ��¼ʱ��
    Up_my_sensor();
    RS485_TASK();
    

}


void Up_my_sensor()
{
    DA_Update(g_car.set1.Ventilator_1.getSpeed(), g_car.set1.Ventilator_2.getSpeed()); // ������תģ����
    g_car.set1.FreshAirDamp.resistor_feedback = ReadFreshAirDampFeedback();
    g_car.set1.Compressor_1.HighPressureValue = ReadHighPressureValue_1();
    g_car.set1.Compressor_2.HighPressureValue = ReadHighPressureValue_2();

    if (!valve[0].Timeout()) // ����ͱ���multiple definition
    {
        g_car.set1.Compressor_1.LowPressureValue = EndianSwitch(g_car.set1.valve1.valve_read_data.Pressure);
    }
    else
    {
        g_car.set1.Compressor_1.LowPressureValue = -100;
    }
    if (!valve[1].Timeout())
    {
        g_car.set1.Compressor_2.LowPressureValue = EndianSwitch(g_car.set1.valve2.valve_read_data.Pressure);
    }
    else
    {
        g_car.set1.Compressor_2.LowPressureValue = -100;
    }
}
