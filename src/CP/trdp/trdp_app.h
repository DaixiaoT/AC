#ifndef _TRDP_APP_H__
#define _TRDP_APP_H__
#include "trdp_data.h"

typedef enum 
{
	TRDP_NULL=0,//还没有初始化信号量
	TRDP_RESET,
	TRDP_WAIT_15S,
	TRDP_INITING,
	TRDP_OK,
	TRDP_ERROR,
}TRDP_STAT;

TRDP_STAT trdp_get_stat();

void trdp_set_stat(TRDP_STAT s);
//car_num车厢号 1~4
void trdp_app_init(U8 car_num);


extern TRDP_CCU2HVAC g_ccutohavcdata;
extern TRDP_COMM_DATA g_ccutoalldata;
extern TRDP_HVAC2CCU g_havctoccudata;

typedef enum 
{
	CHANNEL_NULL=0,
	CHANNEL_A,
	CHANNEL_B,
	CHANNEL_AUTO,
}TRDP_TRUST_CHANNEL;

TRDP_TRUST_CHANNEL update_trdp_app_data(TRDP_TRUST_CHANNEL channel);
#endif//_TRDP_APP_H__









