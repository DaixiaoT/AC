/************************************************************************//**
 *  \file       dg_rtm.h
 *
 *  \brief      Function declarations and type definitions of
 *              Real time management.
 *
 *  \defgroup   RTM_HOST RTM Host Driver
 *  @{
 ****************************************************************************
 *  Licence : duagon Software Licence (see file 'licence.txt')
 ****************************************************************************
 *  (C) COPYRIGHT, duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 ***************************************************************************/

#ifndef DG_RTM_H
#define DG_RTM_H


/****************************************************************************
 *  Include Files
 ***************************************************************************/

#include "os_def.h"


/****************************************************************************
 *  Definitions (typedef, constants, macros)
 ***************************************************************************/

/** Handle information struct. */
typedef struct {
    DG_U8 handle;                   /*!< Real Time Handle number. */
    DG_BOOL rt_handle_defined;      /*!< Flag defining real time handle state.
                                          - false: Real time handle not defined  .
                                          - true:  Real time handle defined.*/
    DG_U8 card_index;               /*!< Card index.  */
    DG_BOOL card_handle_defined;    /*!< Flag defining card state.
                                          - false: Card not defined.
                                          - true:  Card defined.*/
    DG_U8 card_number;              /*!< Card number.*/
} DG_EXT_HANDLE;

/** Initialization of the handle information structure */
#define INIT_EXT_HANDLE {0,0,0,0,0}

/** Retrieve the card number given the handle. **/
#define CARD_GET_NUMBER(handle) (handle).card_number


/** Alias for DG_EXT_HANDLE **/
typedef DG_EXT_HANDLE DG_RTM_HANDLE;

/** Alias for DG_EXT_HANDLE **/
typedef DG_EXT_HANDLE RTM_HANDLE;

/** Pointer to handle structure (alias) **/
typedef RTM_HANDLE *RTM_HANDLE_P;

/** Pointer to handle structure */
typedef DG_EXT_HANDLE *EXT_HANDLE_P;


/****************************************************************************
 *  Public Procedure Interface
 ***************************************************************************/


/**
 * \brief    Initialize a card and real time handle
 *           (sets all members of handle to 0).
 *
 * @param[in]    handle    Handle struct.
 *
 */
DG_DECL_PUBLIC
void
init_ext_handle(
    EXT_HANDLE_P handle);

/**
 * \brief    Return flag defining whether the real time handle is working or not.
 *
 * @param[in]    handle    Handle struct.
 *
 * @retval    true    Is working.
 * @retval    false   Is not working.
 */
DG_DECL_PUBLIC
DG_BOOL
rtm_isWorking(
    RTM_HANDLE_P handle);

/**
 * \brief   Check if the function previously called with this real time handle has set the handle to indicate that it is finished (set to 0).
 *          This function does not communicate with the device. Each repeat call to the API function using rt_handle sends a request to the
 *          device to check if this handle is finished.
 *
 * @param[in]    handle    Handle struct.
 *
 * @retval    true    Finished (The return value and outputs of the function which used the real time handle are valid).
 * @retval    false   Still running (The return value and outputs of the function which used the real time handle are not yet valid).
 */
DG_DECL_PUBLIC
DG_BOOL
rtm_isFinished(
    RTM_HANDLE_P handle);

/**
 * \brief    Initialize a real time handle for use in asynchronouse calls
 *           (sets handle->rt_handle_defined flag and handle->handle to 255).
 *
 * @param[in]    handle    Handle struct.
 *
 */
DG_DECL_PUBLIC
void
rtm_init_handle(
    RTM_HANDLE_P handle);

/**
 * \brief    De-initialize a real time handle.
 *           (sets handle->rt_handle_defined flag and handle->handle to 0).
 *           Used in multicard environments to preserve the card handle for synchronous calls
 *           after use for asynchronous calls
 *
 * @param[out]    handle    Handle struct.
 *
 */
DG_DECL_PUBLIC
void
rtm_deinit_handle(
    RTM_HANDLE_P handle);

/**
 * \brief    Initialize card information in handle.
 *
 * @param[in]     card_number    Card number.
 * @param[out]    handle         Handle struct.
 *
 */
DG_DECL_PUBLIC
void
card_init_handle(
    DG_U8 card_number,
    EXT_HANDLE_P handle);


/* The following functions are for driver use only. They must never be use outside HDC! */

/**
 * \brief    Set real time handle to 0 indicating real time call is finished.
 * 
 * This function is for driver use only. Never use outside HDC!
 *
 * @param[in,out]    handle    Handle struct.
 *
 */
DG_DECL_PUBLIC
void
rtm_setFinished(
    RTM_HANDLE_P handle);

/**
 * \brief    Get real time handle index.
 * 
 * This function is for driver use only. Never use outside HDC!
 *
 * @param[in]    handle    Handle struct.
 *
 * @retval    handle    Real time handle index.
 * @retval    0         If handle struct is NULL or real time handle not defined.
 *
 */
DG_DECL_PUBLIC
DG_U8
rtm_get_handle(
    RTM_HANDLE_P handle);

/**
 * \brief    Set real time handle to command.
 * 
 * This function is for driver use only. Never use outside HDC!
 *
 * @param[out]    handle     Handle struct.
 * @param[in]     command    Command.
 *
 */
DG_DECL_PUBLIC
void
rtm_set_handle(
    RTM_HANDLE_P handle,
    DG_U8 command);

/**
 * \brief    Get card index.
 * 
 * This function is for driver use only. Never use outside HDC!
 *
 * @param[in]    handle    Handle struct.
 *
 * @retval    card_index    Card index.
 * @retval    0             If handle struct is NULL or card not defined.
 *
 */
DG_DECL_PUBLIC
DG_U8
get_card_index(
    EXT_HANDLE_P ext_handle);

#define DG_INIT_GLOBAL_MVB_CARD {NULL,0}

#endif /* DG_RTM_H */
