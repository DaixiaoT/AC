#ifndef _AC_API_H_
#define _AC_API_H_
#include "ac_include.h"
#include "ac_da.h"
static U32 s_trdp_time = 0;
static U8 s_car_no = 0;
static U32 s_now_time = 0;
// static U32 s_time = 0;
void ac_init();
void ac_control(U32 run_sec);
void ac_TRDP_to_Logic(U8 car);
void ac_CarNumberUpdate();




void ac_HVAC_to_TRDP(U8 car);

void Up_my_sensor();
void ac_ModbusDataSwitch();

#endif // _AC_API_H_
