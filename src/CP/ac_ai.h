#ifndef __AI_UDPATE_H__
#define __AI_UDPATE_H__

#include "ac_include.h"
#include "ac_filter.h"
/*
Analog Input 模拟量输入
*/

typedef float AI_TYPE;

typedef BOOL(*AICalcCallback)(S16 ad, AI_TYPE* pro_v, AI_TYPE* value); // pro_v为计算过程数据

typedef struct
{
	AICalcCallback call;
	AI_TYPE max;
	AI_TYPE min;
	AI_TYPE bad;

	U8 filt_num; // 滤波次数 最大16
	FILTAI_MODE filt_mode;

	S16 adj_offset; // 数值校准偏移量	默认0
	U16 adj_p;		// 数值校准偏移系数	默认100
} SENSOR_CONFIG;

typedef struct
{
	const char* name;
	U16 modbus_regaddr;
	SENSOR_CONFIG* sensor_cfg;
} AI_INFO;

extern const AI_INFO ai_info[AI_NUM];

class AI_CTRL
{
public:
	AI_CTRL()
	{
		m_name = NULL;
		m_bad_value = 0xffff;
		m_value = m_bad_value;
		m_value_force = m_bad_value;
		m_pCalcFunc = NULL;
		m_force_flag = FALSE;
		m_max_v = 0xffff - 1;
		m_min_v = 0;
		m_adjust_offset = 0;
		m_adjust_p = 100;
	}
	AI_TYPE getValue()
	{
		if (m_force_flag)
		{
			return m_value_force;
		}
		return (m_value + (AI_TYPE)m_adjust_offset) * (AI_TYPE)m_adjust_p / 100.0;
	}
	AI_TYPE getBadValue() {
		return m_bad_value;
	}
	void config(const char* name, SENSOR_CONFIG* cfg)
	{
		if (cfg->call == NULL)
		{
			LOG_PRINT("func==NULL");
			return;
		}
		m_name = name;
		m_filt.init(cfg->filt_num, cfg->filt_mode);
		m_pCalcFunc = cfg->call;
		m_bad_value = cfg->bad;
		m_value = m_bad_value;
		m_value_force = m_bad_value;
		m_adjust_offset = cfg->adj_offset;
		m_adjust_p = cfg->adj_p;
	}

	BOOL force(BOOL Force, AI_TYPE value)
	{
		m_force_flag = Force;
		m_value_force = value;
		// LOG_PRINT("Set_AI %d\n",force_value);
		return TRUE;
	}
	BOOL isError() {
		return getValue() == getBadValue();
	}
	BOOL set_ad(U16 ad) {
		if (m_pCalcFunc == NULL)
		{
			PRINT_ERR_INT(ad);
			LOG_PRINT("\nm_pCalcFunc is Null Error!\n");
			return FALSE;
		}

		ad = m_filt.get_filter_val(ad);//滤波处理

		S16 ad1 = (S16)ad;

		AI_TYPE v;
		BOOL ret = m_pCalcFunc(ad1, &m_pro_v, &v);
		//LOG_PRINT("%s ad:0x%x -> %d\r\n",m_name,ad,v);
		if (ret) {//计算成功
			if ((v >= m_max_v) || (v <= m_min_v))
			{//超出范围
				m_value = m_bad_value;
				//LOG_PRINT("ERROR set_ad [%s] %d->%d\r\n",m_name,ad,v);
			}
			else
			{
				m_value = v;
			}
		}
		else {//计算失败
			m_value = m_bad_value;
			PRINT_ERR_INT(ad);
			PRINT_ERR_INT(v);
		}

		m_ad = ad;
		return ret;
	}
	

private:
	const char* m_name;
	FILT_AIRT m_filt;
	AI_TYPE m_max_v;
	AI_TYPE m_min_v;
	BOOL m_force_flag;
	AI_TYPE m_value;
	AI_TYPE m_value_force;
	AI_TYPE m_bad_value;
	AICalcCallback m_pCalcFunc;
	S16 m_adjust_offset;
	U16 m_adjust_p;
	U16 m_ad;

	AI_TYPE m_pro_v;//计算中过程数据
};

// AI结构体
extern AI_CTRL ctrl_AI[AI_NUM]; // ntc
BOOL DampCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL CurrentCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL CO2CalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL NTC5KSCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL NTC5KCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL VolCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL HPCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);



void AI_Updata();

void AI_init();

void PrintAI();

U16 U1_RD_F1();

S16 U1HP1();

S16 U1HP2();

BOOL Check_TempError();

#endif //__AI_UDPATE_H__
