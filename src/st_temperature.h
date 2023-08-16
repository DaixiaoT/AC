#ifndef __ST_TEMPERATURE_H__
#define __ST_TEMPERATURE_H__

void Adc_Init2(void);

short Get_Temprate(void);

short Get_VBat(void);

short Get_Vref(void);

BOOL ADC_hw_test();
short ADC_STM_TEMP();

void adc_test();

void Adc_Disable(void);

#endif//__ST_TEMPERATURE_H__



