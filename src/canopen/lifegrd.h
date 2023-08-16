
				 
#ifndef __lifegrd_h__
#define __lifegrd_h__
#ifdef __cplusplus
extern "C" {
#endif


#include <applicfg.h>

typedef void (*heartbeatError_t)(CO_Data*, UNS8);
void _heartbeatError(CO_Data* d, UNS8 heartbeatID);

typedef void (*post_SlaveBootup_t)(CO_Data*, UNS8);
void _post_SlaveBootup(CO_Data* d, UNS8 SlaveID);

typedef void (*post_SlaveStateChange_t)(CO_Data*, UNS8, e_nodeState);
void _post_SlaveStateChange(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState);

typedef void (*nodeguardError_t)(CO_Data*, UNS8);
void _nodeguardError(CO_Data* d, UNS8 id);

#include "data.h"

/*************************************************************************
 * Functions
 *************************************************************************/
/** 
 * @brief Start node guarding with respect to 0x100C and 0x100D
 * in the object dictionary
 * 
 * @param *d Pointer on a CAN object data structure
 * @ingroup nodeguardo
 */
void nodeguardInit(CO_Data* d);

/** 
 * @brief Stop producing node guarding messages
 *
 * @param *d Pointer on a CAN object data structure
 * @ingroup nodeguardo
 */
void nodeguardStop(CO_Data* d);

/** 
 * @brief Start the life guarding service (heartbeat/node guarding).
 * This service handles NMT error control messages either by using
 * heartbeats and/or by using node guarding messages (defined via the
 * object dictionary)
 *
 * @param *d Pointer on a CAN object data structure
 */
void lifeGuardInit(CO_Data* d);

/** 
 * @brief Stop the life guarding service (heartbeat/node guarding).
 *
 * @param *d Pointer on a CAN object data structure
 */
void lifeGuardStop(CO_Data* d);

/** 
 * @ingroup statemachine
 * @brief To read the state of a node
 * This can be used by the master after having sent a life guard request,
 * of by any node if it is waiting for heartbeat.
 * @param *d Pointer on a CAN object data structure
 * @param nodeId Id of a node
 * @return e_nodeState State of the node corresponding to the nodeId
 */
e_nodeState getNodeState (CO_Data* d, UNS8 nodeId);

/** 
 * @brief Start heartbeat consumer and producer
 * with respect to 0x1016 and 0x1017
 * object dictionary entries
 * @param *d Pointer on a CAN object data structure
 * @ingroup heartbeato
 */
void heartbeatInit(CO_Data* d);

/** 
 * @brief Stop heartbeat consumer and producer
 * @param *d Pointer on a CAN object data structure
 * @ingroup heartbeato
 */
void heartbeatStop(CO_Data* d);

/** 
 * @brief This function is responsible to process a canopen-message which seams to be an NMT Error Control
 * Messages.
 * If a BootUp message is detected, it will return the nodeId of the Slave who booted up
 * @param *d Pointer on a CAN object data structure 
 * @param *m Pointer on the CAN-message which has to be analysed.
 * @ingroup nodeguardo
 */
void proceedNODE_GUARD (CO_Data* d, Message* m);

#ifdef __cplusplus
}
#endif
#endif /*__lifegrd_h__ */
