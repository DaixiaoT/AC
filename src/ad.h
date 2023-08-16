#ifndef __AD_H__
#define __AD_H__

#define curr(ad) ((float)((float)4096.0*(float)ad)/ (0x7fff)/12)//µ¥Î» 0.1mA
#define VOL(ad,VREF) ((float)((float)(VREF)*(float)ad)/ (0x7fff))


int get_ad_ntc(int ch, U16 *ad, U32 wait_ms);
void ntc_test();

int get_ad_current4_20ma(int ch, U16 *ad, U32 wait_ms);
void current4_20ma_test();

int get_ad_vol_0_5V(int ch, U16 *ad, U32 wait_ms);
void vol_0_5V_test();


int get_ad_ntc_vref(U16 *ad);
void ad_ntc_vref_test();


int get_ad_vout_vref(U16 *ad);
int ad_vout_vref_test();

int get_ad_5v_ios_vref(U16 *ad);
int ad_5v_ios_vref_test();



#endif//__AD_H__









