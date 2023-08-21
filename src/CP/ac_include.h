#ifndef __AC_INCLUDE_H__
#define __AC_INCLUDE_H__

#include "sys_api.h"

#define HI(x) ((U8)((x >> 8) & 0x00FF))
#define LO(x) ((U8)(x & 0x00FF))
#define EndianSwitch(word) (((word >> 8) & 0xff) | ((word & 0xff) << 8))






#endif//__AC_INCLUDE_H__
