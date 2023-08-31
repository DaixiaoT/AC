
#include "ac_ai.h"

AI_CTRL ctrl_AI[AI_NUM];
BOOL DampCalcCallback(S16 ad, AI_TYPE *pro_v, AI_TYPE *valve)
{

	U32 R = 4750 * ad / (0x7fff - ad);
	*pro_v = R;
	*valve = R;
	return TRUE;
}
// static SENSOR_CONFIG sensor_ntc5k_cfg = {NTC5KCalcCallback, 1000, -400, -400, 8, FILTAI_MID, 0, 100};
static SENSOR_CONFIG sensor_damp_cfg = {DampCalcCallback, 5000, 0, 0, 8, FILTAI_MID, 0, 100};
// static SENSOR_CONFIG sensor_vol_cfg = {VolCalcCallback, 300, 0, 0, 8, FILTAI_MID, 0, 1000};
// static SENSOR_CONFIG sensor_curr_cfg = {CurrentCalcCallback, 300, 0, 0, 8, FILTAI_MID, 0, 1000}; // 放
// static SENSOR_CONFIG sensor_HP_cfg = { HPCalcCallback,10000,0,0,5,FILTAI_MID,0,100};  //
// static SENSOR_CONFIG sensor_HP_cfg = {HPCalcCallback, 300, 0, -1, 5, FILTAI_MID, 0, 100}; //

// static SENSOR_CONFIG sensor_CO2_cfg = {CO2CalcCallback, 10000, 0, -1, 5, FILTAI_MID, 0, 100};

const AI_INFO ai_info[AI_NUM] = {
	// {"机组1新风温度", 0, &sensor_ntc5k_cfg},
	// {"机组1回风温度1", 0, &sensor_ntc5k_cfg},
	// {"机组1送风温度", 0, &sensor_ntc5k_cfg},
	{"机组1新风门", 0, &sensor_damp_cfg},
	// {"机组1回风温度2", 0, &sensor_ntc5k_cfg},
	// {"NTC06", 0, &sensor_ntc5k_cfg},

	// {"机组11高压传感器", 0, &sensor_HP_cfg},
	// {"机组12高压传感器", 0, &sensor_HP_cfg},
	// {"机组二氧化碳传感器", 0, &sensor_CO2_cfg},
	// {"CL04", 0, &sensor_curr_cfg},
	// {"CL05", 0, &sensor_curr_cfg},
	// {"CL06", 0, &sensor_curr_cfg},

	// {"VIN01", 0, &sensor_vol_cfg},
	// {"VIN02", 0, &sensor_vol_cfg},

};

void AI_init()
{
	LOG_PRINT("Enter AI_init()\n");
	for (int i = 0; i < AI_NUM; i++)
	{
		ctrl_AI[i].config(ai_info[i].name, ai_info[i].sensor_cfg);
	}
}

void AI_Updata()
{
}

U16 ReadFreshAirDampFeedback()
{
	return (U16)ctrl_AI[0].getValue();
}

U16 ReadHighPressureValue_1()
{
	return ctrl_AI[6].getValue();
}
U16 ReadHighPressureValue_2()
{
	return ctrl_AI[7].getValue();
}

void ai_init()
{
}

void AI_CTRL::config(const char *name, SENSOR_CONFIG *cfg)
{
	if (cfg->call == NULL)
	{
		LOG_PRINT("func==NULL");
		return;
	}
	bad_value = cfg->bad;
	value = bad_value;
	adjust_offset = cfg->adj_offset;
	adjust_p = cfg->adj_p;
}
