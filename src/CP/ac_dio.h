#ifndef _AC_DIO_H_
#define _AC_DIO_H_
enum DI_DEFINE{
	
	DI_CP1_Feedback = 0, //��Ƶ��1΢�Ϸ���Q1
	DI_CP2_Feedback,     //��Ƶ��2΢�Ϸ���Q2
	DI_HEATER1_Feedback, //�����1�Ӵ�������K3
	DI_HEATER2_Feedback, //�����2�Ӵ�������K4
	DI_CDF1_Feedback,      //�������1�Ӵ�������K5
	DI_CDF1_Redundance_Feedback,// �������1����Ӵ�������K6
	DI_CDF2_Feedback,//�������2�Ӵ�������K7
	DI_CDF2_Redundance_Feedback,//�������2����Ӵ�������K8
	DI_SAF1_Feedback,//�ͷ��1�Ӵ�������K9
	DI_SAF2_Feedback,//�ͷ��2�Ӵ�������K10
	DI_3P_POWER_DETECTOR,//�����Դ��ⷴ��
	DI_SAF1_State,//�ͷ��1״̬����
	DI_SAF2_State,//�ͷ��2״̬����
	DI_TEMP_SWITCH_1,			// TEMP switch 1
	DI_TEMP_SWITCH_2,			// TEMP switch 2
	DI_TEMP_SWITCH_3,			// TEMP switch 3
	DI_TEMP_SWITCH_4,			// TEMP switch 3
	DI_CP1_HP, //��ѹ����1
	DI_CP2_HP, //��ѹ����2
	DI_CP1_LP, // ��ѹ����1
	DI_CP2_LP, //��ѹ����2
	DI_SPARE_22, //����
	DI_MODE_SWITCH_POWER, //ģʽ���عػ�����
	DI_MODE_SWITCH_MT,    //ģʽ�����ֶ�ģʽ����
	DI_MODE_SWITCH_AT,    //ģʽ�����Զ�ģʽ����
	DI_Urgent_Ventilation,  //����ͨ��Ӳ���źţ��ߵ�ƽ��Ч��
	DI_Fire_War,   //�����źţ��ߵ�ƽ��Ч��
	DI_LINE1_SIGNAL,// ����Ӳ���źţ����ã�
	DI_LINE2_SIGNAL, //����Ӳ���źţ����ã�
	DI_SPARE_30, //����
	DI_SPARE_31, //����
	DI_SPARE_32, //����
	DI_SPARE_33, //����
	DI_SPARE_34, //����
	DI_SPARE_35, //����
	DI_CAR_ID1,					// ��������1
	DI_CAR_ID2,					// ��������2
	DI_CAR_ID3,					// ��������3
	DI_NULL
};

enum DO_DEFINE
{
	DO_CP1_Contactor = 0, //��Ƶ��1�����ź�KA01
	DO_CP2_Contactor,     //��Ƶ��2�����ź�KA02
	DO_HEATER1_Contactor, //�����1�Ӵ���K3
	DO_HEATER2_Contactor, //�����2�Ӵ���K4
	DO_CDF1_Contactor,     //�������1�Ӵ���K5
	DO_CDF1_Redundance,    //�������1����Ӵ���K6
	DO_CDF2_Contactor,     //�������2�Ӵ���K7
	DO_CDF2_Redundance,    //�������2����Ӵ���K8
	DO_SAF1_Contactor,     //�ͷ��1�Ӵ���K9
	DO_SAF2_Contactor,     //�ͷ��2�Ӵ���K10
	DO_FAS_Open,           //�·��ſ�
	DO_FAS_Close,          //�·��Ź�
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