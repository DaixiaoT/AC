#ifndef __TRDP_DATA_H__
#define __TRDP_DATA_H__

typedef unsigned short    BITFIELD;
typedef unsigned char   BOOLEAN1;
typedef unsigned char   ANTIVALENT2;
typedef unsigned char   BCD4;
typedef unsigned char   ENUM4;
typedef unsigned char   BITSET8;
typedef unsigned char   ENUM8;
typedef unsigned char   UNSIGNED8;
typedef char            TRDP_INTEGER8;
typedef char            CHARACTER8;
typedef unsigned short  BITSET16;
typedef unsigned short  ENUM16;
typedef unsigned short  UNSIGNED16;
typedef short           TRDP_INTEGER16;
typedef float           TRDP_REAL32;
typedef unsigned long   BITSET32;
typedef unsigned long   UNSIGNED32;
typedef long            TRDP_INTEGER32;
typedef unsigned long   BITSET64;
typedef unsigned long   TRDP_INTEGER64;
typedef long            INTEGER64;


#pragma  pack (push,1)   
typedef struct TRDP_COMM_DATA
{
	UNSIGNED32	CCU_Lifesign_U32;//	0		8	CCU生命信号HH	CCU每个端口周期增加1	1=1	 0-4294967295	
	BOOLEAN1	CCU_N_B1	:1;//	4	b0	1	列车惰行工况		1=有效；0=无效		
	BOOLEAN1	CCU_B_B1	:1;//	4	b1	1	列车制动工况		1=有效；0=无效		
	BOOLEAN1	CCU_P_B1	:1;//	4	b2	1	列车牵引工况		1=有效；0=无效		
	BOOLEAN1	CCU_WheelSet_B1	:1;//	4	b3	1	轮径设置		1=有效；0=无效		3s的脉冲
	BOOLEAN1	CCU_R_B1	:1;//	4	b4	1	列车后向状态	向后	1=向后		
	BOOLEAN1	CCU_F_B1	:1;//	4	b5	1	列车前向状态	向前	1=向前		以激活端为准
	BOOLEAN1	CCU_Cab2Opera_B2	:1;//	4	b6	1	司机室2激活	4车主控	1=激活；0=无效		
	BOOLEAN1	CCU_Cab1Opera_B1	:1;//	4	b7	1	司机室1激活	1车主控	1=激活；0=无效		
	BOOLEAN1	CCU_TimeSet_B1	:1;//	5	b0	1	日期时间设定		1=有效；0=无效		
	BOOLEAN1	CCU_SpeedAvi_B1	:1;//	5	b1	1	速度有效		1=有效；0=无效		
	BOOLEAN1	CCU_PBValueAvi_B1	:1;//	5	b2	1	牵引制动级位有效		1=有效；0=无效		
	BOOLEAN1	CCU_TimeAvi_B1	:1;//	5	b3	1	时间有效		1=有效；0=无效		
	BOOLEAN1	CCU_TrainNumAvi_B1	:1;//	5	b4	1	列车号有效		1=有效；0=无效		
	BOOLEAN1	CCU_WheelAvi_B1	:1;//	5	b5	1	轮径有效		1=有效；0=无效		
	BOOLEAN1	CCU_BYTE5_BIT6	:1;//	5	b6	1					
	BOOLEAN1	CCU_BYTE5_BIT7	:1;//	5	b7	1					
	UNSIGNED8	CCU_Speed100_H_U8;//	6		8	列车速度H		1=0.01km/h	0-2000	
	UNSIGNED8	CCU_Speed100_L_U8;//	7		8	列车速度L		1=0.01km/h		
	UNSIGNED8	CCU_TrainNum_H_U8;//	8		8	列号H	列车编号	1=1		
	UNSIGNED8	CCU_TrainNum_L_U8;//	9		8	列号L	列车编号	1=1		
	UNSIGNED8	CCU_Year_U8;//	10		8	年	0-99，代表2000-2099年	1=1	0-99 	用于各系统与TCMS时间同步
	UNSIGNED8	CCU_Month_U8;//	11		8	月	1-12，代表1-12月	1=1	43842	
	UNSIGNED8	CCU_Day_U8;//	12		8	日	1-31，代表1-31日	1=1	43861	
	UNSIGNED8	CCU_Hour_U8;//	13		8	时	0-23，代表0-23时	1=1	0-23	
	UNSIGNED8	CCU_Minute_U8;//	14		8	分	0-59，代表0-59分	1=1	0-59	
	UNSIGNED8	CCU_Second_U8;//	15		8	秒	0-59，代表0-59秒	1=1	0-59	
	UNSIGNED8	Car1Axle2Wheel_H_U8;//	16		8	1车2轴轮径H	轮径值高字节	1=1mm		
	UNSIGNED8	Car1Axle2Wheel_L_U8;//	17		8	1车2轴轮径L	轮径值低字节	1=1mm		
	UNSIGNED8	Car2Axle2Wheel_H_U8;//	18		8	2车2轴轮径H	轮径值高字节	1=1mm		
	UNSIGNED8	Car2Axle2Wheel_L_U8;//	19		8	2车2轴轮径L	轮径值低字节	1=1mm		
	UNSIGNED8	Car3Axle2Wheel_H_U8;//	20		8	3车2轴轮径H	轮径值高字节	1=1mm		
	UNSIGNED8	Car3Axle2Wheel_L_U8;//	21		8	3车2轴轮径L	轮径值低字节	1=1mm		
	UNSIGNED8	Car4Axle2Wheel_H_U8;//	22		8	4车2轴轮径H	轮径值高字节	1=1mm		
	UNSIGNED8	Car4Axle2Wheel_L_U8;//	23		8	4车2轴轮径L	轮径值低字节	1=1mm		
	UNSIGNED8	CCU_BYTE24;//	24		8					
	UNSIGNED8	CCU_PBValue_U8;//	25		8	牵引制动级位	0-7对应0到7级	1=1	0-7	

	UNSIGNED8	res[74];//预留74字节，总长度100字节
};

