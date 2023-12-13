
#include "ac_ai.h"
#include "ac_ctrl.h"
#include "ad.h"
#include "ntc_5K.h"
#include "ntc_5K_S.h"
/*
Analog Input 模拟量输入
*/
AI_CTRL ctrl_AI[AI_NUM];

#define VOL(ad) ((float)((float)4096.0*(float)ad)/ (0x7fff))
#define VOL1(ad,base) ((float)((float)(base)*(float)ad)/ (0x7fff))

// 根据r值，返回温度:
static int R2Temp(U32 r)
{
	int a = 0;
	int b = RT_Table_NUM / 2;
	int c = RT_Table_NUM - 1;

	if (r >= RT_Tabel[0].r)
		return RT_Tabel[0].t;
	if (r <= RT_Tabel[c].r)
		return RT_Tabel[c].t;

	while (a < b && b < c) {
		if (r >= RT_Tabel[b].r)
			c = b;
		else
			a = b;

		b = (a + c) / 2;
	}

	//LOG_PRINT("a=%d b=%d c=%d\n", a, b, c);
	return RT_Tabel[a].t + (RT_Tabel[a].r - r) * (RT_Tabel[c].t - RT_Tabel[a].t) / ((RT_Tabel[a].r - RT_Tabel[c].r));
}

static int RS2Temp(U32 r)
{
	int a = 0;
	int b = RST_Table_NUM / 2;
	int c = RST_Table_NUM - 1;

	if (r >= RST_Tabel[0].r)
		return RST_Tabel[0].t;
	if (r <= RST_Tabel[c].r)
		return RST_Tabel[c].t;

	while (a < b && b < c) {
		if (r >= RST_Tabel[b].r)
			c = b;
		else
			a = b;

		b = (a + c) / 2;
	}

	//LOG_PRINT("a=%d b=%d c=%d\n", a, b, c);
	return RST_Tabel[a].t + (RST_Tabel[a].r - r) * (RST_Tabel[c].t - RST_Tabel[a].t) / ((RST_Tabel[a].r - RST_Tabel[c].r));
}

BOOL DampCalcCallback(S16 ad, AI_TYPE *pro_v, AI_TYPE *valve)
{

	U32 R = 4750 * ad / (0x7fff - ad);
	*pro_v = R;
	*valve = R;
	return TRUE;
}
BOOL CurrentCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	float vol = VOL1(ad, 4096.0);
	*pro_v = *valve = (vol / 120.0);//单位 1ma
	return TRUE;
}

#define CO2_PPM(a)((a - (float)4.0)/0.0016) //
BOOL CO2CalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	float vol = VOL1(ad, 4096.0);
	float curr = (vol / 120.0);
	*pro_v = (vol / 120.0);//单位 1ma
	*valve = (AI_TYPE)((int)((CO2_PPM(curr))));
	return TRUE;
}
BOOL NTC5KSCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	U32 R = 4750 * ad / (0x7fff - ad);

	float vol = VOL1(ad, 4096.0);
	//LOG_PRINT("ad:0x%x v:%d R:%d\n",ad,(int)vol,R);

	*pro_v = R;
	int t = RS2Temp(R);
	*valve = (AI_TYPE)t;
	return TRUE;
}
BOOL NTC5KCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	U32 R = 4750 * ad / (0x7fff - ad);

	float vol = VOL1(ad, 4096.0);
	//LOG_PRINT("ad:0x%x v:%d R:%d\n",ad,(int)vol,R);

	*pro_v = R;
	int t = R2Temp(R);
	*valve = (AI_TYPE)t;
	return TRUE;
}

BOOL VolCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	float vol = VOL1(ad, 6144.0);
	vol /= 100;
	vol *= 2.0;//做了分压
	*pro_v = *valve = vol / 10.0;//单位 1v
	return TRUE;
}

