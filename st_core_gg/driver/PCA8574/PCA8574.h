#ifndef __PCA8574_H__
#define __PCA8574_H__


BOOL PCA8574_Write(U8 io);

BOOL PCA8574_write_led_flicker(U8 io,int led_bit=3);//默认LED灯接到PCA8574 OUT3上





#endif//__PCA8574_H__



