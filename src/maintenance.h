#ifndef __MAINTENANCE_H__
#define __MAINTENANCE_H__
#include "ac_include.h"



void maintenance_uart_recv(int bus, U8 *buf, int len);//接收PTU数据





void Init_Can_sema();//信号量初始化

void Can_Pick_Data();//PPV数据CANOPEN

#endif