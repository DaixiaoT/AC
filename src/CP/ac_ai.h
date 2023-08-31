#ifndef __AI_UDPATE_H__
#define __AI_UDPATE_H__

#include "ac_include.h"

typedef float AI_TYPE;

typedef BOOL (*AICalcCallback)(S16 ad, AI_TYPE *pro_v, AI_TYPE *value); // pro_v为计算过程数据
enum FILTAI_MODE
{
	FILTAI_AVG = 0,
	FILTAI_MID = 1,
};
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
	const char *name;
	U16 modbus_regaddr;
	SENSOR_CONFIG *sensor_cfg;
} AI_INFO;

extern const AI_INFO ai_info[AI_NUM];

class AI_CTRL
{
public:
	AI_CTRL()
	{
		bad_value = 0xffff;
		value = bad_value;
		force_value = bad_value;
		force_flag = FALSE;
	}
	AI_TYPE getValue()
	{
		if (force_flag)
		{
			return force_value;
		}
		return value;
	}
	void config(const char *name, SENSOR_CONFIG *cfg);
	BOOL force(BOOL Force, AI_TYPE value)
	{
		force_flag = Force;
		force_value = value;
		// LOG_PRINT("Set_AI %d\n",force_value);
		return TRUE;
	}
	BOOL isError(){
		return value==bad_value;
	}

private:
	BOOL force_flag;
	AI_TYPE value;
	AI_TYPE force_value;
	AI_TYPE bad_value;
	S16 adjust_offset;
	U16 adjust_p;
};

// AI结构体
extern AI_CTRL ctrl_AI[AI_NUM]; // ntc
BOOL DampCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve);

// void AI_init();

void AI_Updata();

void AI_init();

U16 ReadFreshAirDampFeedback();
U16 ReadHighPressureValue_1();

U16 ReadHighPressureValue_2();

// void ai_init(void);

#endif //__AI_UDPATE_H__
