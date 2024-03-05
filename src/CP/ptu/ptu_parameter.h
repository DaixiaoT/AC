#ifndef _PTU_PARAMETER_H
#define _PTU_PARAMETER_H
#include "maintenance.h"

void ClearErrLogRecord();
void StoreDeviceParaInfo(char* p, int len);

void ReplyDeviceParaInfo(MAINTENANCE_BUS bus);

#endif


