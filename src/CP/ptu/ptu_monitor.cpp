#include "ptu_monitor.h"

#include "can_iap.h"
#include "ac_ctrl.h"
#include "../trdp/trdp_app.h"
#include "debug_var.h"
DEBUG_VAR g_debug_var;
#define TEST 1
unsigned char s_DeviceStatusAutoRefresh = 0; // 为1时，控制器->PTU送数据

#if TEST
int GetDeviceIOInfoPacket(U8 *buf, int len)
{
	int i;
	const int Tlen = 900;
	mem_set(buf, 0, Tlen);
	buf[0] = 0xf5;
	Set16(buf + 1, Tlen);
	buf[3] = 0x31;

	// 控制器主机DI
	for (i = 0; i < DI_NUM; i++)
		SetBit(buf, 5 + i / 8, i % 8, DI_STAT(i));
	// 控制器主机DO
	for (i = 0; i < DO_NUM; i++)
		SetBit(buf, 21 + i / 8, i % 8, DO_STAT(i));

	for (int i = 0; i < AI_NUM; i++)
	{
		Set16(buf + 62 + (i * 2), (S16)(ctrl_AI[i].getValue()));
		// SetBit(buf, 54 + (i / 8), i & 7, (S16)ctrl_ai[i].value() != (S16)ctrl_ai[i].bad_value());
		SetBit(buf, 54 + (i / 8), i & 7, 1);
	}
	for (int i = 0; i < DA_NUM; i++)
	{
		Set16(buf + 122 + (i * 2), (ctrl_DA[i].get_value() / 100));
		SetBit(buf, 58 + (i / 8), i & 7, ctrl_DA[i].get_value() != 0);
	}
	buf[129] = g_car.trdpMode;
	buf[133] = g_car.ctrlMode;
	buf[242] = hton32(g_ccutohavcdata.CCU_Lifesign_U32) / 1000;

	i = 300;
	// 机组1
	Set32(buf + i, (g_car.set1.Compressor_1.timer.getThisRunTime()));
	i += 4;

	Set32(buf + i, g_car.set1.Compressor_1.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Compressor_2.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Compressor_2.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Condenser1.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Condenser1.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Condenser2.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Condenser2.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Ventilator_1.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Ventilator_1.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Ventilator_2.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.Ventilator_2.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.heater1.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.heater1.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.heater2.timer.getThisRunTime());
	i += 4;

	Set32(buf + i, g_car.set1.heater2.timer.getTotalRunTime());
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.Compressor_1.timer.getOffTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.Compressor_2.timer.getOffTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.heater1.timer.getOffTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.heater2.timer.getOffTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.Compressor_1.timer.getOnTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.Compressor_2.timer.getOnTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.heater1.timer.getOnTime()) / 1000);
	i += 4;

	Set32(buf + i, TimeGap(g_car.set1.heater2.timer.getOnTime()) / 1000);
	i += 4;





	
	// Set16(buf + 158, g_car.set1.m_EEV1.EEV_value());
	// Set16(buf + 160, g_car.m_set1.m_EEV2.EEV_value());
	Set16(buf + 138, g_car.set1.Compressor_1.getFrequency());
	Set16(buf + 154, g_car.set1.Compressor_1.getFreqState());
	Set16(buf + 170, (g_car.set1.freq1_485module.Freqrx_data.SetOutFreq));
	Set16(buf + 172, (g_car.set1.freq1_485module.Freqrx_data.InputVoltage));
	// Set16(buf + 174, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.input_current));
	// Set16(buf + 176, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.input_freq));
	// Set16(buf + 178, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.Temp));
	// Set16(buf + 180, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.out_freq));
	// Set16(buf + 182, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.out_voltage));
	// Set16(buf + 184, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.out_current));
	// Set16(buf + 186, g_car.set1.m_freq1.Freqrx_data.work_state);
	// Set16(buf + 188, EndianSwitch(g_car.set1.m_freq1.Freqrx_data.bus_voltage));

	// Set16(buf + 190, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.set_outfreq));
	// Set16(buf + 192, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.input_voltag));
	// Set16(buf + 194, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.input_current));
	// Set16(buf + 196, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.input_freq));
	// Set16(buf + 198, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.Temp));
	// Set16(buf + 200, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.out_freq));
	// Set16(buf + 202, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.out_voltage));
	// Set16(buf + 204, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.out_current));
	// Set16(buf + 206, g_car.set1.m_freq2.Freqrx_data.work_state);
	// Set16(buf + 208, EndianSwitch(g_car.set1.m_freq2.Freqrx_data.bus_voltage));

	// Set16(buf + 210, EndianSwitch(g_car.set1.m_valve1.valverx_data.MorkState));
	// Set16(buf + 212, g_car.set1.m_valve1.valverx_data.ValveState);
	// Set16(buf + 214, g_car.set1.m_valve1.valverx_data.valve_warn);
	// Set16(buf + 216, EndianSwitch(g_car.set1.m_valve1.valverx_data.superheat));
	// Set16(buf + 218, EndianSwitch(g_car.set1.m_valve1.valverx_data.saturation));
	// Set16(buf + 220, EndianSwitch(g_car.set1.m_valve1.valverx_data.pressure));
	// Set16(buf + 222, EndianSwitch(g_car.set1.m_valve1.valverx_data.Temp));
	// Set16(buf + 224, EndianSwitch(g_car.set1.m_valve1.valverx_data.TValve_apetrture));

	// Set16(buf + 226, EndianSwitch(g_car.set1.m_valve2.valverx_data.MorkState));
	// Set16(buf + 228, g_car.set1.m_valve2.valverx_data.ValveState);
	// Set16(buf + 230, g_car.set1.m_valve2.valverx_data.valve_warn);
	// Set16(buf + 232, EndianSwitch(g_car.set1.m_valve2.valverx_data.superheat));
	// Set16(buf + 234, EndianSwitch(g_car.set1.m_valve2.valverx_data.saturation));
	// Set16(buf + 236, EndianSwitch(g_car.set1.m_valve2.valverx_data.pressure));
	// Set16(buf + 238, EndianSwitch(g_car.set1.m_valve2.valverx_data.Temp));
	// Set16(buf + 240, EndianSwitch(g_car.set1.m_valve2.valverx_data.TValve_apetrture));




	buf[Tlen - 1] = GetParity(buf, Tlen - 1);
	return Tlen;
}

