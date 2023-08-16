/**
 * \file dg_trdp_types.h
 * \brief
 * \date 30.10.2014
 *
 * \details
 *
 * ==========================================================================
 *  Licence   : Duagon Software Licence (see file 'licence.txt')
 *
 * --------------------------------------------------------------------------
 *
 *  (C) COPYRIGHT, Duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 *
 * ==========================================================================
 */

#ifndef DG_TRDP_TYPES_H_
#define DG_TRDP_TYPES_H_

/**
 * \addtogroup trdp_host
 * @{
 */

/* ==================================================================================
 * Includes
 * ================================================================================*/
#include "os_def.h"

/* ==================================================================================
 * Definitions (typedef, constants, macros)
 * ================================================================================*/
#define DG_TRDP_MAX_URI_LEN 104
#define MAX_DATASET_ELEMENTS 16 // For statically allocating memory for dataset config

typedef DG_U32 DG_TRDP_IP_ADDR_T;

typedef DG_CHAR8 DG_TRDP_URI_T[DG_TRDP_MAX_URI_LEN];

typedef DG_U8 DG_TRDP_UUID_T[16];

/** Message Types    */
typedef enum
{
    DG_TRDP_MSG_PD = 0x5064,     /**< 'Pd' PD Data                                    */
    DG_TRDP_MSG_PP = 0x5070,     /**< 'Pp' PD Data (Pull Reply)                       */
    DG_TRDP_MSG_PR = 0x5072,     /**< 'Pr' PD Request                                 */
    DG_TRDP_MSG_PE = 0x5065,     /**< 'Pe' PD Error                                   */
    DG_TRDP_MSG_MN = 0x4D6E,     /**< 'Mn' MD Notification (Request without reply)    */
    DG_TRDP_MSG_MR = 0x4D72,     /**< 'Mr' MD Request with reply                      */
    DG_TRDP_MSG_MP = 0x4D70,     /**< 'Mp' MD Reply without confirmation              */
    DG_TRDP_MSG_MQ = 0x4D71,     /**< 'Mq' MD Reply with confirmation                 */
    DG_TRDP_MSG_MC = 0x4D63,     /**< 'Mc' MD Confirm                                 */
    DG_TRDP_MSG_ME = 0x4D65      /**< 'Me' MD Error                                   */
} DG_TRDP_MSG_T;

/** Various flags for PD and MD packets    */
typedef enum
{
    DG_TRDP_FLAGS_DEFAULT  = 0,        /**< Default value defined in tlc_openSession will be taken     */
    DG_TRDP_FLAGS_NONE     = 0x01,     /**< No flags set                                               */
    DG_TRDP_FLAGS_MARSHALL = 0x02,     /**< Optional marshalling/unmarshalling in TRDP stack           */
    DG_TRDP_FLAGS_CALLBACK = 0x04,     /**< Use of callback function                                   */
    DG_TRDP_FLAGS_TCP      = 0x08,     /**< Use TCP for message data                                   */
	DG_TRDP_FLAGS_FORCE_CB = 0x10      /**< Force a callback for every received packet                 */
} DG_TRDP_FLAGS_T;

/** Type attribute for telegrams.	*/
typedef enum
{
    DG_TRDP_EXCHG_UNSET        = 0,    /**< default, direction is not defined             */
    DG_TRDP_EXCHG_SOURCE       = 1,    /**< telegram shall be published                   */
    DG_TRDP_EXCHG_SINK         = 2,    /**< telegram shall be subscribed                  */
    DG_TRDP_EXCHG_SOURCESINK   = 3     /**< telegram shall be published and subscribed    */
} DG_TRDP_EXCHG_OPTION_T;

/** How invalid PD shall be handled    */
typedef enum
{
    DG_TRDP_TO_DEFAULT         = 0,    /**< Default value defined in tlc_openDession will be taken     */
    DG_TRDP_TO_SET_TO_ZERO     = 1,    /**< If set, data will be reset to zero on time out             */
    DG_TRDP_TO_KEEP_LAST_VALUE = 2     /**< If set, last received values will be returned              */
} DG_TRDP_TO_BEHAVIOR_T;

