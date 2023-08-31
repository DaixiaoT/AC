#include "ptu_monitor.h"
#include "can_iap.h"
#include "ac_ctrl.h"
unsigned char s_DeviceStatusAutoRefresh = 0;//为1时，控制器->PTU送数据

int GetDeviceIOInfoPacket(U8 *buf, int len)
{
	
}


void maintenance_update()
{
	
}

void Reply_DeviceIdentifyInfoPacket(MAINTENANCE_BUS bus)
{
    U8 *buf = (U8 *)pMainTaincePacket;

	buf[0] = 0xf5;
	Set16(buf + 1, 19); // PUT_HVAC新的维护软件,包长度修改为两个字节了
	buf[3] = 0x11;

	Set32(buf + 4, IAP_VERSION);

	Set32(buf + 8, IAP_ID);

	buf[12] = 23;
	buf[13] = 8;
	buf[14] = 24;
	buf[15] = 15;
	buf[16] = 24;
	buf[17] = 25;

	buf[18] = GetParity(buf, 18);
	maintenance_send(bus, buf, 19);
}

void Reply_DeviceIOInfoPacket(MAINTENANCE_BUS bus)
{
    U8 *buf = (U8 *)pMainTaincePacket;
 
	int Tlen = GetDeviceIOInfoPacket(buf, 2048);

	if (Tlen > 0)
		maintenance_send(bus, buf, Tlen);
}

void StoreDeviceIOInfo(char *p, int len)
{   
	U8 *buf = (U8*)p;
    g_car.set1.FreshAirDamp.resistor_feedback=666;
    g_car.set1.Compressor_1.freq_HZ=555;
    
}



















