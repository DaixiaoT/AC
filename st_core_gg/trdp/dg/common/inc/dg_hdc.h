/* ==========================================================================
 *
 *  File      : DG_HDC.H
 *
 *  Purpose   : Host Device Communication Layer between RPC and HDIO
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
 
#ifndef DG_HDC_H
#define DG_HDC_H

#include "os_def.h"
#include "dg_rpc_g.h"
#include "dg_rpc.h"
#include "dg_rtm.h"
#include "dg_error.h"

#define HDC_PROTOCOL_ASKWAIT 0xff
#define HDC_PROTOCOL_ERROR   0xfe

/*! Error messages for the host driver in the command field. Must be in range [1, 255] */
#define ERROR_UNKNOWN_COMMAND         1
#define ERROR_RPC_TYPE_UNKNOWN        2
#define ERROR_INCONSISTENT_REQUEST    3
#define ERROR_TO_MANY_PARAMETERS      4
#define ERROR_BUFFER_TO_SMALL         5

#define MASK_HANDLE_REQUEST   0x80

/* --------------------------------------------------------------------------
 *  Protocol mapping
 * (Attention Channel 0 can only be used by 1 Protocol at the same time)
 * --------------------------------------------------------------------------
 */
#define DG_PFS_CHANNEL         1
#define DG_SVC_CHANNEL         1
#define DG_IPT_MD_CHANNEL      1
#define DG_WM_CHANNEL          1
#define DG_MD_TRDP_CHANNEL     1
#define DG_IPT_TDC_CHANNEL     1
#define DG_MARSHALLING_CHANNEL 1
#define DG_TRACE_PROTOCOL_CHANNEL 1
#define DG_FTP_CLIENT_PROTOCOL_CHANNEL 1
#define DG_SNMP_CLIENT_PROTOCOL_CHANNEL 1
#define DG_RSTP_PROTOCOL_CHANNEL 1

#if defined PROT_IPTCOM_PD || defined PROT_CIP || defined PROT_TRDP || defined PROT_PN || defined PROT_RDA_RT || defined PROT_MVB_MD_FULL
    #define DG_SOCKETS_CHANNEL    1     // real time protocol (IPTCom, CIP, or TRDP) on channel 0
#else
    #define DG_SOCKETS_CHANNEL    0     // no real time protocol, socket on channel 0
#endif
#define DG_IPT_PD_CHANNEL     0
#define DG_CAN_CHANNEL        0
#define DG_CIP_CHANNEL        0
#define DG_PD_TRDP_CHANNEL    0
#define DG_RDA_RT_CHANNEL     0
#define DG_PN_CHANNEL         0
#define DG_SAFETY_CHANNEL     0
#define DG_SDT_CHANNEL        0
/* --------------------------------------------------------------------------
 * Procedure :  dg_hdc_request
 *
 * Purpose   :  execute a request on the device
 *
 * Input:       protocol the protocol number for this call
 *              handle the handle for this call, or NULL for synchronous calls
 *              send send buffer
 *              recv receive buffer
 * Return:      0 for success or != 0 in case of error
 * Remark:
 * --------------------------------------------------------------------------
 */
RPC_DECL_PUBLIC DG_RESULT
dg_hdc_request(RPC_U8 protocol, RTM_HANDLE_P handle, RPC_PROTOCOL_DATA *send, RPC_PROTOCOL_DATA *recv);

/* --------------------------------------------------------------------------
 * Procedure :  hdc_init
 *
 * Purpose :    Initialise all communication channels
 *
 * Input :      card_index for card on which to initialise channels
 *
 * Return :     HDC_OK or any HDC_X error
 *
 * Remarks :
 * --------------------------------------------------------------------------
 */
DG_DECL_PUBLIC
DG_RESULT
hdc_init(DG_U8 card_index);

/* --------------------------------------------------------------------------
 * Procedure :  hdc_reset
 *
 * Purpose :    reset card after a board reset
 *
 * Input :      card_index for card on which to re-initialisation channels
 *
 * Return :     DG_OK or DG_ERROR_CARD_NOT_FOUND
 *
 * Remarks :    this causes a re-initialisation at next call to hdc_init() 
 *              which is usually called from any of the <protocol>_init() 
 *              functions
 * --------------------------------------------------------------------------
 */
DG_DECL_PUBLIC
DG_RESULT
hdc_reset(DG_U8 card_index);

/* --------------------------------------------------------------------------
 * Procedure :  dg_hdc_set_perf_callbacks
 *
 * Purpose :    allows to set callbacks at different stages of hdc layer for
 *              performance measurement
 *
 * Input :      start    Function to be called after channel get channel mutex
 *              send     Function to be called after send done
 *              recv_h   Function to be called after receive header done
 *              recv_p   Function to be called after receive payload done
 * Return :     --
 * Errors :     --
 *
 * Remarks :    local function
 * --------------------------------------------------------------------------
 */
void dg_hdc_set_perf_callbacks(DuagonBoundaryFunc start, DuagonBoundaryFunc send, DuagonBoundaryFunc recv_h, DuagonBoundaryFunc recv_p, DG_U8 card_index);

DG_DECL_PUBLIC
DG_U8
dg_get_channel_for_protocol(DG_U8 protocol);

#endif /* DG_HDC_H */