void StoreDeviceIOInfo(char *p, int len)
{
	U8 *buf = (U8 *)p;
	for (int i = 0; i < DI_NUM; i++)
		g_DI.SetForce(i, GetBit8(buf[21 + i / 8], i % 8), GetBit8(buf[5 + i / 8], i % 8));

	for (int i = 0; i < AI_NUM; i++)
	{
		ctrl_AI[i].force(GetBit8(buf[130 + i / 8], i % 8), ((AI_TYPE)(Get16(buf + 62 + (i * 2)))));
	}
	g_car.set1.Compressor_1.forceFrequency(GetBit8(buf[146], 0), Get16(buf + 164));
	g_car.set1.Compressor_2.forceFrequency(GetBit8(buf[146], 1), Get16(buf + 166));
	g_car.set1.Compressor_1.forceState(GetBit8(buf[147], 0), Get16(buf + 180));
	g_car.set1.Compressor_2.forceState(GetBit8(buf[147], 1), Get16(buf + 182));

	g_debug_var.var = Get32_LE(buf + 137);
}
#else

// 未修改

int GetDeviceIOInfoPacket(U8 *buf, int len) // 取控制器数据
{
	int i;
	const int Tlen = 900; // 字节
	if (len < Tlen)
	{
		return 0;
	}
	mem_set(buf, 0, Tlen); // 清零
	buf[0] = 0xf5;		   // 帧头
	Set16(buf + 1, Tlen);
	buf[3] = 0x31; // 识别码
	// 控制器主机DI
	for (i = 0; i < DI_NUM; i++)
		SetBit(buf, 5 + i / 8, i % 8, DI_STAT(i));
	// 控制器主机DO
	for (i = 0; i < DO_NUM; i++)
		SetBit(buf, 21 + i / 8, i % 8, DO_STAT(i));

	for (int i = 0; i < AI_NUM; i++)
	{
		Set16(buf + 62 + (i * 2), (S16)(ctrl_ai[i].value()));
		// SetBit(buf, 54 + (i / 8), i & 7, (S16)ctrl_ai[i].value() != (S16)ctrl_ai[i].bad_value());
		SetBit(buf, 54 + (i / 8), i & 7, 1);
	}

	for (int i = 0; i < DA_NUM; i++)
	{
		Set16(buf + 122 + (i * 2), (ctrl_DA[i].value() / 100));
		SetBit(buf, 58 + (i / 8), i & 7, ctrl_DA[i].value() != 0);
	}

#ifdef _PCA9629_
	Set16(buf + 126, (ctrl_valve[0].value()));
	SetBit(buf, 59, 0, ctrl_valve[0].value() != 0);

	Set16(buf + 128, (ctrl_valve[1].value()));
	SetBit(buf, 59, 1, ctrl_valve[1].value() != 0);
#endif // _PCA9629_
	buf[127] = App_HVACs_SWVH;
	// buf[128] =  App_HVACs_SWVM;
	buf[128] = App_HVACs_SWVL;
	if ((g_car.m_tRDP.get_EM_COOL() != FALSE) && (g_car.m_tRDP.OK() != FALSE))
	{
		buf[129] = 11;
	}
	else
	{
		buf[129] = TRDP_MODE();
	}

	SetBit8(buf + 130, 0, 1); // PI计算值在PTU上显示
	SetBit8(buf + 130, 1, g_car.m_tRDP.OK() == FALSE);
	Set16(buf + 131, (U16)g_car.m_sw_temp);

	if (g_car.m_init_heat_flag && IS_SET_INIT_HEAT_MODE(g_car.m_set1.m_work_mode))
		buf[133] = SET_INIT_HEAT;
	else
		buf[133] = (g_car.m_init_cool_flag && IS_SET_INIT_COOL_MODE(g_car.m_set1.m_work_mode)) ? SET_INIT_COOL : HVAC_mode(g_car.m_set1.m_work_mode);

	if ((g_car.m_tRDP.get_EM_COOL() != FALSE) && (g_car.m_tRDP.OK() != FALSE))
	{
		buf[133] = 22;
	}

	Set16(buf + 134, g_car.m_acu);

	Set16(buf + 138, g_car.m_set1.m_compressor1.freq_value());
	Set16(buf + 140, g_car.m_set1.m_compressor2.freq_value());
	Set16(buf + 154, g_car.m_set1.m_compressor1.freq_state());
	Set16(buf + 156, g_car.m_set1.m_compressor2.freq_state());

	Set16(buf + 158, g_car.m_set1.m_EEV1.EEV_value());
	Set16(buf + 160, g_car.m_set1.m_EEV2.EEV_value());

	Set16(buf + 170, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.set_outfreq));
	Set16(buf + 172, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.input_voltag));
	Set16(buf + 174, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.input_current));
	Set16(buf + 176, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.input_freq));
	Set16(buf + 178, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.Temp));
	Set16(buf + 180, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.out_freq));
	Set16(buf + 182, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.out_voltage));
	Set16(buf + 184, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.out_current));
	Set16(buf + 186, g_car.m_set1.m_freq1.Freqrx_data.work_state);
	Set16(buf + 188, EndianSwitch(g_car.m_set1.m_freq1.Freqrx_data.bus_voltage));

	Set16(buf + 190, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.set_outfreq));
	Set16(buf + 192, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.input_voltag));
	Set16(buf + 194, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.input_current));
	Set16(buf + 196, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.input_freq));
	Set16(buf + 198, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.Temp));
	Set16(buf + 200, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.out_freq));
	Set16(buf + 202, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.out_voltage));
	Set16(buf + 204, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.out_current));
	Set16(buf + 206, g_car.m_set1.m_freq2.Freqrx_data.work_state);
	Set16(buf + 208, EndianSwitch(g_car.m_set1.m_freq2.Freqrx_data.bus_voltage));

	Set16(buf + 210, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.MorkState));
	Set16(buf + 212, g_car.m_set1.m_valve1.valverx_data.ValveState);
	Set16(buf + 214, g_car.m_set1.m_valve1.valverx_data.valve_warn);
	Set16(buf + 216, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.superheat));
	Set16(buf + 218, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.saturation));
	Set16(buf + 220, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.pressure));
	Set16(buf + 222, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.Temp));
	Set16(buf + 224, EndianSwitch(g_car.m_set1.m_valve1.valverx_data.TValve_apetrture));

	Set16(buf + 226, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.MorkState));
	Set16(buf + 228, g_car.m_set1.m_valve2.valverx_data.ValveState);
	Set16(buf + 230, g_car.m_set1.m_valve2.valverx_data.valve_warn);
	Set16(buf + 232, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.superheat));
	Set16(buf + 234, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.saturation));
	Set16(buf + 236, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.pressure));
	Set16(buf + 238, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.Temp));
	Set16(buf + 240, EndianSwitch(g_car.m_set1.m_valve2.valverx_data.TValve_apetrture));
	buf[246] = g_car.m_set1.m_compressor1.m_LpTimeRecord.m_num;
	buf[247] = g_car.m_set1.m_compressor2.m_LpTimeRecord.m_num;
	buf[250] = g_car.Diag_err[0];
	buf[251] = g_car.Diag_err[1];

	SetBit(buf, 280, 0, (g_car.to_cool_err.thisRunTime() >= COOLHEAT_TIME));
	SetBit(buf, 280, 1, (g_car.to_heat_err.thisRunTime() >= COOLHEAT_TIME));
	SetBit(buf, 280, 2, (g_car.car_Htemp_err.thisRunTime() >= COOLHEAT_TIME));
	SetBit(buf, 280, 3, (g_car.car_Ltemp_err.thisRunTime() >= COOLHEAT_TIME));
	SetBit(buf, 280, 4, g_car.m_set1.m_compressor1.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 280, 5, g_car.m_set1.m_compressor2.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 280, 6, g_car.m_set1.m_condenser1.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 280, 7, g_car.m_set1.m_condenser2.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 281, 0, g_car.m_set1.m_vent1.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 281, 1, g_car.m_set1.m_vent2.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 281, 2, g_car.m_set1.m_heat1.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 281, 3, g_car.m_set1.m_heat2.timer_Run.totalRunTime() >= KM_ERRTIME);
	SetBit(buf, 281, 4, g_car.m_set1.m_compressor1.m_km_num >= KM_ERRNUM);
	SetBit(buf, 281, 5, g_car.m_set1.m_compressor2.m_km_num >= KM_ERRNUM);
	SetBit(buf, 281, 6, g_car.m_set1.m_condenser1.m_Fanctrl.m_km_num >= KM_ERRNUM);
	SetBit(buf, 281, 7, g_car.m_set1.m_condenser1.m_FanBkctrl.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 0, g_car.m_set1.m_condenser2.m_Fanctrl.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 1, g_car.m_set1.m_condenser2.m_FanBkctrl.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 2, g_car.m_set1.m_vent1.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 3, g_car.m_set1.m_vent2.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 4, g_car.m_set1.m_heat1.m_km_num >= KM_ERRNUM);
	SetBit(buf, 282, 5, g_car.m_set1.m_heat2.m_km_num >= KM_ERRNUM);
	Set32(buf + 283, g_car.m_set1.m_compressor1.m_LpTimeRecord.LastRecordTimeGrap() / 1000);
	Set32(buf + 287, g_car.m_set1.m_compressor2.m_LpTimeRecord.LastRecordTimeGrap() / 1000);

	i = 300;
	// 机组1
	Set32(buf + i, g_car.m_set1.m_compressor1.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_compressor1.timer_Run.totalRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_compressor2.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_compressor2.timer_Run.totalRunTime());
	i += 4;

	Set32(buf + i, g_car.m_set1.m_condenser1.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser1.timer_Run.totalRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser2.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser2.timer_Run.totalRunTime());
	i += 4;

	Set32(buf + i, g_car.m_set1.m_vent1.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_vent1.timer_Run.totalRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_vent2.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_vent2.timer_Run.totalRunTime());
	i += 4;

	Set32(buf + i, g_car.m_set1.m_heat1.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_heat1.timer_Run.totalRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_heat2.timer_Run.thisRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_heat2.timer_Run.totalRunTime());
	i += 4;
	Set32(buf + i, g_car.m_set1.m_compressor1.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_compressor2.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser1.m_Fanctrl.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser1.m_FanBkctrl.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser2.m_Fanctrl.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_condenser2.m_FanBkctrl.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_vent1.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_vent2.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_heat1.m_km_num);
	i += 4;
	Set32(buf + i, g_car.m_set1.m_heat2.m_km_num);
	i += 4;

	mem_copy(buf + 701, (U8 *)&g_havctoccudata, sizeof(g_havctoccudata));
	mem_copy(buf + 501, (U8 *)&g_ccutohavcdata, sizeof(g_ccutohavcdata));
	mem_copy(buf + 601, (U8 *)&g_ccutoalldata, sizeof(g_ccutoalldata));

	buf[Tlen - 1] = GetParity(buf, Tlen - 1);
	return Tlen; // 反馈长度
}
void StoreDeviceIOInfo(char *p, int len) // PTU强制->控制器
{
	U8 *buf = (U8 *)p;
	int i;
	// 强制控制器主机DI
	for (i = 0; i < DI_NUM; i++)
		g_DI.SetForce(i, GetBit8(buf[21 + i / 8], i % 8), GetBit8(buf[5 + i / 8], i % 8));
	// 强制控制器主机DO
	for (i = 0; i < DO_NUM; i++)
		g_DO.SetForce(i, GetBit8(buf[49 + i / 8], i % 8), GetBit8(buf[37 + i / 8], i % 8));

	for (int i = 0; i < AI_NUM; i++)
	{
		ctrl_ai[i].force(GetBit8(buf[130 + i / 8], i % 8), ((AI_TYPE)(Get16(buf + 62 + (i * 2)))));
	}
	// DA
	for (int i = 0; i < DA_NUM; i++)
	{
		ctrl_DA[i].force(GetBit8(buf[134 + i / 8], i % 8), Get16(buf + 122 + (i * 2)) * 100);
		/*if (GetBit8(buf[134 + i / 8], i % 8)==1){
			ctrl_DA[i].set_da((Get16(buf + 122 + (i * 2)))*100);
		}*/
	}

	g_debug_var.var = Get32_LE(buf + 137);
	if ((g_debug_var.var & 0x80000000) == 0) // PTU debug模式
		g_debug_var.var = 0;

	g_car.set1.Compressor_1.force_freq(GetBit8(buf[146], 0), Get16(buf + 164));
	g_car.m_set1.m_compressor2.force_freq(GetBit8(buf[146], 1), Get16(buf + 166));
	g_car.m_set1.m_compressor1.force_state(GetBit8(buf[147], 0), Get16(buf + 180));
	g_car.m_set1.m_compressor2.force_state(GetBit8(buf[147], 1), Get16(buf + 182));
	g_car.m_set1.m_EEV1.force_heat(GetBit8(buf[148], 0), Get16(buf + 184));
	g_car.m_set1.m_EEV2.force_heat(GetBit8(buf[148], 1), Get16(buf + 186));

	TXdata_cahce[0] = GetBit8(buf[148], 3); // PPV 强制lock 输出DO
	TXdata_cahce[1] = GetBit8(buf[148], 4);

	if (GetBit8(buf[500], 0)) // PTU上的TRDP数据任一lock的状态
	{
		mem_copy((U8 *)&g_havctoccudata, buf + 501, sizeof(g_havctoccudata));
		mem_copy((U8 *)&g_ccutoalldata, buf + 701, sizeof(g_havctoccudata));
	}

	if (GetBit8(buf[600], 0))
	{
		mem_copy((U8 *)&g_ccutohavcdata, buf + 601, sizeof(g_ccutohavcdata));
	}
}

#endif

void Reply_DeviceIdentifyInfoPacket(MAINTENANCE_BUS bus)
{
	U8 *buf = (U8 *)pMainTaincePacket;

	buf[0] = 0xf5;
	Set16(buf + 1, 19); // PUT_HVAC新的维护软件,包长度修改为两个字节了
	buf[3] = 0x11;

	Set32(buf + 4, IAP_VERSION);

	Set32(buf + 8, IAP_ID);

	buf[12] = 23;
	buf[13] = 8;
	buf[14] = 24;
	buf[15] = 15;
	buf[16] = 24;
	buf[17] = 25;

	buf[18] = GetParity(buf, 18);
	maintenance_send(bus, buf, 19);
}

void Reply_DeviceIOInfoPacket(MAINTENANCE_BUS bus)
{
	U8 *buf = (U8 *)pMainTaincePacket;

	int Tlen = GetDeviceIOInfoPacket(buf, 2048);

	if (Tlen > 0)
		maintenance_send(bus, buf, Tlen);
}