typedef struct TRDP_CCU2HVAC
{
	//BYTE0~3
	UNSIGNED32 CCU_Lifesign_U32;//CCU每个端口周期增加1	1=1	 0-4294967295
	//BYTE4
	BOOLEAN1	CCU_SIV2OK_B1 :1;//	4	b0	辅助逆变器SIV2状态OK	CCU_SIV2OK_B1	当两台辅助逆变器正常工作时，空调正常工作。当两台辅助逆变器均不正常时，所有客室空调停机，当有一台工作不正常时，有三项电的两台机组进入减载模式，两台三相电不正常的机组进入紧急通风模式，当三相电恢复后，所有可是机组进入减载模式			
	BOOLEAN1	CCU_SIV1OK_B1 :1;//	4	b1	辅助逆变器SIV1状态OK	CCU_SIV1OK_B1	当两台辅助逆变器正常工作时，空调正常工作。当两台辅助逆变器均不正常时，所有客室空调停机，当有一台工作不正常时，有三项电的两台机组进入减载模式，两台三相电不正常的机组进入紧急通风模式，当三相电恢复后，所有可是机组进入减载模式			
	BOOLEAN1	CCU_HVAC_Car4Start_B1 :1;//	4	b2	4车压缩机允许启动信号	CCU_HVAC_Car4Start_B1	暂定 2S 脉冲	1=允许启动，0=不允许启动	0-1	空调只能在对应窗口内启动压缩机
	BOOLEAN1	CCU_HVAC_Car3Start_B1 :1;//	4	b3	3车压缩机允许启动信号	CCU_HVAC_Car3Start_B1	暂定 2S 脉冲	1=允许启动，0=不允许启动	0-1	空调只能在对应窗口内启动压缩机
	BOOLEAN1	CCU_HVAC_Car2Start_B1 :1;//	4	b4	2车压缩机允许启动信号	CCU_HVAC_Car2Start_B1	暂定 2S 脉冲	1=允许启动，0=不允许启动	0-1	空调只能在对应窗口内启动压缩机
	BOOLEAN1	CCU_HVAC_Car1Start_B1 :1;//	4	b5	1车压缩机允许启动信号	CCU_HVAC_Car1Start_B1	暂定 2S 脉冲	1=允许启动，0=不允许启动	0-1	空调只能在对应窗口内启动压缩机
	BOOLEAN1	HVACr_HalfLoad_B1 :1;//	4	b6						
	BOOLEAN1	CCU_WashMode_B1 :1;//	4	b7	洗车模式	CCU_WashMode_B1	电平	洗车=1，非洗车=0	0-1	"司机室洗车模式时关闭新风阀	客室洗车模式关闭废排风机，新风风机，新风PPV以及废排PPV"
	//BYTE5
	BOOLEAN1	CCU_HVAC_AllDoorClosed_B1 :1;//	5	b0	1	车门全关闭信号	BOOL		1=车门全关闭，0=车门未关闭	0-1	暂时没有收到技术输入
	BOOLEAN1	CCU_HVAC_CloseDoor_B1 :1;//	5	b1	1	关门信号	BOOL		1=有关门信号，0=无关门信号	0-1	暂时没有收到技术输入
	BOOLEAN1	CCU_HVAC_OpenDoor_B1 :1;//	5	b2	1	开门信号	BOOL		1=有开门信号，0=无开门信号	0-1	暂时没有收到技术输入
	BOOLEAN1	CCU_BYTE5_BIT3 :1;//	5	b3	1						
	BOOLEAN1	CCU_HVAC_Car4TempModeSet_B1 :1;//	5	b4	1	4车温度模式设置	BOOL	3s脉冲	温度设定=1，温度未设定=0	0-1	温度模式设置有效时，更改温度和模式
	BOOLEAN1	CCU_HVAC_Car2TempModeSet_B0 :1;//	5	b5	1	3车温度模式设置	BOOL	3s脉冲	温度设定=1，温度未设定=0	0-1	温度模式设置有效时，更改温度和模式
	BOOLEAN1	CCU_HVAC_Car2TempModeSet_B1 :1;//	5	b6	1	2车温度模式设置	BOOL	3s脉冲	温度设定=1，温度未设定=0	0-1	温度模式设置有效时，更改温度和模式
	BOOLEAN1	CCU_HVAC_Car1TempModeSet_B1 :1;//	5	b7	1	1车温度模式设置	BOOL	3s脉冲	温度设定=1，温度未设定=0	0-1	温度模式设置有效时，更改温度和模式
	//BYTE6
	UNSIGNED8	CCU_BYTE6;//

	UNSIGNED8	CCU_HVAC_AverIOutTemp_U8;//	7	8	整车平均室外温度	多个温度传感器求平均值，如果传感器全部故障，则发送255，当温度传感器测量值不在-20到100之间，判定为传感器故障	温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”	0-255	
	UNSIGNED8	CCU_HVAC_AverInnerTemp_U8;//	8	8	整车平均室内温度	多个温度传感器求平均值，如果传感器全部故障，则发送255，当温度传感器测量值不在-20到100之间，判定为传感器故障	温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”	0-255	
	UNSIGNED8	CCU_BYTE9;	//	9	8					
	UNSIGNED8	CCU_HVAC_Car2Temp_U8;//	10	8	2车温度设定值		温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”		温度模式设置有效时采信温度设定值
	UNSIGNED8	CCU_HVAC_Car3Temp_U8;//	11	8	3车温度设定值		温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”		
	UNSIGNED8	CCU_BYTE12;//	12	8					
	UNSIGNED8	CCU_HVAC_Car1Mode_U8;//	13	8	1车模式设定值		01:通风 02:制热 03:制冷 04:自动 05:停止		温度模式设置有效时采信模式设定值
	UNSIGNED8	CCU_HVAC_Car2Mode_U8;//	14	8	2车模式设定值		01:通风 02:制热 03:制冷 04:自动 05:停止		
	UNSIGNED8	CCU_HVAC_Car3Mode_U8;//	15	8	3车模式设定值		01:通风 02:制热 03:制冷 04:自动 05:停止		
	UNSIGNED8	CCU_HVAC_Car4Mode_U8;//	16	8	4车模式设定值		01:通风 02:制热 03:制冷 04:自动 05:停止		

	UNSIGNED8	res[83];//预留83字节，总长度100字节
};

