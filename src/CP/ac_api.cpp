#include "ac_api.h"
#include "ac_ctrl.h"
#include "ac_ai.h"
#include "ex_module485.h"
#include "trdp_app.h"
#include "ptu/ptu_monitor.h"
#include "ac_config.h"
#include "ptu/ptu_error_run_log.h"
#include "../can_iap.h"

AC_PARM g_parm;

AC_ERR_Record g_err_record;
void ac_init()
{
	mi_assert(sizeof(AC_ERR_Record) == 256);
	mi_assert(sizeof(AC_RUN_Record) == 256);

	// 加载空调参数
	if (!LoadACParm(&g_parm))
	{
		// reset g_parm
		LOG_PRINT("bad ac parm. reset it.\r\n");

		ResetACParm(&g_parm);

		StoreACParm(&g_parm);
	}

	// load run info
	if (!LoadRunInfo(&g_runInfo))
	{
		LOG_PRINT("bad ac runInfo, reset it.\r\n");
		ResetRunInfo(&g_runInfo);
		StoreRunInfo(&g_runInfo);
		
	}

	g_car.Init();
}




void ac_control(U32 run_sec)
{
	// LOG_PRINT("system on.........\n");
	s_now_time = sys_time(); // 记录时间
	Up_my_sensor();
	RS485_TASK();
	ac_ModbusDataSwitch();
	ac_CarNumberUpdate();
	static U32 s_time = 0;
	if (TimeGap(s_time) < 500)
		return;
	if (g_car.AdjustRunTimeInfo()) {
		StoreRunInfo(&g_runInfo); // 机组设备运行时间存储
		LOG_AC("运行数据存入铁电\n");
	}
	s_time = sys_time();
	g_car.ProcessMode();
	//故障记录存储
	//运行记录存储
	//if (ac_get_record(&g_err_record))	  // 故障记录存储
	//    ac_err_log_record(&g_err_record); //!!!!
	save_run_log();						  // 运行记录存储
	ac_err_log_record(&g_err_record);
	// s_time=sys_time();
}
void ac_TRDP_to_Logic(U8 car)
{
	switch (car) {
	case 1:
		g_car.trdp.Set_Mode(TRDP_MODE(g_ccutohavcdata.CCU_HVAC_Car1Mode_U8));
		g_car.trdp.Set_Compressor_Enable(g_ccutohavcdata.CCU_HVAC_Car1Start_B1);
		g_car.trdp.Set_TempMode_Enable(g_ccutohavcdata.CCU_HVAC_Car1TempModeSet_B1);
		break;
	case 7:
		g_car.trdp.Set_Mode(TRDP_MODE(g_ccutohavcdata.CCU_HVAC_Car7Mode_U8));
		g_car.trdp.Set_Compressor_Enable(g_ccutohavcdata.CCU_HVAC_Car7Start_B1);
		g_car.trdp.Set_TempMode_Enable(g_ccutohavcdata.CCU_HVAC_Car7TempModeSet_B1);
		break;
	}
}
void ac_CarNumberUpdate()
{
	static U8 s_car_no = 0;
	// LOG_PRINT("Enter ac_CarNumberUpdate()\n");
	U8 car_number = 0;
	if (s_car_no == 0)
	{
		car_number = (DI_STAT(DI_CAR_ID1) << 2) | (DI_STAT(DI_CAR_ID2) << 1) | DI_STAT(DI_CAR_ID3);

		if (car_number == 4)
		{
			s_car_no = 1;
		}
		else if (car_number == 7)
		{
			s_car_no = 7;
		}
		else
		{
			s_car_no = 0;
		}
		if (s_car_no != 0)
		{
			//g_parm.test = 1;
			//LOG_AC("\ng_parm.test=%d\n", g_parm.test);
			trdp_app_init(s_car_no);
		}
		s_trdp_time = sys_time();
	}
	if ((s_car_no == 1) || (s_car_no == 7) && (TimeGap(s_trdp_time) >= 500) && (sys_time() > 30 * 1000))
	{
		ac_HVAC_to_TRDP(s_car_no);
		ac_TRDP_to_Logic(s_car_no);
		s_trdp_time = sys_time();
		//LOG_AC("\n车号：%d\n", s_car_no);
	}

}

void ac_HVAC_to_TRDP(U8 car)
{
	g_havctoccudata.HVACs_CNo_I_A_U8 = car;
	

}

