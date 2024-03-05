#include "ptu_parameter.h"
#include "RTC.h"

void StoreDeviceParaInfo(char *p, int len)
{
	U8 *buf = (U8*)p;
	int i;
	if (GetBit8(buf[114], 0))
	{
		ClearErrLogRecord();
	}
}
void ReplyDeviceParaInfo(MAINTENANCE_BUS bus) // 参数发送给PTU
{
	unsigned char *buf = (unsigned char *)pMainTaincePacket;
	int i;
	const int Tlen = 260;
	mem_set(buf, 0, Tlen);

	buf[0] = 0xf5;
	Set16(buf + 1, Tlen);
	buf[3] = 0x41;
	buf[4] = g_parm.CarNo;
	RTC_GetTime((Time_t *)(buf + 5));

	i = 11;
	Set32(buf + i, g_runInfo.err_log_num); // 故障记录的总条数
	buf[Tlen - 1] = GetParity(buf, Tlen - 1);
	maintenance_send(bus, buf, Tlen);
}

void ClearErrLogRecord()
{
	g_runInfo.err_log_num = 0;
	g_runInfo.err_log_cur_addr = 0;
	StoreRunInfo(&g_runInfo);
}


