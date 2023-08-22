#ifndef _AC_DIO_H_
#define _AC_DIO_H_
enum DI_DEFINE{
	
	DI_CP1_Feedback = 0, //变频器1微断反馈Q1
	DI_CP2_Feedback,     //变频器2微断反馈Q2
	DI_HEATER1_Feedback, //电加热1接触器反馈K3
	DI_HEATER2_Feedback, //电加热2接触器反馈K4
	DI_CDF1_Feedback,      //冷凝风机1接触器反馈K5
	DI_CDF1_Redundance_Feedback,// 冷凝风机1冗余接触器反馈K6
	DI_CDF2_Feedback,//冷凝风机2接触器反馈K7
	DI_CDF2_Redundance_Feedback,//冷凝风机2冗余接触器反馈K8
	DI_SAF1_Feedback,//送风机1接触器反馈K9
	DI_SAF2_Feedback,//送风机2接触器反馈K10
	DI_3P_POWER_DETECTOR,//三相电源检测反馈
	DI_SAF1_State,//送风机1状态反馈
	DI_SAF2_State,//送风机2状态反馈
	DI_TEMP_SWITCH_1,			// TEMP switch 1
	DI_TEMP_SWITCH_2,			// TEMP switch 2
	DI_TEMP_SWITCH_3,			// TEMP switch 3
	DI_TEMP_SWITCH_4,			// TEMP switch 3
	DI_CP1_HP, //高压开关1
	DI_CP2_HP, //高压开关2
	DI_CP1_LP, // 低压开关1
	DI_CP2_LP, //低压开关2
	DI_SPARE_22, //空余
	DI_MODE_SWITCH_POWER, //模式开关关机反馈
	DI_MODE_SWITCH_MT,    //模式开关手动模式反馈
	DI_MODE_SWITCH_AT,    //模式开关自动模式反馈
	DI_Urgent_Ventilation,  //紧急通风硬线信号（高电平有效）
	DI_Fire_War,   //火灾信号（高电平有效）
	DI_LINE1_SIGNAL,// 车辆硬线信号（备用）
	DI_LINE2_SIGNAL, //车辆硬线信号（备用）
	DI_SPARE_30, //空余
	DI_SPARE_31, //空余
	DI_SPARE_32, //空余
	DI_SPARE_33, //空余
	DI_SPARE_34, //空余
	DI_SPARE_35, //空余
	DI_CAR_ID1,					// 车辆编码1
	DI_CAR_ID2,					// 车辆编码2
	DI_CAR_ID3,					// 车辆编码3
	DI_NULL
};

enum DO_DEFINE
{
	DO_CP1_Contactor = 0, //变频器1启动信号KA01
	DO_CP2_Contactor,     //变频器2启动信号KA02
	DO_HEATER1_Contactor, //电加热1接触器K3
	DO_HEATER2_Contactor, //电加热2接触器K4
	DO_CDF1_Contactor,     //冷凝风机1接触器K5
	DO_CDF1_Redundance,    //冷凝风机1冗余接触器K6
	DO_CDF2_Contactor,     //冷凝风机2接触器K7
	DO_CDF2_Redundance,    //冷凝风机2冗余接触器K8
	DO_SAF1_Contactor,     //送风机1接触器K9
	DO_SAF2_Contactor,     //送风机2接触器K10
	DO_FAS_Open,           //新风门开
	DO_FAS_Close,          //新风门关
	DO_SPARE_13,				//Spare
	DO_SPARE_14,				//Spare
	DO_SPARE_15,				//Spare
	DO_SPARE_16,				//Spare
	DO_SPARE_17,				//Spare
	DO_SPARE_18,				//Spare
	DO_SPARE_19,				//Spare
	DO_SPARE_20,				//Spare
	DO_NULL
};

#endif // _AC_DIO_H_