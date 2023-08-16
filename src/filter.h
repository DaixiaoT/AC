#ifndef _FILTER_H_
#define _FILTER_H_

//#define FILT_BUF_LEN	8 //16
//#define FILT_OBJ_NUM    20
#define FILT_BAD_VAL    0x7FFFFFFF

enum FILT_MODE {
    FILT_AVG = 0,
    FILT_MID = 1,
};

void filter_init();

void filter_reset(int h);

int get_filter_val(int h, int data, FILT_MODE avg);


#endif//_FILTER_H_
