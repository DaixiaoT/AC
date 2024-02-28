#ifndef __MAINTENANCE_H__
#define __MAINTENANCE_H__
#include "ac_include.h"
enum MAINTENANCE_BUS
{
	MAINTENANCE_UART,
	MAINTENANCE_USB,
	MAINTENANCE_NET,
};

#include "ac_ai.h"
#include "trdp_udp.h"
#include "ac_ctrl.h"
#include "ptu_error_run_log.h"
#include "ptu_monitor.h"
#include "ptu_parameter.h"


extern char *pMainTaincePacket;

unsigned char GetParity(unsigned char *p, int len);

int maintenance_send(MAINTENANCE_BUS bus, U8 *buf, int len);



void maintenance_uart_recv(int bus, U8 *buf, int len);
//void maintenance_update();
//void maintenance_auto_update();
//接收PTU数据

static void SetBit(U8* ptr, int idx, int bit, U8 v)
{
	if (v)
	{
		ptr[idx] |= (1 << bit);
	}
	else
	{
		ptr[idx] &= ~(1 << bit);
	}
}


void maintenance_update();
void Init_Can_sema();//信号量初始化

void Can_Pick_Data();//PPV数据CANOPEN

#endif