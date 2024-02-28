#ifndef _PTU_MONITOR_H_
#define _PTU_MONITOR_H_
#include "maintenance.h"


#define EndianSwitch(word) (((word >> 8) & 0xff) | ((word & 0xff) << 8))

extern unsigned char s_DeviceStatusAutoRefresh;




void Reply_DeviceIdentifyInfoPacket(MAINTENANCE_BUS bus);
void Reply_DeviceIOInfoPacket(MAINTENANCE_BUS bus);

int GetDeviceIOInfoPacket(U8* buf, int len);

void StoreDeviceIOInfo(char *p, int len);






#endif


