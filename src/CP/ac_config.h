#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__



#pragma pack(push,1)
struct AC_PARM
{
	
};
#pragma pack(pop)

BOOL TOut_CPEn(); // ���¶��Ƿ����30��

BOOL IsCompressorStartEn();							// �Ƿ�����ѹ��������


#pragma pack(push,1)

// ���һ��д256�ֽ�
struct AC_ERR_Record
{			   // Ŀǰ����Ϊ��������
	
};


struct AC_RUN_Record
{			   // ����Ϊ��������
	
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
