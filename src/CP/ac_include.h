#ifndef __AC_INCLUDE_H__
#define __AC_INCLUDE_H__
#include "sys_api.h"
#include "MB85RS256B.h"
#include "SST26VF064B.h"
#include "RTC.h"
#include "ac_timer.h"
#include "ADIO.h"
#include "ac_dio.h"
#include "ac_config.h"
#include "ac_trdp.h"

#define HI(x) ((U8)((x >> 8) & 0x00FF))
#define LO(x) ((U8)(x & 0x00FF))
#define EndianSwitch(word) (((word >> 8) & 0xff) | ((word & 0xff) << 8))
#define LOG_AC LOG_PRINT






#endif//__AC_INCLUDE_H__
