#ifndef __VALVE_CTRL_H__
#define __VALVE_CTRL_H__

typedef enum {
	CS_INIT,
	CS_RUN,
	CS_STOP,
}eCrtlStat;


typedef enum {
	CD_OPEN,
	CD_CLOSE
}eCrtlDir;

#pragma  pack (push,1)
typedef struct {
	U8 stat[2];
	U8 dir[2];
	U16 curr_step[2];
	U16 dest_step[2];
}DEV_STAT_DATA_2;
#pragma  pack (pop)



void valve_get_stat_data(DEV_STAT_DATA_2* stat);

void valve_ctrl(U8 *buf,int len);
U16 valve_get_positon(int n);

void valve_test();

void set_valve_pos(U16 pos1, U16 pos2);
void value_action(U8 ch, U16 step);
#endif//__VALVE_CTRL_H__







