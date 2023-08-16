#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__



#pragma pack(push,1)
struct AC_PARM
{
	
};
#pragma pack(pop)

BOOL TOut_CPEn(); // 外温度是否大于30度

BOOL IsCompressorStartEn();							// 是否允许压缩机启动


#pragma pack(push,1)

// 最大一次写256字节
struct AC_ERR_Record
{			   // 目前定义为故障数据
	
};


struct AC_RUN_Record
{			   // 定义为运行数据
	
};

#pragma pack(pop)

// #define AC_RUN_INFO_MARK 0x92fc6e51

#pragma pack(push, 1)
struct SET_TIMER
{
	
};

struct OTHER_TIMER
{
	
};

struct KM_NUM
{
	
};

struct AC_RUN_INFO
{
	
};
#pragma pack(pop)



#endif // !__AC_CONFIG_H__