void ac_ModbusDataSwitch()
{
	g_car.set1.freq1_485module.WRFreq_data.Freq = EndianSwitch(g_car.set1.Compressor_1.getFrequency());
	g_car.set1.freq1_485module.WRFreq_mode.WorkState = EndianSwitch(g_car.set1.Compressor_1.getFreqState());
	g_car.set1.valve1_485module.valve_write_cmd.Superheat = EndianSwitch(g_car.set1.valve[0].getSuperheat());

	g_car.set1.freq2_485module.WRFreq_data.Freq = EndianSwitch(g_car.set1.Compressor_2.getFrequency());
	g_car.set1.freq2_485module.WRFreq_mode.WorkState = EndianSwitch(g_car.set1.Compressor_2.getFreqState());
	g_car.set1.valve2_485module.valve_write_cmd.Superheat = EndianSwitch(g_car.set1.valve[1].getSuperheat());

	if (freq[0].Timeout() == FALSE)
	{
		g_car.set1.Compressor_1.freq_Out = EndianSwitch(g_car.set1.freq1_485module.Freqrx_data.OutFreq);
		g_car.set1.Compressor_1.current_out = EndianSwitch(g_car.set1.freq1_485module.Freqrx_data.OutCurrent);
		g_car.set1.Compressor_1.voltage_out = EndianSwitch(g_car.set1.freq1_485module.Freqrx_data.OutVoltage);
	}
	else
	{
		g_car.set1.Compressor_1.freq_Out = 0;
		g_car.set1.Compressor_1.current_out = 0;
		g_car.set1.Compressor_1.voltage_out = 0;
	}
	if (freq[1].Timeout() == FALSE)
	{
		g_car.set1.Compressor_2.freq_Out = EndianSwitch(g_car.set1.freq2_485module.Freqrx_data.OutFreq);
		g_car.set1.Compressor_2.current_out = EndianSwitch(g_car.set1.freq2_485module.Freqrx_data.OutCurrent);
		g_car.set1.Compressor_2.voltage_out = EndianSwitch(g_car.set1.freq2_485module.Freqrx_data.OutVoltage);
	}
	else
	{
		g_car.set1.Compressor_2.freq_Out = 0;
		g_car.set1.Compressor_2.current_out = 0;
		g_car.set1.Compressor_2.voltage_out = 0;
	}
}

//更新传感器数据
void Up_my_sensor()
{
	DA_Update(g_car.set1.Ventilator_1.getSpeed(), g_car.set1.Ventilator_2.getSpeed());
	g_car.set1.FreshAirDamp.resistor_feedback = U1_RD_F1();

	g_car.set1.Compressor_1.HighPressureValue = U1HP1();
	g_car.set1.Compressor_2.HighPressureValue = U1HP2();
	if (g_car.set1.valve[0].Timeout() != TRUE)
		g_car.set1.Compressor_1.LowPressureValue = EndianSwitch(g_car.set1.valve1_485module.valve_read_data.Pressure);
	else
		g_car.set1.Compressor_1.LowPressureValue = -100;
	if (g_car.set1.valve[1].Timeout() != TRUE)
		g_car.set1.Compressor_2.LowPressureValue = EndianSwitch(g_car.set1.valve2_485module.valve_read_data.Pressure);
	else
		g_car.set1.Compressor_2.LowPressureValue = -100;
}


void ResetACParm(AC_PARM* p) // 复位参数
{
	p->mark = AC_PARM_MARK;

	p->ModeSwitchDelay_s = 5;
	p->FreshAirDampAllOpenTime_s = 350;
	p->Passenger_No_parm = 0;
	p->iTempSetpoint1_parm = 220; // 常量
	p->si_lowX_parm = -200;
	p->si_lowY_parm = 320;
	p->si_highX_parm = 320;
	p->si_highY_parm = 70;

	p->Cool_acu_D1_parm = 120;
	p->Cool_acu_D2_parm = 90;
	p->Cool_acu_D3_parm = 60;
	p->Cool_acu_D4_parm = 30;
	p->Cool_acu_D5_parm = 0;



	p->Heat_acu_D1_parm = -20; // 0
	p->Heat_acu_D2_parm = -90; //-50

}

//static void ResetRunInfo(AC_RUN_INFO* p)
//{
//    mem_set((U8*)p, 0, sizeof(AC_RUN_INFO));
//
//    p->sw_ver = IAP_VERSION;
//    p->mark = AC_RUN_INFO_MARK;
//}

static BOOL LoadACParm(AC_PARM* p)
{
#ifdef _SPI_FRAM_
	SpiFramRead(AC_PARM_ADDR, (U8*)p, sizeof(AC_PARM));
#endif // _SPI_FRAM_
	U16 crc = CRC16((U8*)p, sizeof(AC_PARM) - 2);

	memdump((U8*)p, sizeof(AC_PARM));
	LOG_PRINT("crc:%x\r\n", crc);

	return (p->mark == AC_PARM_MARK && p->crc == crc);
}

static BOOL StoreACParm(AC_PARM* p)
{
	p->crc = CRC16((U8*)p, sizeof(AC_PARM) - 2);

#ifdef _SPI_FRAM_
	SpiFramWrite(AC_PARM_ADDR, (U8*)p, sizeof(AC_PARM));
#endif // _SPI_FRAM_
	return TRUE;
}



BOOL LoadRunInfo(AC_RUN_INFO* p)
{
#ifdef _SPI_FRAM_
	SpiFramRead(AC_RUN_INFO_ADDR, (U8*)p, sizeof(AC_RUN_INFO));
#endif // _SPI_FRAM_

	U16 crc = CRC16((U8*)p, sizeof(AC_RUN_INFO) - 2);
	return (p->mark == AC_RUN_INFO_MARK && p->crc == crc);
}

void ResetRunInfo(AC_RUN_INFO* p)
{
	mem_set((U8*)p, 0, sizeof(AC_RUN_INFO));

	p->sw_ver = IAP_VERSION;
	p->mark = AC_RUN_INFO_MARK;
}





