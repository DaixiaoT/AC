#include "ptu_parameter.h"


void StoreDeviceParaInfo(char *p, int len)
{
	
}
void ReplyDeviceParaInfo(MAINTENANCE_BUS bus)//参数发送给PTU
{
	unsigned char* buf = (unsigned char*)pMainTaincePacket;
	int i;
	const int Tlen = 260;
	mem_set(buf, 0, Tlen);

	buf[0] = 0xf5;
	Set16(buf + 1, Tlen);
	buf[3] = 0x41;

	buf[Tlen - 1] = GetParity(buf, Tlen - 1);
	maintenance_send(bus, buf, Tlen);
}