#define HIGH_BA(a)((a - (float)4.0)/0.533) //
BOOL HPCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	float vol = VOL1(ad, 4096.0);
	float curr = (vol / 120.0);
	*pro_v = (vol / 120.0);//单位 1ma

	*valve = (AI_TYPE)((int)((HIGH_BA(curr)) * 10));
	return TRUE;
}
 static SENSOR_CONFIG sensor_ntc5kS_cfg = { NTC5KSCalcCallback, 1000, -400, -400, 8, FILTAI_MID, 0, 100 };
 static SENSOR_CONFIG sensor_ntc5k_cfg = {NTC5KCalcCallback, 1000, -400, -400, 8, FILTAI_MID, 0, 100};
 static SENSOR_CONFIG sensor_damp_cfg = {DampCalcCallback, 5000, 0, 0, 8, FILTAI_MID, 0, 100};
 static SENSOR_CONFIG sensor_vol_cfg = {VolCalcCallback, 300, 0, 0, 8, FILTAI_MID, 0, 1000};
 static SENSOR_CONFIG sensor_curr_cfg = {CurrentCalcCallback, 300, 0, 0, 8, FILTAI_MID, 0, 1000}; // 放
 //static SENSOR_CONFIG sensor_HP_cfg = { HPCalcCallback,10000,0,0,5,FILTAI_MID,0,100};  //
 static SENSOR_CONFIG sensor_HP_cfg = {HPCalcCallback, 300, 0, -1, 5, FILTAI_MID, 0, 100}; //

 static SENSOR_CONFIG sensor_CO2_cfg = {CO2CalcCallback, 10000, 0, -1, 5, FILTAI_MID, 0, 100};

const AI_INFO ai_info[AI_NUM] = {
	{"机组1新风温度", 0, &sensor_ntc5kS_cfg},
	{"机组1回风温度1", 0, &sensor_ntc5kS_cfg},
	{"机组1送风温度", 0, &sensor_ntc5k_cfg},
	{"机组1新风门", 0, &sensor_damp_cfg},
	{"机组1回风温度2", 0, &sensor_ntc5kS_cfg},
	{"NTC06", 0, &sensor_ntc5k_cfg},

	{"机组11高压传感器", 0, &sensor_HP_cfg},
	{"机组12高压传感器", 0, &sensor_HP_cfg},
	{"机组二氧化碳传感器", 0, &sensor_CO2_cfg},
	{"CL04", 0, &sensor_curr_cfg},
	{"CL05", 0, &sensor_curr_cfg},
	{"CL06", 0, &sensor_curr_cfg},

	{"VIN01", 0, &sensor_vol_cfg},
	{"VIN02", 0, &sensor_vol_cfg},

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
	//PrintAI();
	static U32 s_tick = 0;
	static U8 s_ch = 0;
	//?
	if (TimeGap(s_tick) < 41) {
		return;
	}
	s_tick = sys_time();
	U16 ad;
	if (s_ch < 6) {
		get_ad_ntc(s_ch, &ad, 10);
		ctrl_AI[s_ch].set_ad(ad);
	}
	else if (s_ch >= 6 && s_ch < 12)
	{
		get_ad_current4_20ma(s_ch - 6, &ad, 10);
		ctrl_AI[s_ch].set_ad(ad);
	}
	else if (s_ch >= 12 && s_ch < 14)
	{
		get_ad_vol_0_5V(s_ch - 12, &ad, 10);
		ctrl_AI[s_ch].set_ad(ad);
	}

	s_ch++;
	if (s_ch >= AI_NUM)
		s_ch = 0; // 共16路NTC

}

void PrintAI() {
	for (int i = 0; i < 16; i++) {
		LOG_PRINT("\n%d:isError:%d,ad:%.2f\n",i, ctrl_AI[i].isError(), ctrl_AI[i].getValue());
	}
}


void ai_init()
{


}
U16 U1_RD_F1()
{
	return (U16)ctrl_AI[3].getValue();
}

S16 U1HP1()
{
	return ctrl_AI[6].getValue();
}
S16 U1HP2()
{
	return ctrl_AI[7].getValue();
}



