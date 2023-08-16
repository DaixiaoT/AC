#ifndef __AI_UDPATE_H__
#define __AI_UDPATE_H__
#include <stdlib.h>
#include "sys_api.h"



typedef float AI_TYPE;
//模拟量控件
typedef BOOL(*AICalcCallback)(S16 ad, AI_TYPE* pro_v, AI_TYPE* value);//pro_v为计算过程数据

enum FILTAI_MODE {
	FILTAI_AVG = 0,
	FILTAI_MID = 1,
};

#define AD_FILT_BUF_MAX	16
class FILT_AIRT {
public:
	FILT_AIRT()
	{
		init(1, FILTAI_MID);
	}
	void init(U8 filt_num, FILTAI_MODE mode) {
		if ((filt_num == 0) || (filt_num > AD_FILT_BUF_MAX)) {
			PRINT_ERR_INT(filt_num);
			filt_num = AD_FILT_BUF_MAX;
		}
		m_filt_num = filt_num;
		m_mode = mode;
		m_ok = 0;
	}

	S16 get_filter_val(S16 data)
	{
		if (m_filt_num == 1) {
			return data;
		}
		if (m_ok) {
			for (int i = 0; i < m_filt_num; i++)
				m_buf[i] = data;
			m_ok = 1;
			m_idx = 1;
			return data;
		}

		m_buf[m_idx] = data;
		m_idx++;
		if (m_idx >= m_filt_num)
			m_idx = 0;

		if (m_mode == FILTAI_AVG) {
			U32 sum = m_buf[0];
			for (int i = 1; i < m_filt_num; i++) {
				sum += m_buf[i];
			}
			return sum / m_filt_num;
		}
		else {
			S16 buf[AD_FILT_BUF_MAX];
			int i, j;

			for (i = 0; i < m_filt_num; i++)
				buf[i] = m_buf[i];

			for (i = 0; i < m_filt_num - 1; i++) {
				for (j = i + 1; j < m_filt_num; j++) {
					if (buf[j] > buf[i]) {
						int t = buf[i];
						buf[i] = buf[j];
						buf[j] = t;
					}
				}
			}

			return (buf[m_filt_num / 2 - 1] + buf[m_filt_num / 2]) / 2;
		}
	}



private:
	FILTAI_MODE m_mode;
	U8 m_filt_num;
	S16 m_buf[AD_FILT_BUF_MAX];
	int m_ok;
	int m_idx;
};


typedef struct {
	AICalcCallback call;
	AI_TYPE max;
	AI_TYPE min;
	AI_TYPE bad;

	U8 filt_num;	//滤波次数 最大16
	FILTAI_MODE filt_mode;

	S16 adj_offset;	//数值校准偏移量	默认0
	U16 adj_p;		//数值校准偏移系数	默认100
}SENSOR_CONFIG;
//{CurrentCalcCallback, 300, 0, 0, 5, FILT_MID, 0, 1000};
class ai_ctrl
{
public:
	ai_ctrl() {
		m_name = NULL;
		m_bad_value = 0xffff;
		m_value = m_bad_value;
		m_value_f = m_bad_value;
		m_pCalcFunc = NULL;
		m_force_flag = FALSE;
		m_max_v = 0xffff - 1;
		m_min_v = 0;
		m_adjust_offset = 0;
		m_adjust_p = 100;
	}

	void config(const char* name, SENSOR_CONFIG* cfg) {
		if (cfg->call == NULL)
		{
			PRINT_ERR_STR("func==NULL");
			return;
		}

		m_filt.init(cfg->filt_num, cfg->filt_mode);

		m_name = name;
		m_pCalcFunc = cfg->call;
		m_max_v = cfg->max;
		m_min_v = cfg->min;
		m_bad_value = cfg->bad;
		m_value = m_bad_value;
		m_value_f = m_bad_value;
		m_adjust_offset = cfg->adj_offset;
		m_adjust_p = cfg->adj_p;
	}

	void dump()
	{
		LOG_PRINT("%s\tad:%04x\t%d.%d\t\tv:%d.%d", m_name, m_ad, (S32)(m_pro_v), abs((S32)(m_pro_v * 10.0) % 10), (S32)(value()), abs((S32)(value() * 10.0) % 10));
	}

	U16 get_ad()
	{
		return m_ad;
	}

	BOOL set_ad(U16 ad) {
		if (m_pCalcFunc == NULL)
		{
			PRINT_ERR_INT(ad);
			return FALSE;
		}

		ad = m_filt.get_filter_val(ad);

		S16 ad1 = (S16)ad;

		AI_TYPE v;
		BOOL ret = m_pCalcFunc(ad1, &m_pro_v, &v);
		//LOG_PRINT("%s ad:0x%x -> %d\r\n",m_name,ad,v);
		if (ret) {
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
		else {
			m_value = m_bad_value;
			PRINT_ERR_INT(ad);
			PRINT_ERR_INT(v);
		}

		m_ad = ad;
		return ret;
	}

	BOOL force(BOOL bForce, AI_TYPE v) {
		m_force_flag = bForce;
		m_value_f = v;
		LOG_PRINT("ERROR set_ad [%s] ->%d\r\n", m_name, m_value_f);
		return TRUE;
	}

	AI_TYPE value()
	{
		if (m_force_flag)
			return m_value_f;
		return (m_value + (AI_TYPE)m_adjust_offset) * (AI_TYPE)m_adjust_p / 100.0;
	}

	AI_TYPE bad_value()
	{
		return m_bad_value;
	}

	BOOL is_error()
	{
		return value() == bad_value();
	}

	void set_err(U8 err)
	{
		m_err_no = err;
		if (err)
			LOG_PRINT("%s err:%d\r\n", m_name, err);
	}

	U8 get_err()
	{
		return m_err_no;
	}

private:
	const char* m_name;
	AI_TYPE m_max_v;
	AI_TYPE m_min_v;
	AI_TYPE m_value;
	AICalcCallback	m_pCalcFunc;
	BOOL m_force_flag;
	AI_TYPE m_value_f;
	AI_TYPE m_bad_value;
	S16 m_adjust_offset;
	U16 m_adjust_p;
	U16 m_ad;

	FILT_AIRT m_filt;

	AI_TYPE m_pro_v;//计算中过程数据
	U8 m_err_no;
};

BOOL CurrentCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL HPCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL CO2CalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL NTC5KCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL VolCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
BOOL DampCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);
#define TEMP_MIN -400
#define TEMP_MAX  600
#define TEMP_BAD  TEMP_MIN


#define VOL_MIN	 0
#define VOL_MAX  1000
#define VOL_BAD  VOL_MAX












void ai_init(void);

void ai_update(void);

U16 get_ai(int n);


#endif//__AI_UDPATE_H__

