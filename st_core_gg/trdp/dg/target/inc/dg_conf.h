/* ==========================================================================
 *
 *  File      : DG_CONF_H
 *
 *  Purpose   : General Host Driver Configurations 
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

#ifndef DG_CONF_H
#define DG_CONF_H

#include "os_def.h"
#include "dg_hdcfg.h"

/* ==========================================================================
 *
 *  General Constants and Types
 *
 * ==========================================================================
 */

//#define PROT_PFS            1
#define PROT_SOCKETS        1
//#define PROT_CO             1    
// #define PROT_IPTCOM_PD      1
// #define PROT_IPTCOM_MD      1
#define PROT_TRDP           1
//#define PROT_MARSH          1

/* --------------------------------------------------------------------------
 * Enable debug printout
 * --------------------------------------------------------------------------
 */
#define IPT_PD_DEBUG    0    /* Print process data debug messages */
#define IPT_MD_DEBUG    0    /* Print message data debug messages */
#define IP_DEBUG        0    /* Print sockets debug messages */
#define CO_DEBUG        0    /* Print canopen debug messages */
#define PFS_DEBUG       0    /* Print postfix file system debug messages */
#define OSL_DEBUG       0    /* Print low level debug messages */
#define DB_REG_ACCESS   0    /* Print each Access to the registers */
#define HDIO_DEBUG      0    /* Print low level IO debug messages */
#define HDC_DEBUG       0    /* Print HDC debug mssessages */
#define RPC_DEBUG       0    /* Print RPC debug messages */
#define PN_DEBUG        0    /* Print Profinet debug messages */
#define WM_DEBUG        0

/***************************************************************************/
/***************************************************************************/
/* MUST BE DEFINED BY CUSTOMER (HAS TO MATCH WITH INTERFACE CONFIGURATION) */
/***************************************************************************/
/***************************************************************************/

#define IPTCOM_VERSION 3700

#define DG_CARD_0_MVB_CHANNEL 0
#define DG_CARD_1_MVB_CHANNEL 0
#define DG_CARD_2_MVB_CHANNEL 0
#define DG_CARD_3_MVB_CHANNEL 0

#define DG_CARD_0_MVB_SERVER_CHANNEL 2
#define DG_CARD_1_MVB_SERVER_CHANNEL 2
#define DG_CARD_2_MVB_SERVER_CHANNEL 2
#define DG_CARD_3_MVB_SERVER_CHANNEL 2


/* --------------------------------------------------------------------------
 *  IO MODE FOR ALL AVAILABLE CHANNELS
 *   Please select:
 *   DG_HDCFG_PARALLEL
 *   DG_HDCFG_SERIAL
 * --------------------------------------------------------------------------
 */
#define DG_IO_MODE DG_HDCFG_PARALLEL

/* --------------------------------------------------------------------------
 * Timeouts for Read/Write [milliseconds]
 * --------------------------------------------------------------------------
 */
#ifndef DG_CHANNEL_0_READ_TIMEOUT
#define DG_CHANNEL_0_READ_TIMEOUT 30000
#endif

#ifndef DG_CHANNEL_1_READ_TIMEOUT
#define DG_CHANNEL_1_READ_TIMEOUT 30000
#endif
#define DG_CHANNEL_2_READ_TIMEOUT 30000

#define DG_CHANNEL_0_WRITE_TIMEOUT 10000
#define DG_CHANNEL_1_WRITE_TIMEOUT 10000
#define DG_CHANNEL_2_WRITE_TIMEOUT 10000

/* --------------------------------------------------------------------------
 * Timeouts for function completion [microseconds]
 * --------------------------------------------------------------------------
 */
#define DG_CHANNEL_0_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CHANNEL_1_FN_TO_DEFAULT (500*1000)//30000000
#define DG_CHANNEL_2_FN_TO_DEFAULT (500*1000)//30000000

#define DG_CARD_0_CHANNEL_0_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_1_CHANNEL_0_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_2_CHANNEL_0_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_3_CHANNEL_0_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_0_CHANNEL_1_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_1_CHANNEL_1_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_2_CHANNEL_1_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_3_CHANNEL_1_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_0_CHANNEL_2_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_1_CHANNEL_2_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_2_CHANNEL_2_FN_TO_DEFAULT (500*1000)//500ms//30000000
#define DG_CARD_3_CHANNEL_2_FN_TO_DEFAULT (500*1000)//500ms//30000000


/* --------------------------------------------------------------------------
 *  CARD configuration
 * --------------------------------------------------------------------------
 */
 

#define CARD_MVB        0
#define CARD_SERVER2    1
#define CARD_MVB_SERVER 2
#define CARD_SOCKETS    3
#define CARD_IPTCOM     4
#define CARD_CO         5
#define CARD_NONE       6
#define CARD_RT_ETH_MVB 7


/* CARD_0 */
#define CARD_0_TYPE   CARD_SOCKETS
#define CARD_0_HOST_BASE_CHANNEL_0 0x60000000
#define CARD_0_HOST_BASE_CHANNEL_1 0x60000008
#define CARD_0_HOST_BASE_CHANNEL_2 0

/* CARD_1 */
#define CARD_1_TYPE   CARD_SOCKETS
#define CARD_1_HOST_BASE_CHANNEL_0 0
#define CARD_1_HOST_BASE_CHANNEL_1 0
#define CARD_1_HOST_BASE_CHANNEL_2 0

/* CARD_2 */
#define CARD_2_TYPE   CARD_NONE

/* CARD_3 */
#define CARD_3_TYPE   CARD_NONE



#endif /* #ifndef DG_CONF_H */
