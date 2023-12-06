#ifndef __MAINTENANCE_H__
#define __MAINTENANCE_H__
#include "ac_include.h"
#include "ac_ai.h"
#include "trdp_udp.h"
#include "ac_ctrl.h"
enum MAINTENANCE_BUS
{
	MAINTENANCE_UART,
	MAINTENANCE_USB,
	MAINTENANCE_NET,
};

extern char *pMainTaincePacket;

unsigned char GetParity(unsigned char *p, int len);

int maintenance_send(MAINTENANCE_BUS bus, U8 *buf, int len);



void maintenance_uart_recv(int bus, U8 *buf, int len);//接收PTU数据





void Init_Can_sema();//信号量初始化

void Can_Pick_Data();//PPV数据CANOPEN

#endif