/* ==========================================================================
 *
 *  File      : DG_ERROR.H
 *
 *  Purpose   : Error codes for Duagon host driver
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

#ifndef DG_ERROR_H
#define DG_ERROR_H




/* ==========================================================================
 *
 *  Include Files
 *
 * ==========================================================================
 */
#include "os_def.h"

/* ==========================================================================
 *
 *  Type definitions
 *
 * ==========================================================================
 */
typedef DG_S32 DG_RESULT;

/* ==========================================================================
 *
 *  Error codes for host interface errno
 *
 * ==========================================================================
 */
#define DG_OK                                   0  /* No error */
#define DG_ERROR                              400  /* General host driver
                                                      error */
#define DG_ERROR_TIMEOUT                      401  /* Timeout on read/write
                                                      acess */
#define DG_ERROR_COMM_FAIL                    402  /* Comm.failure */
#define DG_ERROR_SCAN_FAIL                    403  /* Scan PC/104 address bus failed */
#define DG_ERROR_SYSTEM_FAILURE               404  /* Internal error */
#define DG_ERROR_NOT_INITIALIZED              405  /* Missing initialization */
#define DG_ERROR_ALREADY_INITIALIZED          406  /* Initialization was already done */
#define DG_ERROR_NOT_IMPLEMENTED              407  /* Function not implemented */

#define DG_ERROR_NO_MEMORY                    420  /* Out of memory */

#define DG_ERROR_OSL_SYNC_INIT                500  /* Mutex initialisation
                                                      failed */
#define DG_ERROR_OSL_SYNC_LOCK                501  /* Mutex lock failed */
#define DG_ERROR_OSL_SYNC_UNLOCK              502  /* Mutex unlock failed */
#define DG_ERROR_OSL_SYNC_DESTROY             503  /* Mutex destroy failed */

#define DG_ERROR_OSL_INIT                     520  /* I/O access
                                                      initialisation
                                                      failed */
#define DG_ERROR_OSL_READ                     521  /* Read access failed */
#define DG_ERROR_OSL_WRITE                    522  /* Write access failed */

#define DG_ERROR_OSL_NOT_READY                523  /* device registers not ready */

#define DG_ERROR_HDIO_RECV_WOULD_BLOCK        541  /* Receiving task would
                                                      block */
#define DG_ERROR_HDIO_SEND_WOULD_BLOCK        542  /* Sending task would
                                                      block */
#define DG_ERROR_HDIO_UNKNOWN_CHANNEL_TYPE    544  /* Unknown channel type */
#define DG_ERROR_HDIO_UNKNOWN_IO_MODE         545  /* I/O mode unknown */
#define DG_ERROR_HDIO_NOT_SUPPORTED           546  /* Not supported for
                                                      configured I/O mode */

#define DG_ERROR_HDC_PROTOCOL_MISMATCH        560  /* Received wrong protocol
                                                      ID */
#define DG_ERROR_HDC_PROTOCOL                 561  /* Error in HDC protocol */
#define DG_ERROR_HDC_HANDLE                   562  /* Handle is wrong */
#define DG_ERROR_HDC_LENGTH_MISSMATCH         563  /* Payload size specified
                                                      in header not
                                                      available */
#define DG_ERROR_HDC_NOT_INIT                 564  /* HDC protocol not initialized */

#define DG_ERROR_RPC_UNKNOWN_TYPE             580  /* Unknown RPC type */
#define DG_ERROR_RPC_TOO_MANY_PAR             581  /* To many parameters */

#define DG_ERROR_VERSION_DRIVER_OLD           590  /* device requires a newer host */
#define DG_ERROR_VERSION_DEVICE_OLD           591  /* host needs a newer device */
#define DG_ERROR_EMPTY_BLOCK                  592  /* protocol not ready */
#define DG_ERROR_REGISTER_NOT_CORRECT         593  /* normally means the device has been reset */
#define DG_ERROR_FUNCTION_TIMED_OUT           594  /* time to execute function exceeds limit */
#define DG_ERROR_CHANNEL_TIMED_OUT            595  /* previous function executed on this channel exceeded timeout limit */
#define DG_ERROR_PARAM_OUT_OF_BOUND           596  /* value of a parameter is out of boundaries*/
#define DG_ERROR_PARAMETER_NULL               598  /* a parameter is NULL */
#define DG_ERROR_PARAMETER_STRING_NOT_TERM    599  /* a string parameter is not zero terminated in defined space or longer than expected. */
#define DG_ERROR_CARD_NOT_FOUND               600  /* card not defined in dg_config.h */
#define DG_ERROR_SOCK_NOT_AVAILABLE           610  /* socket is temporary not available */

#define DG_ERROR_SAFETY_NOT_INITIALIZED       800  /* Safety protocol is not initialized */
#define DG_ERROR_SAFETY_LIFE_SIGN_TIMED_OUT   801  /* Host Life Sign Watchdog timed out */

/* Error codes for return value */
#define DG_ENOERR                               0  /* No error */
#define DG_EGENERAL                            -1  /* General error */

DG_DECL_PUBLIC
DG_DECL_CONST DG_CHAR8* str_dg_errorName (DG_S32 errorCode);

DG_DECL_PUBLIC
DG_DECL_CONST DG_CHAR8* str_dg_errorDescription (DG_S32 errorCode);

#endif /* DG_ERROR_H */

