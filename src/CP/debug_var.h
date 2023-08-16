#ifndef __DEBUG_VAR_H__
#define __DEBUG_VAR_H__


typedef union DEBUG_VAR
{
	U32 var;
	struct 
	{
		U32 ex_comm_ignore :1;
		U32 inv_fault_ignore :1;
		U32 feedback_ignore :1;//忽略接触器反馈点故障
		U32 diagnose_ignore :1;//诊断模式
		U32 Shutdown_ignore :1;//急停模式
		U32 heart_ignore : 1;//忽略心跳模式
		U32 res :26;
	}bit;
};







extern DEBUG_VAR g_debug_var;






#endif//__DEBUG_VAR_H__



