#include"ac_pictrl.h"
#include"ac_config.h"

void PI_Control::PIAnteil(struct PI_Anteil* aPI_Anteil)
{
	U8 i;
	aPI_Anteil->Hilf.iAbtDiffBegr = aPI_Anteil->Ein.iAbtDiff;
	//LOG_PRINT("aPI_Anteil->Hilf.iAbtDiffBegr1 =%d\r\n",aPI_Anteil->Hilf.iAbtDiffBegr );
	if (aPI_Anteil->Hilf.iAbtDiffBegr > aPI_Anteil->Param.iMaxAbw)      aPI_Anteil->Hilf.iAbtDiffBegr = aPI_Anteil->Param.iMaxAbw;
	if (aPI_Anteil->Hilf.iAbtDiffBegr < (-aPI_Anteil->Param.iMaxAbw))   aPI_Anteil->Hilf.iAbtDiffBegr = (-aPI_Anteil->Param.iMaxAbw);
	// LOG_PRINT("aPI_Anteil->Hilf.iAbtDiffBegr2 =%d\r\n",aPI_Anteil->Hilf.iAbtDiffBegr );


	if (aPI_Anteil->Ein.cPI_Aktiv) {
		if (!(aPI_Anteil->Param.izDelay)) {
			//LOG_PRINT("aPI_Anteil->Param.izDelay=%d\r\n",aPI_Anteil->Param.izDelay);
			aPI_Anteil->Param.izDelay = (U32)aPI_Anteil->Param.iDelay;
			if (aPI_Anteil->Hilf.iPos >= (aPI_Anteil->Param.iTiefeMW))     aPI_Anteil->Hilf.iPos = 0;
			aPI_Anteil->Hilf.aiWerte[aPI_Anteil->Hilf.iPos] = 0;
			if ((aPI_Anteil->Ein.cStopUp && (aPI_Anteil->Hilf.iAbtDiffBegr > 0)) ||
				(aPI_Anteil->Ein.cStopDown && (aPI_Anteil->Hilf.iAbtDiffBegr < 0))) {
				aPI_Anteil->Hilf.aiWerte[aPI_Anteil->Hilf.iPos] = 0;
			}
			else {
				aPI_Anteil->Hilf.aiWerte[aPI_Anteil->Hilf.iPos] = aPI_Anteil->Hilf.iAbtDiffBegr;
			}
			aPI_Anteil->Hilf.iPos++;
			if (aPI_Anteil->Hilf.iPos >= (aPI_Anteil->Param.iTiefeMW))     aPI_Anteil->Hilf.iPos = 0;
			aPI_Anteil->Hilf.iWerteMittel = 0;
			for (i = 0; i < aPI_Anteil->Param.iTiefeMW; i++) {
				aPI_Anteil->Hilf.iWerteMittel += aPI_Anteil->Hilf.aiWerte[i];
			}
			aPI_Anteil->Hilf.iWerteMittel /= aPI_Anteil->Param.iTiefeMW;
			if (!((aPI_Anteil->Ein.cStopUp && (aPI_Anteil->Hilf.iAbtDiffBegr > 0)) ||
				(aPI_Anteil->Ein.cStopDown && (aPI_Anteil->Hilf.iAbtDiffBegr < 0))))
			{
				aPI_Anteil->Hilf.iI_AnteilSumme += aPI_Anteil->Hilf.iWerteMittel;
			}
			if (aPI_Anteil->Hilf.iI_AnteilSumme > (aPI_Anteil->Param.iI_Grenze * 100 / aPI_Anteil->Param.iI_Verst))
				aPI_Anteil->Hilf.iI_AnteilSumme = (aPI_Anteil->Param.iI_Grenze * 100 / aPI_Anteil->Param.iI_Verst);
			if (aPI_Anteil->Hilf.iI_AnteilSumme < ((-aPI_Anteil->Param.iI_Grenze) * 100 / aPI_Anteil->Param.iI_Verst))
				aPI_Anteil->Hilf.iI_AnteilSumme = ((-aPI_Anteil->Param.iI_Grenze) * 100 / aPI_Anteil->Param.iI_Verst);
			aPI_Anteil->Hilf.iP_Anteil = aPI_Anteil->Hilf.iAbtDiffBegr * aPI_Anteil->Param.iP_Verst / 100;
			aPI_Anteil->Hilf.iI_Anteil = aPI_Anteil->Hilf.iI_AnteilSumme * aPI_Anteil->Param.iI_Verst / 100;
			aPI_Anteil->Aus.iPI_Wert = aPI_Anteil->Hilf.iP_Anteil + aPI_Anteil->Hilf.iI_Anteil;
		}
	}
	else
	{
		aPI_Anteil->Aus.iPI_Wert = 0;
		aPI_Anteil->Hilf.iPos = 0;
		aPI_Anteil->Hilf.iI_Anteil = 0;
		aPI_Anteil->Hilf.iI_AnteilSumme = 0;
		aPI_Anteil->Hilf.iP_Anteil = 0;
		aPI_Anteil->Hilf.iWerteMittel = 0;
		aPI_Anteil->Aus.iPI_Wert = 0;
		for (i = 0; i < aPI_Anteil->Param.iTiefeMW; i++)
		{
			aPI_Anteil->Hilf.aiWerte[i] = aPI_Anteil->Hilf.iAbtDiffBegr;
		}
	}
}

