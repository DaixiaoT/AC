#include "ac_trdp.h"

void TRDP_IN::Set_Mode(TRDP_MODE m)
{
	if (mode != m)
	{
		if ((m == 0) || (m > 7))
			return;
		// LOG_PRINT("TRDP--->mode=%d set---->mode=%d get_modeTemp()=%d\r\n",mode,m,get_modeTemp());
		if (get_modeTemp())
			mode = m;
	}
	return;
}
BOOL TRDP_IN::OK()
{
	if (flag_f & TRDP_OK_BIT_F)
		return flag_fv & TRDP_OK_BIT;
	else
		return flag & TRDP_OK_BIT;
}

BOOL TRDP_IN::get_modeTemp()
{
	if (flag_f & TRDP_MODE_TEMP_BIT_F)
		return flag_fv & TRDP_MODE_TEMP_BIT;
	else
		return flag & TRDP_MODE_TEMP_BIT;
}
// 送风温度
S16 TRDP_IN::get_INTEMP()
{
	if (!OK())
		return 255;
	return (flag_f & TRDP_TEMP_IN_F) ? INTemp_f : INTemp;
}
// 新风温度
S16 TRDP_IN::get_OUTTEMP()
{
	if (!OK())
		return 255;
	return (flag_f & TRDP_TEMP_OUT_F) ? OUTTemp_f : OUTTemp;
}

void TRDP_IN::Set_Compressor_Enable(BOOL b)
{
	if (b)
		flag |= TRDP_CP_EN_BIT;
	else
		flag &= (~TRDP_CP_EN_BIT);
}

void TRDP_IN::Set_TempMode_Enable(BOOL b)
{
	if (b)
		flag |= TRDP_MODE_TEMP_BIT;
	else
		flag &= (~TRDP_MODE_TEMP_BIT);
}

TRDP_MODE TRDP_IN::getMode()
{
	return (flag_f & TRDP_SET_MODE_F) ? mode_f : mode;
}
