#include "ADIO.h"
#define    IO_FLT_LENTH					((U16)10)
#define    IO_FLT_VALIDCOUNTS			((U16)10)

typedef struct DS_INPUTF_
{
	U8    gpioVal;						// 端口原始值
	U8    inVal;						// 滤波后，端口值
	U16   ioHighLevels;					// 高电平次数
	U16   ioLowLevels;					// 低电平次数
	U16   FlLenth;						// 滤波区长度
	U16   validCounts;					// 有效电平次数
}DS_INPUTF, * P_DS_INPUTF;

static  DS_INPUTF  sysInputF[DI_NUM] = { 0 };


static void di_filer_init()
{
	int i;
	for (i = 0; i < DI_NUM; i++)
	{
		sysInputF[i].gpioVal = 0;
		sysInputF[i].inVal = 0;
		sysInputF[i].ioHighLevels = 0;
		sysInputF[i].ioLowLevels = 0;
		sysInputF[i].FlLenth = IO_FLT_LENTH;
		sysInputF[i].validCounts = IO_FLT_VALIDCOUNTS;
	}
}

#define get_bit(buf,n) GetBit8(buf[(n)>>3],(n)&7)

static void di_filer_update(U8* input)
{
	int i;
	for (i = 0; i < DI_NUM; i++)
	{
		sysInputF[i].gpioVal = get_bit(input, i);
		if (sysInputF[i].gpioVal)
		{
			if (sysInputF[i].ioHighLevels < sysInputF[i].FlLenth) sysInputF[i].ioHighLevels++;
			if (sysInputF[i].ioLowLevels > 0) sysInputF[i].ioLowLevels--;
		}
		else
		{
			if (sysInputF[i].ioLowLevels < sysInputF[i].FlLenth) sysInputF[i].ioLowLevels++;
			if (sysInputF[i].ioHighLevels > 0) sysInputF[i].ioHighLevels--;
		}
		//----------------------------------------------------------------------------------
		// 更新输入状态
		//----------------------------------------------------------------------------------
		if (sysInputF[i].ioHighLevels >= sysInputF[i].validCounts) sysInputF[i].inVal = 1;
		else if (sysInputF[i].ioLowLevels >= sysInputF[i].validCounts) sysInputF[i].inVal = 0;

		g_DI.Set(i, sysInputF[i].inVal);

		// 		if (sysInputF[i].gpioVal)
		// 		{
		// 			LOG_PRINT("%d,%d,%d,%d,%d\r\n",
		// 				i,
		// 				sysInputF[i].inVal,
		// 				sysInputF[i].validCounts,
		// 				sysInputF[i].ioHighLevels,
		// 				DI_STAT(i)
		// 				);
		// 		}
	}
}
