void PI_Control::PI_Controller_S(bool En_pid, U32 aiZ_Wait_PI_MODE_FGR, S32 iFGR_SollTemp, S32 iTem_R1_S, S32 iKanalsollwert_S, S32 iFre_Air_S_FAS_T)
{
	if (En_pid) {
		PI_FGR.Ein.cPI_Aktiv = TRUE;
	}
	else {
		aiZ_Wait_PI_MODE_FGR = 300000;  //300秒
		PI_FGR.Ein.cPI_Aktiv = FALSE;
	}
	if (aiZ_Wait_PI_MODE_FGR)
		PI_FGR.Param.iP_Verst = gPi_iP_Verst_TimeLimit;
	else
		PI_FGR.Param.iP_Verst = gPi_iP_Verst_TimeOut;

	//LOG_PRINT("PI_FGR.Param.iP_Verst=%d\r\n",PI_FGR.Param.iP_Verst);
	U32 T_MIN_KANAL_FGR = iFGR_SollTemp - g_MIN_KANAL_Parm;
	U32 T_MAX_KANAL_FGR = iFGR_SollTemp + g_MAX_KANAL_Parm;
	//LOG_PRINT("T_MIN_KANAL_FGR=%d\r\n",T_MIN_KANAL_FGR);
	//LOG_PRINT("T_MAX_KANAL_FGR=%d\r\n",T_MAX_KANAL_FGR);
	//LOG_PRINT("g_MIN_KANAL_Parm=%d\r\n",g_MIN_KANAL_Parm);
	//LOG_PRINT("g_MAX_KANAL_Parm=%d\r\n",g_MAX_KANAL_Parm);
	PI_FGR.Ein.cStopDown = (aiZ_Wait_PI_MODE_FGR || (iKanalsollwert_S <= (T_MIN_KANAL_FGR + 2)) || ((iKanalsollwert_S - iFre_Air_S_FAS_T) < -10));
	PI_FGR.Ein.cStopUp = (aiZ_Wait_PI_MODE_FGR || (iKanalsollwert_S >= (T_MAX_KANAL_FGR + 2)) || ((iKanalsollwert_S - iFre_Air_S_FAS_T) > 10));
	// LOG_PRINT("PI_FGR.Ein.cStopDown=%d\r\n",PI_FGR.Ein.cStopDown);
	// LOG_PRINT("PI_FGR.Ein.cStopUp=%d\r\n",PI_FGR.Ein.cStopUp);
	if ((iFGR_SollTemp - iTem_R1_S) > 20)
		PI_FGR.Ein.iAbtDiff = 20;
	else {
		if ((iFGR_SollTemp - iTem_R1_S) < -20)
			PI_FGR.Ein.iAbtDiff = -20;
		else
			PI_FGR.Ein.iAbtDiff = (iFGR_SollTemp - iTem_R1_S);
	}
	PIAnteil(&PI_FGR);
	PI_FGR.Param.izDelay = PI_FGR.Param.izDelay - 500;
	if (PI_FGR.Param.izDelay < 0)
		PI_FGR.Param.izDelay = 5000;

}

void LinInterpol(S32 si_WertX,
	S32* si_WertY,
	S32 si_lowX,
	S32 si_lowY,
	S32 si_highX,
	S32 si_highY)
{
	*si_WertY = ((si_lowY - si_highY) * (si_WertX - si_lowX) / (si_lowX - si_highX)) + si_lowY;
}
S32 PI_Control::temp_slope(S32 Tf) {  //设点温度曲线
	S32 Td = 0;
	if (Tf <= 175) {
		Td = 220;
	}
	else if (Tf >= 210) {
		Td = 230;
	}
	else {
		Td = 220 + ((2 * 100 / 7) * (Tf - 175)) / 100;
	}
	return Td;
}
void PI_Control::PI_Ctrl(BOOL PI_en, S32 Td, S32 Tr, S32 Tf, S32 Ts) {
	S32 iDuctSollBase_S = 0;
	S32 Passenger_corr = 0;

	PI_Controller_S(PI_en, PI_time, Td, Tr, iKanalsollwert_S, Tf);

	Passenger_corr = g_Passenger_No * 85 / 100;  //Passenger_No //Passenger_No 乘客数量 //0
	LinInterpol(Tf, &iDuctSollBase_S, g_si_lowX, g_si_lowY, g_si_highX, g_si_highY); //后面的参数 PTU 可调
	iDuctSollRoomsoll_S = ((Td - (g_TempSetpoint - 20)) * 3 / 2);  //权重

	iKanalsollwert_S = iDuctSollBase_S + iDuctSollRoomsoll_S + PI_FGR.Aus.iPI_Wert + Passenger_corr; //iKanalsollwert_S
	PI_time = PI_time - 500;
	if (PI_time <= 0) {
		PI_time = 0;
	}
	/*LOG_PRINT("PI_time=%d\r\n", PI_time);
	LOG_PRINT("iKanalsollwert_S2=%d\r\n", iKanalsollwert_S);
	LOG_PRINT("SAT=%d\r\n", Ts - iKanalsollwert_S);
	LOG_PRINT("g_si_lowX=%d,g_si_lowY=%d,g_si_highX=%d, g_si_highY=%d,\r\n", g_si_lowX, g_si_lowY, g_si_highX, g_si_highY);
	LOG_PRINT("iDuctSollRoomsoll_S=%d\r\n", iDuctSollRoomsoll_S);
	LOG_PRINT("iDuctSollBase_S=%d\r\n", iDuctSollBase_S);
	LOG_PRINT("g_Passenger_No=%d,Passenger_corr=%d\r\n", g_Passenger_No, Passenger_corr);
	LOG_PRINT("PI_FGR.Param.izDelay=%d\r\n", PI_FGR.Param.izDelay);*/

}
