#include "ac_trdp.h"

void TRDP_IN::Set_Mode(TRDP_MODE m)
{
	if (mode != m)
	{
		if ((m == 0) || (m > 7))
			return;
		// LOG_PRINT("TRDP--->mode=%d set---->mode=%d get_modeTemp()=%d\r\n",mode,m,get_modeTemp());
		if (Get_modeTemp())
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
void TRDP_IN::Set_OK(BOOL b)
{
	if (b)
		flag |= TRDP_OK_BIT;
	else
		flag &= (~TRDP_OK_BIT);

	//	LOG_PRINT("TRDP--->SetOK=%d,flg=%d\r\n",b,flag );
}

BOOL TRDP_IN::Get_modeTemp()
{
	if (flag_f & TRDP_MODE_TEMP_BIT_F)
		return flag_fv & TRDP_MODE_TEMP_BIT;
	else
		return flag & TRDP_MODE_TEMP_BIT;
}
// 送风温度
S16 TRDP_IN::Get_InTemp()
{
	if (!OK())
		return 255;
	return (flag_f & TRDP_TEMP_IN_F) ? INTemp_f : INTemp;
}
// 新风温度
S16 TRDP_IN::Get_OutTemp()
{
	if (!OK())
		return 255;
	return (flag_f & TRDP_TEMP_OUT_F) ? OUTTemp_f : OUTTemp;
}

void TRDP_IN::Set_OutTemp(U8 t) // 新风温度
{
	OUTTemp = t;
	return;
}

void TRDP_IN::Set_Compressor_Enable(BOOL b)
{
	if (b)
		flag |= TRDP_CP_EN_BIT;
	else
		flag &= (~TRDP_CP_EN_BIT);
}

BOOL TRDP_IN::Get_Compressor_Enable()
{
	if (!OK())
		return FALSE;
	if (flag_f & TRDP_CP_EN_BIT_F)
		return flag_fv & TRDP_CP_EN_BIT;
	else
		return flag & TRDP_CP_EN_BIT;
}

void TRDP_IN::Set_TempMode_Enable(BOOL b)
{
	if (b)
		flag |= TRDP_MODE_TEMP_BIT;
	else
		flag &= (~TRDP_MODE_TEMP_BIT);
}

TRDP_MODE TRDP_IN::Get_Mode()
{
	return (flag_f & TRDP_SET_MODE_F) ? mode_f : mode;
}
