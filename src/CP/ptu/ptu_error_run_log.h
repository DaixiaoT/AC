#ifndef _PTU_ERROR_RUN_LOG_H_
#define _PTU_ERROR_RUN_LOG_H_
#include "maintenance.h"

extern U32 g_down_log_tick;


BOOL ac_get_record(AC_ERR_Record *f);







void Reply_ErrLogPacket(MAINTENANCE_BUS bus, U8* p, int idx);

BOOL StoreRunInfo(AC_RUN_INFO* p);

int ac_run_log_record(AC_RUN_Record* f);

void save_run_log();

void Reply_RunLogPacket(MAINTENANCE_BUS bus, U8* p, int idx);

void ac_err_log_record(AC_ERR_Record* f);









#endif