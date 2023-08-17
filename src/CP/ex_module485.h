#ifndef __EX_MODULE485_H__
#define __EX_MODULE485_H__
#include "ac_include.h"

#define VALVE_NUM (2)

typedef struct{
    U8 MB_ADDR;
    U8 FUNC;
    U16 REG_ADD;
    U16 NUM;
    U8 CRC16_H;
    U8 CRC16_L;
}ReadCmd_t;

typedef struct 
{
	U8 start_code;
	U8 func;
	U8 REG_ADD;
	U16 superheat;//HI(���ȶ�)-U1							
	U8 crc16_H;
	U8 crc16_L;							   
}Valve_TxDa_t;

typedef struct 
{
	U8 start_code;
	U8 func;
	U8  NUM;
	U16 MorkState;//����״̬							
	U16 ValveState;  //״̬λ	
	U16 res[7];//Ԥ��
	U16 valve_warn; //״̬λ	
	U16 superheat;	//��ǰ���ȶ�	
	U16 saturation;	//ǰ�����¶�	   
	U16 pressure;//��ǰѹ��
	U16 Temp; //��ǰ�¶�
	U16 res1[1];//Ԥ��
	U16 TValve_apetrture; //	���ͷ�����		
	U16 crc16;
}Valve_RxDa_t;



class Valve_module{

public:
    
    U32 time_out;

    BOOL valve1_timeout();

    BOOL valve2_timeout();


private:



};


extern Valve_module valve[VALVE_NUM];

typedef struct 
{	
	ReadCmd_t  readcmd_data;
	Valve_TxDa_t valvetx_data;
	Valve_RxDa_t valverx_data;	
}ValvModule_t;




#endif //__EX_MODULE485_H__