typedef struct TRDP_HVAC2CCU
{
	UNSIGNED32	HVACXs_Lifesign_U32;//	0		8	X车HVAC生命信号HH	HVAC每个端口周期增加1	1=1	 0-4294967295			
	UNSIGNED8	HVACs_CNo_I_A_U8;//	4		8	车辆编号		1=1车，4=4车	0-4			
	UNSIGNED8	HVACCs_FAD_S_X_U8;//	5		8	X车司机室新风门开度		0=全关，50=开度50%，100=全开	0-100			
	UNSIGNED8	HVACCs_OpMo_S_X_U8;//	6		8	X车司机室工作模式	司机室紧急通风状态新风门打开 接收到洗车模式后，执行器动作执行结束后，反馈洗车信号	01:全暖 02:半暖 03:全冷 05:通风 06:自动 07:停止 08:火灾 09:紧急通风 10:洗车	1-10			
	UNSIGNED8	HVACCs_RAT_SS_X_U8;//	7		8	X车司机室室内温度	多个温度传感器求平均值，如果传感器全部故障，则发送255，当温度传感器测量值不在-20到100之间，判定为传感器故障	温度设定值0-240对应-20℃ ~ 100℃，40对应 0°，最小设定单位为“0.5℃”, 255表示温度无效	0-255			
	UNSIGNED8	HVACCs_FAT_SS_X_U8;//	8		8	X车司机室室外温度	多个温度传感器求平均值，如果传感器全部故障，则发送255，当温度传感器测量值不在-20到100之间，判定为传感器故障	温度设定值0-240对应-20℃ ~ 100℃，40对应 0°，最小设定单位为“0.5℃”, 255表示温度无效	0-255			
	UNSIGNED8	HVACCs_CO2_SS_X_U8;//	9		8	X车司机室CO2浓度		0=0%，50=50%，100=100%	0-100			
	UNSIGNED8	HVACCs_SetP_S_X_U8;//	10		8	X车司机室温度设定偏差	温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”					
	UNSIGNED8	HVAC_BYTE11;//	11		8							
	UNSIGNED8	HVAC_BYTE12;//	12		8							
	UNSIGNED8	HVAC_BYTE13;//	13		8							
	UNSIGNED8	HVAC_BYTE14;//	14		8							
	UNSIGNED8	HVAC_BYTE15;//	15		8							
	UNSIGNED8	HVAC_BYTE16;//	16		8							
	UNSIGNED8	HVACS1s_OpMo1_S_X_U8;//	17		8	X车客室机组1工作模式	接收到洗车模式后，执行器动作执行结束后，反馈洗车信号	01:全暖 02:半暖 03:全冷 04:半冷 05:通风 06:自动 07:停止 08:火灾 09:紧急通风 10:洗车	1-10			
	UNSIGNED8	HVACS2s_OpMo2_S_X_U8;//	18		8	X车客室机组2工作模式	接收到洗车模式后，执行器动作执行结束后，反馈洗车信号	01:全暖 02:半暖 03:全冷 04:半冷 05:通风 06:自动 07:停止 08:火灾 09:紧急通风 10:洗车	1-10			
	UNSIGNED8	HVACS1s_RAT_SS_X_U8;//	19		8	X车客室室内温度	"多个温度传感器求平均值，如果传感器故障，则发送255，当温度传感器测量值不在	-20到100之间，判定为传感器故障"	温度设定值0-240对应-20℃ ~ 100℃，40对应 0°，最小设定单位为“0.5℃”	0-255			
	UNSIGNED8	HVACS1s_FAT_SS_X_U8;//	20		8	X车客室室外温度	"多个温度传感器求平均值，如果传感器故障，则发送255，当温度传感器测量值不在	-20到100之间，判定为传感器故障"	温度设定值0-240对应-20℃ ~ 100℃，40对应 0°，最小设定单位为“0.5℃”	0-255			
	UNSIGNED8	HVACS1s_CO2_SS_X_U8;//	21		8	X车客室CO2浓度		0=0%，50=50%，100=100%	0-100			
	UNSIGNED8	HVACS1s_SetP_S_X_U8;//	22		8	X车客室温度设定值	温度设定值0-255对应-20℃ ~ 107.5℃，40对应 0°，最小设定单位为“0.5℃”					
	UNSIGNED8	HVAC_BYTE23;//	23		8							
	UNSIGNED8	HVAC_BYTE24;//	24		8							
	UNSIGNED8	HVAC_BYTE25;//	25		8							
	UNSIGNED8	HVAC_BYTE26;//	26		8							
	UNSIGNED8	HVAC_BYTE27;//	27		8							
	UNSIGNED8	HVAC_BYTE28;//	28		8							
	UNSIGNED8	HVAC_BYTE29;//	29		8							
	UNSIGNED8	HVAC_BYTE30;//	30		8							
	BOOLEAN1	HVACCs_SAF2_S_X_B :1;//	31	b0	1	X车司机室送风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_SAF1_S_X_B :1;//	31	b1	1	X车司机室送风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_ConD2_S_X_B :1;//	31	b2	1	X车司机室冷凝风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_ConD1_S_X_B :1;//	31	b3	1	X车司机室冷凝风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_Heat2_S_X_B :1;//	31	b4	1	X车司机室电加热2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_Heat1_S_X_B :1;//	31	b5	1	X车司机室电加热1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_ComP2_S_X_B :1;//	31	b6	1	X车司机室压缩机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_ComP1_S_X_B :1;//	31	b7	1	X车司机室压缩机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACCs_Wash_S_X_B :1;//	32	b0	1	X车司机室洗车模式状态激活	接收到洗车模式后，执行器动作执行结束后，反馈洗车信号，空调执行洗车模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACCs_SMK_S_X_B :1;//	32	b1	1	X车司机室火灾模式状态激活	空调执行火灾模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACCs_EMV_S_X_B :1;//	32	b2	1	X车司机室紧急通风状态激活	空调执行紧急通风模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACCs_Minor_E_X_B :1;//	32	b3	1	X车司机室轻微故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=轻微故障，0=未发现轻微故障	0-1			
	BOOLEAN1	HVACCs_Moder_E_X_B :1;//	32	b4	1	X车司机室中等故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=中等故障，0=未发现中等故障	0-1			
	BOOLEAN1	HVACCs_Major_E_X_B :1;//	32	b5	1	X车司机室重大故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=重大故障，0=未发现重大故障	0-1			
	BOOLEAN1	HVACCs_SelfT_R_X_B :1;//	32	b6	1	X车司机室机组自检结果		1=成功，0=未成功	0-1			
	BOOLEAN1	HVACCs_SelfT_F_X_B :1;//	32	b7	1	X车司机室机组自检结束		1=结束，0=未结束	0-1			
	BOOLEAN1	HVAC_BYTE33_BIT0 :1;//	33	b0	1				0-1			
	BOOLEAN1	HVAC_BYTE33_BIT1 :1;//	33	b1	1				0-1			
	BOOLEAN1	HVAC_BYTE33_BIT2 :1;//	33	b2	1				0-1			
	BOOLEAN1	HVAC_BYTE33_BIT3 :1;//	33	b3	1				0-1			
	BOOLEAN1	HVAC_BYTE33_BIT4 :1;//	33	b4	1				0-1			
	BOOLEAN1	HVAC_BYTE33_BIT5 :1;//	33	b5	1				0-1			
	BOOLEAN1	HVACCs_OpMo_S_X_B :1;//	33	b6	1	X车司机室模式控制方式	旋钮开关为硬线，于软件无关	1=网络，0=模式旋钮开关	0-1			
	BOOLEAN1	HVACCs_SetP_S_X_B :1;//	33	b7	1	X车司机室设点温度控制方式	网络和温度旋钮开关均为网络控制	1=网络，0=温度旋钮开关	0-1			
	UNSIGNED8	HVAC_BYTE34;//	34		8							
	UNSIGNED8	HVAC_BYTE35;//	35		8							
	BOOLEAN1	HVACS2s_ComP4_S_X_B :1;//	36	b0	1	X车客室机组2压缩机4状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ComP4_S_X_B :1;//	36	b1	1	X车客室机组1压缩机4状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_ComP3_S_X_B :1;//	36	b2	1	X车客室机组2压缩机3状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ComP3_S_X_B :1;//	36	b3	1	X车客室机组1压缩机3状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_ComP2_S_X_B :1;//	36	b4	1	X车客室机组2压缩机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ComP2_S_X_B :1;//	36	b5	1	X车客室机组1压缩机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_ComP1_S_X_B :1;//	36	b6	1	X车客室机组2压缩机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ComP1_S_X_B :1;//	36	b7	1	X车客室机组1压缩机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_ConD2_S_X_B :1;//	37	b0	1	X车客室机组2冷凝风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ConD2_S_X_B :1;//	37	b1	1	X车客室机组1冷凝风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_ConD1_S_X_B :1;//	37	b2	1	X车客室机组2冷凝风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_ConD1_S_X_B :1;//	37	b3	1	X车客室机组1冷凝风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_Heat2_S_X_B :1;//	37	b4	1	X车客室机组2电加热2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_Heat2_S_X_B :1;//	37	b5	1	X车客室机组1电加热2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_Heat1_S_X_B :1;//	37	b6	1	X车客室机组2电加热1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_Heat1_S_X_B :1;//	37	b7	1	X车客室机组1电加热1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_EAF_S_X_B :1;//	38	b0	1	X车客室机组2废排风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_EAF_S_X_B :1;//	38	b1	1	X车客室机组1废排风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_SanS_S_X_B :1;//	38	b2	1	X车客室机组2排沙风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_SanS_S_X_B :1;//	38	b3	1	X车客室机组1排沙风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_SAF2_S_X_B :1;//	38	b4	1	X车客室机组2送风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_SAF2_S_X_B :1;//	38	b5	1	X车客室机组1送风机2状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_SAF1_S_X_B :1;//	38	b6	1	X车客室机组2送风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_SAF1_S_X_B :1;//	38	b7	1	X车客室机组1送风机1状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_VCD_S_X_B :1;//	39	b0	1	X车客室机组2消毒杀菌装置		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_VCD_S_X_B :1;//	39	b1	1	X车客室机组1消毒杀菌装置		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_EPPV_S_X_B :1;//	39	b2	1	X车客室机组2废排PPV状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_EPPV_S_X_B :1;//	39	b3	1	X车客室机组1废排PPV状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_FPPV_S_X_B :1;//	39	b4	1	X车客室机组2新风PPV状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_FPPV_S_X_B :1;//	39	b5	1	X车客室机组1新风PPV状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_FAF_S_X_B :1;//	39	b6	1	X车客室机组2新风风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS1s_FAF_S_X_B :1;//	39	b7	1	X车客室机组1新风风机状态		1=启动，0=未启动	0-1			
	BOOLEAN1	HVACS2s_Moder_E_X_B :1;//	40	b0	1	X车客室机组2中等故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=中等故障，0=未发现中等故障	0-1			
	BOOLEAN1	HVACS1s_Moder_E_X_B :1;//	40	b1	1	X车客室机组1中等故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=中等故障，0=未发现中等故障	0-1			
	BOOLEAN1	HVACS2s_Major_E_X_B :1;//	40	b2	1	X车客室机组2重大故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=重大故障，0=未发现重大故障	0-1			
	BOOLEAN1	HVACS1s_Major_E_X_B :1;//	40	b3	1	X车客室机组1重大故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=重大故障，0=未发现重大故障	0-1			
	BOOLEAN1	HVACS2s_SelfT_R_X_B :1;//	40	b4	1	X车客室机组2自检结果		1=成功，0=未成功	0-1			
	BOOLEAN1	HVACS1s_SelfT_R_X_B :1;//	40	b5	1	X车客室机组1自检结果		1=成功，0=未成功	0-1			
	BOOLEAN1	HVACS2s_SelfT_F_X_B :1;//	40	b6	1	X车客室机组2自检结束		1=结束，0=未结束	0-1			
	BOOLEAN1	HVACS1s_SelfT_F_X_B :1;//	40	b7	1	X车客室机组1自检结束		1=结束，0=未结束	0-1			
	BOOLEAN1	HVACS2s_Wash_S_X_B :1;//	41	b0	1	X车客室机组2洗车模式状态	接收到洗车模式后，执行器动作执行结束后，反馈洗车信号，空调执行洗车模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS1s_Wash_S_X_B :1;//	41	b1	1	X车客室机组1洗车模式状态	接收到洗车模式后，执行器动作执行结束后，反馈洗车信号，空调执行洗车模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS2s_SMK_S_X_B :1;//	41	b2	1	X车客室机组2火灾模式状态	空调执行火灾模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS1s_SMK_S_X_B :1;//	41	b3	1	X车客室机组1火灾模式状态	空调执行火灾模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS2s_EMV_S_X_B :1;//	41	b4	1	X车客室机组2紧急通风状态	空调执行紧急通风模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS1s_EMV_S_X_B :1;//	41	b5	1	X车客室机组1紧急通风状态	空调执行紧急通风模式后反馈=1	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS2s_Minor_E_X_B :1;//	41	b6	1	X车客室机组2轻微故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=轻微故障，0=未发现轻微故障	0-1			
	BOOLEAN1	HVACS1s_Minor_E_X_B :1;//	41	b7	1	X车客室机组1轻微故障	由于空调系统有冗余设计，所以故障等级的评判是根据空调系统的主要部件不同的故障组合来确认的，详细请参考表格《故障等级判定表》	1=轻微故障，0=未发现轻微故障	0-1			
	BOOLEAN1	HVAC_BYTE42_BIT0 :1;//	42	b0	1				0-1			
	BOOLEAN1	HVAC_BYTE42_BIT1 :1;//	42	b1	1				0-1			
	BOOLEAN1	HVAC_BYTE42_BIT2 :1;//	42	b2	1				0-1			
	BOOLEAN1	HVAC_BYTE42_BIT3 :1;//	42	b3	1				0-1			
	BOOLEAN1	HVACSs_OpMo_S_X_B :1;//	42	b4	1	X车客室空调模式控制方式	旋钮开关为硬线，与软件无关	1=网络，0=模式旋钮开关	0-1			
	BOOLEAN1	HVACSs_SetP_S_X_B :1;//	42	b5	1	X车客室空调设点温度控制方式	网络和触摸屏均为网络控制	1=网络，0=触摸屏	0-1			
	BOOLEAN1	HVACS2s_Red_S_X_B :1;//	42	b6	1	X车客室机组2减载模式状态	当两台辅助逆变器正常工作时，空调正常工作。当两台辅助逆变器均不正常时，所有客室空调停机，当有一台工作不正常时，有三项电的两台机组进入减载模式，两台三相电不正常的机组进入紧急通风模式，当三相电恢复后，所有可是机组进入减载模式	1=激活，0=未激活	0-1			
	BOOLEAN1	HVACS1s_Red_S_X_B :1;//	42	b7	1	X车客室机组1减载模式状态	当两台辅助逆变器正常工作时，空调正常工作。当两台辅助逆变器均不正常时，所有客室空调停机，当有一台工作不正常时，有三项电的两台机组进入减载模式，两台三相电不正常的机组进入紧急通风模式，当三相电恢复后，所有可是机组进入减载模式	1=激活，0=未激活	0-1			
	UNSIGNED8	HVAC_BYTE43;//	43		8							
	UNSIGNED8	HVAC_BYTE44;//	44		8							
	UNSIGNED8	HVAC_BYTE45;//	45		8							
	UNSIGNED8	HVAC_BYTE46;//	46		8							
	UNSIGNED8	HVAC_BYTE47;//	47		8							
	BOOLEAN1	HVACCs_ConD2K8_E_X_B :1;//	48	b0	1	X车司机室冷凝风机2接触器K8故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_ConD2K7_E_X_B :1;//	48	b1	1	X车司机室冷凝风机2接触器K7故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_ConD1K6_E_X_B :1;//	48	b2	1	X车司机室冷凝风机1接触器K6故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_ConD1K5_E_X_B :1;//	48	b3	1	X车司机室冷凝风机1接触器K5故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_Heat2K4_E_X_B :1;//	48	b4	1	X车司机室电加热2接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_Heat1K3_E_X_B :1;//	48	b5	1	X车司机室电加热1接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_FrIn2_E_X_B :1;//	48	b6	1	X车司机室压缩机2变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_FrIn1_E_X_B :1;//	48	b7	1	X车司机室压缩机1变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_HPS1_E_X_B :1;//	49	b0	1	X车司机室高压传感器1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CPLP2_E_X_B :1;//	49	b1	1	X车司机室压缩机2低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CPLP1_E_X_B :1;//	49	b2	1	X车司机室压缩机1低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CPHP2_E_X_B :1;//	49	b3	1	X车司机室压缩机2高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CPHP1_E_X_B :1;//	49	b4	1	X车司机室压缩机1高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_3P_E_X_B :1;//	49	b5	1	X车司机室三相电故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_SAF2K10_E_X_B :1;//	49	b6	1	X车司机室送风机2接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_SAF1K9_E_X_B :1;//	49	b7	1	X车司机室送风机1接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_EEV1C_E_X_B :1;//	50	b0	1	X车司机室压缩机1电磁膨胀阀通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_FrIn2C_E_X_B :1;//	50	b1	1	X车司机室压缩机2变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_FrIn1C_E_X_B :1;//	50	b2	1	X车司机室压缩机1变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_RATS2_E_X_B :1;//	50	b3	1	X车司机室回风温度传感器2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_RATS1_E_X_B :1;//	50	b4	1	X车司机室回风温度传感器1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_FATS_E_X_B :1;//	50	b5	1	X车司机室新风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_SATS_E_X_B :1;//	50	b6	1	X车司机室送风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_HPS2_E_X_B :1;//	50	b7	1	X车司机室高压传感器2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVAC_BYTE51_BIT0 :1;//	51	b0	1				0-1			
	BOOLEAN1	HVACCs_EEV2_E_X_B :1;//	51	b1	1			X车司机室EEV2故障			
	BOOLEAN1	HVACCs_EEV1_E_X_B :1;//	51	b2	1			X车司机室EEV1故障				
	BOOLEAN1	HVACCs_CO2_E_X_B :1;//	51	b3	1			X车司机室CO2传感器故障				
	BOOLEAN1	HVACCs_CB2_E_X_B :1;//	51	b4	1	X车司机室压缩机2断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CB1_E_X_B :1;//	51	b5	1	X车司机室压缩机1断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_CC_E_X_B :1;//	51	b6	1	X车司机室控制器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACCs_EEV2C_E_X_B :1;//	51	b7	1	X车司机室压缩机2电磁膨胀阀通讯故障		1=故障，0=没有故障	0-1			
	UNSIGNED8	HVAC_BYTE52;//	52		8							
	UNSIGNED8	HVAC_BYTE53;//	53		8							
	BOOLEAN1	HVACS2s_FrIn4_E_X_B :1;//	54	b0	1	X车客室机组2压缩机4变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn4_E_X_B :1;//	54	b1	1	X车客室机组1压缩机4变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn3_E_X_B :1;//	54	b2	1	X车客室机组2压缩机3变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn3_E_X_B :1;//	54	b3	1	X车客室机组1压缩机3变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn2_E_X_B :1;//	54	b4	1	X车客室机组2压缩机2变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn2_E_X_B :1;//	54	b5	1	X车客室机组1压缩机2变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn1_E_X_B :1;//	54	b6	1	X车客室机组2压缩机1变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn1_E_X_B :1;//	54	b7	1	X车客室机组1压缩机1变频器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_ConD1K7_E_X_B :1;//	55	b0	1	X车客室机组2冷凝风机1接触器K7故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_ConD1K7_E_X_B :1;//	55	b1	1	X车客室机组1冷凝风机1接触器K7故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_ConD1K6_E_X_B :1;//	55	b2	1	X车客室机组2冷凝风机1接触器K6故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_ConD1K6_E_X_B :1;//	55	b3	1	X车客室机组1冷凝风机1接触器K6故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_Heat2K10_E_X_B :1;//	55	b4	1	X车客室机组2电加热2接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_Heat2K10_E_X_B :1;//	55	b5	1	X车客室机组1电加热2接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_Heat1K3_E_X_B :1;//	55	b6	1	X车客室机组2电加热1接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_Heat1K3_E_X_B :1;//	55	b7	1	X车客室机组1电加热1接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SAF1K5_E_X_B :1;//	56	b0	1	X车客室机组2送风机1接触器K5故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SAF1K5_E_X_B :1;//	56	b1	1	X车客室机组1送风机1接触器K5故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SAF1K4_E_X_B :1;//	56	b2	1	X车客室机组2送风机1接触器K4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SAF1K4_E_X_B :1;//	56	b3	1	X车客室机组1送风机1接触器K4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_ConD2K14_E_X_B :1;//	56	b4	1	X车客室机组2冷凝风机2接触器K14故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_ConD2K14_E_X_B :1;//	56	b5	1	X车客室机组1冷凝风机2接触器K14故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_ConD2K13_E_X_B :1;//	56	b6	1	X车客室机组2冷凝风机2接触器K13故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_ConD2K13_E_X_B :1;//	56	b7	1	X车客室机组1冷凝风机2接触器K13故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FAF_E_X_B :1;//	57	b0	1	X车客室机组2新风风机故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FAF_E_X_B :1;//	57	b1	1	X车客室机组1新风风机故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EAF_E_X_B :1;//	57	b2	1	X车客室机组2废排风机故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EAF_E_X_B :1;//	57	b3	1	X车客室机组1废排风机故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SAF2K12_E_X_B :1;//	57	b4	1	X车客室机组2送风机2接触器K12故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SAF2K12_E_X_B :1;//	57	b5	1	X车客室机组1送风机2接触器K12故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SAF2K11_E_X_B :1;//	57	b6	1	X车客室机组2送风机2接触器K11故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SAF2K11_E_X_B :1;//	57	b7	1	X车客室机组1送风机2接触器K11故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FPPV_E_X_B :1;//	58	b0	1	X车客室机组2新风PPV故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FPPV_E_X_B :1;//	58	b1	1	X车客室机组1新风PPV故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FAFK17_E_X_B :1;//	58	b2	1	X车客室机组2新风风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FAFK17_E_X_B :1;//	58	b3	1	X车客室机组1新风风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EAFK16_E_X_B :1;//	58	b4	1	X车客室机组2废排风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EAFK16_E_X_B :1;//	58	b5	1	X车客室机组1废排风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SanSK15_E_X_B :1;//	58	b6	1	X车客室机组2排沙风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SanSK15_E_X_B :1;//	58	b7	1	X车客室机组1排沙风机接触器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPHP2_E_X_B :1;//	59	b0	1	X车客室机组2压缩机2高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPHP2_E_X_B :1;//	59	b1	1	X车客室机组1压缩机2高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPHP1_E_X_B :1;//	59	b2	1	X车客室机组2压缩机1高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPHP1_E_X_B :1;//	59	b3	1	X车客室机组1压缩机1高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_3P_E_X_B :1;//	59	b4	1	X车客室机组2三相电故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_3P_E_X_B :1;//	59	b5	1	X车客室机组1三相电故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EPPV_E_X_B :1;//	59	b6	1	X车客室机组2废风PPV故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EPPV_E_X_B :1;//	59	b7	1	X车客室机组1废风PPV故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPLP2_E_X_B :1;//	60	b0	1	X车客室机组2压缩机2低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPLP2_E_X_B :1;//	60	b1	1	X车客室机组1压缩机2低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPLP1_E_X_B :1;//	60	b2	1	X车客室机组2压缩机1低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPLP1_E_X_B :1;//	60	b3	1	X车客室机组1压缩机1低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPHP4_E_X_B :1;//	60	b4	1	X车客室机组2压缩机4高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPHP4_E_X_B :1;//	60	b5	1	X车客室机组1压缩机4高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPHP3_E_X_B :1;//	60	b6	1	X车客室机组2压缩机3高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPHP3_E_X_B :1;//	60	b7	1	X车客室机组1压缩机3高压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_HPS2_E_X_B :1;//	61	b0	1	X车客室机组2空调高压传感器2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_HPS2_E_X_B :1;//	61	b1	1	X车客室机组1空调高压传感器2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_HPS1_E_X_B :1;//	61	b2	1	X车客室机组2空调高压传感器1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_HPS1_E_X_B :1;//	61	b3	1	X车客室机组1空调高压传感器1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPLP4_E_X_B :1;//	61	b4	1	X车客室机组2压缩机4低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPLP4_E_X_B :1;//	61	b5	1	X车客室机组1压缩机4低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_CPLP3_E_X_B :1;//	61	b6	1	X车客室机组2压缩机3低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CPLP3_E_X_B :1;//	61	b7	1	X车客室机组1压缩机3低压故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FATS_E_X_B :1;//	62	b0	1	X车客室机组2空调新风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FATS_E_X_B :1;//	62	b1	1	X车客室机组1空调新风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_SATS_E_X_B :1;//	62	b2	1	X车客室机组2空调送风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_SATS_E_X_B :1;//	62	b3	1	X车客室机组1空调送风温度传感器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_HPS4_E_X_B :1;//	62	b4	1	X车客室机组2空调高压传感器4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_HPS4_E_X_B :1;//	62	b5	1	X车客室机组1空调高压传感器4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_HPS3_E_X_B :1;//	62	b6	1	X车客室机组2空调高压传感器3故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_HPS3_E_X_B :1;//	62	b7	1	X车客室机组1空调高压传感器3故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn1C_E_X_B :1;//	63	b0	1	X车客室机组2压缩机1变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn1C_E_X_B :1;//	63	b1	1	X车客室机组1压缩机1变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_HMIC_E_X_B :1;//	63	b2	1	X车客室触摸屏通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_RATS5_E_X_B :1;//	63	b3	1	X车客室回风温度传感器5故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_RATS4_E_X_B :1;//	63	b4	1	X车客室回风温度传感器4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_RATS3_E_X_B :1;//	63	b5	1	X车客室回风温度传感器3故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_RATS2_E_X_B :1;//	63	b6	1	X车客室回风温度传感器2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_RATS1_E_X_B :1;//	63	b7	1	X车客室回风温度传感器1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV1C_E_X_B :1;//	64	b0	1	X车客室机组2电子膨胀阀1通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV1C_E_X_B :1;//	64	b1	1	X车客室机组1电子膨胀阀1通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn4C_E_X_B :1;//	64	b2	1	X车客室机组2压缩机4变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn4C_E_X_B :1;//	64	b3	1	X车客室机组1压缩机4变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn3C_E_X_B :1;//	64	b4	1	X车客室机组2压缩机3变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn3C_E_X_B :1;//	64	b5	1	X车客室机组1压缩机3变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_FrIn2C_E_X_B :1;//	64	b6	1	X车客室机组2压缩机2变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_FrIn2C_E_X_B :1;//	64	b7	1	X车客室机组1压缩机2变频器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CB1_E_X_B :1;//	65	b0	1	X车客室压机组1缩机1断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACSs_CC_E_X_B :1;//	65	b1	1	X车客室空调控制器通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV4C_E_X_B :1;//	65	b2	1	X车客室机组2电子膨胀阀4通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV4C_E_X_B :1;//	65	b3	1	X车客室机组1电子膨胀阀4通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV3C_E_X_B :1;//	65	b4	1	X车客室机组2电子膨胀阀3通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV3C_E_X_B :1;//	65	b5	1	X车客室机组1电子膨胀阀3通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV2C_E_X_B :1;//	65	b6	1	X车客室机组2电子膨胀阀2通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV2C_E_X_B :1;//	65	b7	1	X车客室机组1电子膨胀阀2通讯故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACSs_CO2_E_X_B :1;//	66	b0	1				0-1			
	BOOLEAN1	HVASC2s_CB4_E_X_B :1;//	66	b1	1	X车客室机组2压缩机4断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CB4_E_X_B :1;//	66	b2	1	X车客室机组1压缩机4断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVASC2s_CB3_E_X_B :1;//	66	b3	1	X车客室机组2压缩机3断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CB3_E_X_B :1;//	66	b4	1	X车客室机组1压缩机3断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVASC2s_CB2_E_X_B :1;//	66	b5	1	X车客室机组2压缩机2断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_CB2_E_X_B :1;//	66	b6	1	X车客室机组1压缩机2断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVASC2s_CB1_E_X_B :1;//	66	b7	1	X车客室机组2压缩机1断路器故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV4_E_X_B :1;//	67	b0	1	X车客室机组1EEV4故障		
	BOOLEAN1	HVACS1s_EEV4_E_X_B :1;//	67	b1	1	X车客室机组1EEV4故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV3_E_X_B :1;//	67	b2	1	X车客室机组2EEV3故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV3_E_X_B :1;//	67	b3	1	X车客室机组1EEV3故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV2_E_X_B :1;//	67	b4	1	X车客室机组2EEV2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV2_E_X_B :1;//	67	b5	1	X车客室机组1EEV2故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS2s_EEV1_E_X_B :1;//	67	b6	1	X车客室机组2EEV1故障		1=故障，0=没有故障	0-1			
	BOOLEAN1	HVACS1s_EEV1_E_X_B :1;//	67	b7	1	X车客室机组1EEV1故障	
						
	UNSIGNED8	HVAC_BYTE68;//	68		8							
	UNSIGNED8	HVAC_BYTE69;//	69		8							
	UNSIGNED8	HVAC_BYTE70;//	70		8							
	UNSIGNED8	HVACs_SWVM_I_A_U8;//	71		8   软件版本M  						
	UNSIGNED8	HVACs_SWVH_I_A_U8;//	72		8	软件版本H	H.L，以10进制显示	1=1	0-255			
	UNSIGNED8	HVACs_SWVL_I_A_U8;//	73		8	软件版本L	H.L，以10进制显示	1=1	0-255			

	UNSIGNED8	res[26];//预留26字节，总长度100字节
};




#pragma  pack (pop)  









#endif//__TRDP_DATA_H__


