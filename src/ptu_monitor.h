#ifndef _PTU_MONITOR_H_
#define _PTU_MONITOR_H_
#include "maintenance.h"


#define EndianSwitch(word) (((word >> 8) & 0xff) | ((word & 0xff) << 8))

extern unsigned char s_ucDeviceStatusAutoRefresh;












#endif