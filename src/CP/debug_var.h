#ifndef __DEBUG_VAR_H__
#define __DEBUG_VAR_H__


typedef union DEBUG_VAR
{
	U32 var;
	struct 
	{
		U32 ex_comm_ignore :1;
		U32 inv_fault_ignore :1;
		U32 feedback_ignore :1;//���ԽӴ������������
		U32 diagnose_ignore :1;//���ģʽ
		U32 Shutdown_ignore :1;//��ͣģʽ
		U32 heart_ignore : 1;//��������ģʽ
		U32 res :26;
	}bit;
};







extern DEBUG_VAR g_debug_var;






#endif//__DEBUG_VAR_H__



