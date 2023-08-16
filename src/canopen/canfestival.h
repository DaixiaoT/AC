


#ifndef __CAN_CANFESTIVAL__
#define __CAN_CANFESTIVAL__
#ifdef __cplusplus
extern "C" {
#endif

#include "applicfg.h"
#include "data.h"




// ---------  to be called by user app ---------
typedef struct {
		UNS8 CANx;
		CanRxMsg m;
}CANOpen_Message;


typedef struct _struct_co_data_2_can {
	CO_Data* CO_CAN1;
	CO_Data* CO_CAN2;
} CO_DATA_STRUCT;

extern CO_DATA_STRUCT  CO_D;
void canopen_Init(void);
void canopen_check(int num ,CANOpen_Message *CAN);
void initTimer(void);
UNS8 canSend(CAN_PORT notused, Message *m);
UNS8 canChangeBaudRate(CAN_PORT port, char* baud);
void heartbeat_error(CO_Data* d, UNS8 heartbeatID);
//UNS32 index1016_callback(CO_Data* d, const indextable* index, UNS8 bSubindex);
void Slave_up(CO_Data* d, UNS8 SlaveID);
extern CANOpen_Message CAN1_Rx_m;

#ifdef __cplusplus
}
#endif
#endif
