#ifndef OS_DEF_H
#define OS_DEF_H




/* ==========================================================================
 *
 *  File      : OS_DEF.H
 *
 *  Purpose   : Host definitions for WIN32 hosts
 *
 *  Licence   : Duagon Software Licence (see file 'licence.txt')
 *
 * --------------------------------------------------------------------------
 *
 *  (C) COPYRIGHT, Duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 *
 * ==========================================================================
 */


/* ==========================================================================
 *
 *  General Constants and Types
 *
 * ==========================================================================
 */

/* Following defines may be used to configure the driver to your needs */
#define HAVE_MUTEX      1    /* Fast exclusion of mutex library and  functionality */
#define HAVE_PRINTF     0    /* Fast exclusion of printf libraries andfunctionality */
#define HAVE_TIMER      0    /* Fast exclusion of timer library and functionality */
#define HAVE_FILE       0    /* Fast exclusion of file library */


/* specify whether I/O is done with system call or with kernel module
 * IO_SYSCALL:       faster, kernel needs to be rebuild
 * IO_KERNEL_MODULE: slower, kernel module can be loaded during runtime
 */
#define IO_SYSCALL        1
#define IO_KERNEL_MODULE  0


/* ==========================================================================
 *
 *  Include Files
 *
 * ==========================================================================
 */
//#include <errno.h>     /* errno         */
//#include <string.h>    /* strlen        */

#if (1 == HAVE_TIMER)
#include <time.h>      /* time          */
#endif

#if (1 == HAVE_MUTEX)
//#include <pthread.h>   /* mutex         */
#endif

#if (1 == HAVE_FILE)
#include <stdio.h>     /* files         */
#endif

/* ==========================================================================
 *
 *  Data Types
 *
 * ==========================================================================
 */

/* --------------------------------------------------------------------------
 *  Data types with less than 8-bits
 * --------------------------------------------------------------------------
 */
typedef unsigned char       DG_BOOL;

/* --------------------------------------------------------------------------
 *  8-bit data types
 * --------------------------------------------------------------------------
 */
typedef unsigned char       DG_U8;
typedef signed char         DG_S8;
typedef char                DG_CHAR8;

/* --------------------------------------------------------------------------
 *  16-bit data types
 * --------------------------------------------------------------------------
 */
typedef unsigned short      DG_U16;
typedef short               DG_S16;

/* --------------------------------------------------------------------------
 *  32-bit data types
 * --------------------------------------------------------------------------
 */
typedef unsigned int        DG_U32;
typedef int                 DG_S32;

typedef float               DG_REAL32;

/* --------------------------------------------------------------------------
 *  64-bit data types
 * --------------------------------------------------------------------------
 */
typedef unsigned long long  DG_U64;
typedef long long           DG_S64;
typedef unsigned long long  UINT64;
typedef long long           INT64;

typedef double              DG_REAL64;

/* This type is used for protocol mutexes and can be accessed by the osl
 * functions.
 */
#if (1 == HAVE_MUTEX)
typedef sema_t    OSL_MUTEX;
#else
typedef DG_U32             OSL_MUTEX;
#endif

/* Please adapt to File handle type (only needed for certain drivers!) */
#if (1 == HAVE_FILE)
typedef FILE      OSL_FILE;
#else
typedef DG_S32    OSL_FILE;
#endif

//typedef DG_S32  clock_t;
/* Please adapt this Type to the System Timeout Struct */
typedef DG_S32  OSL_TIMER;


/* --------------------------------------------------------------------------
 *  REGISTER SIZE CONFIGURATION
 *
 *  This datatype is dependant of the architecture and the host configuration.
 *  It has to match with the STEPCFG value in the HCR register (interface ISA
 *  configuration). Possible values: DG_U8, DG_U16, DG_U32
 * --------------------------------------------------------------------------
 */
#ifdef USE_32BIT_ACCESS
typedef DG_U32                   REG_SIZE;
#else
#ifdef USE_16BIT_ACCESS
typedef DG_U16                   REG_SIZE;
#else
typedef DG_U8                    REG_SIZE;
#endif
#endif


/* ==========================================================================
 *
 *  Macros
 *
 * ==========================================================================
 */

/* --------------------------------------------------------------------------
 *  Macros for declaration of variables and procedures.
 *  NOTE:
 *  extern "C" is used in mixed C/C++ headers to force C linkage on an
 *  external definition.
 * --------------------------------------------------------------------------
 */
#define DG_DECL_VOLATILE   volatile

#define DG_DECL_LOCAL      static

#ifdef __cplusplus
#   define DG_DECL_PUBLIC  extern "C"
#else
#   define DG_DECL_PUBLIC  extern
#endif

#define DG_DECL_CONST      const


#if 0
#ifndef NULL
 #define NULL (void*)0
#endif

#ifndef TRUE
 #define TRUE (0==0)
#endif

#ifndef FALSE
 #define FALSE (!TRUE)
#endif
#endif

/* --------------------------------------------------------------------------
 *  Must be adapted to little and big endian (set '1' if big endian host)
 * --------------------------------------------------------------------------
 */
#define DG_HOST_SWAP 0

/* ----------------------------------------------------------------------- */
/* Please adapt to the system in use */
#if (1 == HAVE_PRINTF)
#define osl_printf (void)printf
#else
//DG_DECL_LOCAL void void_osl_printf(DG_DECL_CONST DG_CHAR8* nothing,...) { nothing = nothing; } /* dummy printf to avoid warnings */
#define osl_printf LOG_PRINT
#endif

#define DLL

#endif /* #ifndef OS_DEF_H*/
