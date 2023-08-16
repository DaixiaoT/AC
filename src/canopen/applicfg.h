

#ifndef __APPLICFG_AVR__
#define __APPLICFG_AVR__
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"

// Integers
#define INTEGER8 signed char
#define INTEGER16 short
#define INTEGER24
#define INTEGER32 long
#define INTEGER40
#define INTEGER48
#define INTEGER56
#define INTEGER64

// Unsigned integers
#define UNS8   unsigned char
#define UNS16  unsigned short
#define UNS32  unsigned long
/*
#define UNS24
#define UNS40
#define UNS48
#define UNS56
#define UNS64
*/ 


// Reals
#define REAL32	float
#define REAL64 double
#include "cp_can.h"
//#include "uart0.h"
//#include "ac_include.h"


// MSG functions
// not finished, the strings have to be placed to the flash and printed out 
// using the printf_P function
/// Definition of MSG_ERR
// ---------------------
//#define DEBUG_ERR_CONSOLE_ON
//#define DEBUG_WAR_CONSOLE_ON

#ifdef DEBUG_ERR_CONSOLE_ON



#define MSG_ERR(num, str, val)    LOG_PRINT("%s %s %s, \r\n", (num), (str),(val))
         
#else
#    define MSG_ERR(num, str, val)
#endif

/// Definition of MSG_WAR
// ---------------------
#ifdef DEBUG_WAR_CONSOLE_ON
#define MSG_WAR(num, str, val)  LOG_PRINT("%s %s %s,\r\n", (num), (str),(val))  
         
#else
#    define MSG_WAR(num, str, val)
#endif

typedef void* CAN_HANDLE;

typedef CAN_TypeDef* CAN_PORT;
#ifdef __cplusplus
}
#endif
#endif


