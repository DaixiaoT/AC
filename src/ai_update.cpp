#include "sys_api.h"
#include "ADIO.h"
#include "ai_update.h"
//#include "ntc_10k.h"
#include "ntc_5k.h"
#include "filter.h"
#include "ads1113.h"

void ai_init()
{
}

// 根据r值，返回温度:
static int R2Temp(U32 r)
{
	
	return 0;
}

#define VOL(ad) ((float)((float)4096.0*(float)ad)/ (0x7fff))
#define VOL1(ad,base) ((float)((float)(base)*(float)ad)/ (0x7fff))


BOOL NTC5KCalcCallback(S16 ad, AI_TYPE * pro_v, AI_TYPE * valve)
{
	
	return TRUE;
}

BOOL DampCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	

	return TRUE;
}





BOOL CurrentCalcCallback(S16 ad, AI_TYPE * pro_v, AI_TYPE * valve)
{
	
	return TRUE;
}

BOOL VolCalcCallback(S16 ad, AI_TYPE * pro_v, AI_TYPE * valve)
{
	
	return TRUE;
}


#define HIGH_BA(a)((a - (float)4.0)/0.533) //
BOOL HPCalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{
	
	return TRUE;
}
#define CO2_PPM(a)((a - (float)4.0)/0.0016) //
BOOL CO2CalcCallback(S16 ad, AI_TYPE* pro_v, AI_TYPE* valve)
{


	return TRUE; 

}