/** Return codes for all API functions */
typedef enum
{
    DG_TRDP_NO_ERR              = 0,    /**< No error                                                     */
    DG_TRDP_PARAM_ERR           = -1,   /**< Parameter missing or out of range                            */
    DG_TRDP_INIT_ERR            = -2,   /**< Call without valid initialization                            */
    DG_TRDP_NOINIT_ERR          = -3,   /**< Call with invalid handle                                     */
    DG_TRDP_TIMEOUT_ERR         = -4,   /**< Timeout                                                      */
    DG_TRDP_NODATA_ERR          = -5,   /**< Non blocking mode: no data received                          */
    DG_TRDP_SOCK_ERR            = -6,   /**< Socket error / option not supported                          */
    DG_TRDP_IO_ERR              = -7,   /**< Socket IO error, data can't be received/sent                 */
    DG_TRDP_MEM_ERR             = -8,   /**< No more memory available                                     */
    DG_TRDP_SEMA_ERR            = -9,   /**< Semaphore not available                                      */
    DG_TRDP_QUEUE_ERR           = -10,  /**< Queue empty                                                  */
    DG_TRDP_QUEUE_FULL_ERR      = -11,  /**< Queue full                                                   */
    DG_TRDP_MUTEX_ERR           = -12,  /**< Mutex not available                                          */
    DG_TRDP_THREAD_ERR          = -13,  /**< Thread error                                                 */
    DG_TRDP_BLOCK_ERR           = -14,  /**< System call would have blocked in blocking mode              */
    DG_TRDP_INTEGRATION_ERR     = -15,  /**< Alignment or endianess for selected target wrong             */
    DG_TRDP_NOCONN_ERR          = -16,  /**< No TCP connection                                            */
    DG_TRDP_NOSESSION_ERR       = -30,  /**< No such session                                              */
    DG_TRDP_SESSION_ABORT_ERR   = -31,  /**< Session aborted                                              */
    DG_TRDP_NOSUB_ERR           = -32,  /**< No subscriber                                                */
    DG_TRDP_NOPUB_ERR           = -33,  /**< No publisher                                                 */
    DG_TRDP_NOLIST_ERR          = -34,  /**< No listener                                                  */
    DG_TRDP_CRC_ERR             = -35,  /**< Wrong CRC                                                    */
    DG_TRDP_WIRE_ERR            = -36,  /**< Wire                                                         */
    DG_TRDP_TOPO_ERR            = -37,  /**< Invalid topo count                                           */
    DG_TRDP_COMID_ERR           = -38,  /**< Unknown ComId                                                */
    DG_TRDP_STATE_ERR           = -39,  /**< Call in wrong state                                          */
    DG_TRDP_APP_TIMEOUT_ERR     = -40,  /**< Application Timeout                                          */
    DG_TRDP_APP_REPLYTO_ERR     = -41,  /**< Application Reply Sent Timeout                               */
    DG_TRDP_APP_CONFIRMTO_ERR   = -42,  /**< Application Confirm Sent Timeout                             */
    DG_TRDP_REPLYTO_ERR         = -43,  /**< Protocol Reply Timeout                                       */
    DG_TRDP_CONFIRMTO_ERR       = -44,  /**< Protocol Confirm Timeout                                     */
    DG_TRDP_REQCONFIRMTO_ERR    = -45,  /**< Protocol Confirm Timeout (Request sender)                    */
    DG_TRDP_PACKET_ERR          = -46,  /**< Incomplete message data packet                               */
    DG_TRDP_UNRESOLVED_ERR      = -47,  /**< Unable to resolve hostname                                   */
    DG_TRDP_XML_PARSER_ERR      = -48,  /**< XML parser error                                             */
    DG_TRDP_INUSE_ERR           = -49,  /**< Resource is still in use                                     */
    DG_TRDP_MARSHALLING_ERR     = -50,  /**< Source size exceeded, dataset mismatch                       */
    DG_TRDP_DG_MARSH_ERR        = -51,  /**< Marshalling: General error                                   */
    DG_TRDP_DG_MARSH_PARAM      = -52,  /**< Marshalling: Parameter error, e.g. null pointer              */
    DG_TRDP_DG_MARSH_MEM        = -53,  /**< Marshalling: Memory error                                    */
    DG_TRDP_DG_MARSH_INIT       = -54,  /**< Marshalling: Initialization error / not initialized          */
    DG_TRDP_DG_MARSH_DSNOTFOUND = -55,  /**< Marshalling: Dataset not found                               */
    DG_TRDP_DG_MARSH_RECURSION  = -56,  /**< Marshalling: Recursion error -> max recursion level reached  */
    DG_TRDP_DG_MARSH_BUFFSIZE   = -57,  /**< Marshalling: Provided buffer size is too small               */
    DG_TRDP_DG_MARSH_CONFIG     = -58,  /**< Marshalling: Mal-configuration / or no configuration at all. */
    DG_TRDP_DG_MARSH_INVALID    = -59,  /**< Marshalling: Invalid function call at this point             */
    DG_TRDP_STACK_NOT_READY     = -80,  /**< Stack is not yet fully initialized                           */
    DG_TRDP_INVALID_FUNC        = -81,  /**< Invalid function call at this point                          */
    DG_TRDP_NO_DNS_SERVER       = -82,  /**< Name resolution requested but no DNS server available */
    DG_TRDP_NO_FRESH_DATA       = -83,  /**< Valid subscriber found, but no updated data since last read */
    DG_TRDP_UNKNOWN_ERR         = -99   /**< Unspecified error                                            */
} DG_TRDP_ERR_T;

/* ==================================================================================
 * Global variables declarations
 * ================================================================================*/


/* ==================================================================================
 * Global function prototypes
 * ================================================================================*/

/**
 * @}
 */


#endif /* define DG_TRDP_TYPES_H_ */
