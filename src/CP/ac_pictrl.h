#ifndef __AC_PICTRL_H__
#define __AC_PICTRL_H__


#include "sys_api.h"


struct strPI_Anteil_Ein
{
	S32		iAbtDiff;
	U8      cStopUp;
	U8      cStopDown;
	U8      cPI_Aktiv;
};

struct strPI_Anteil_Aus
{
	S32     iPI_Wert;
};

struct strPI_Anteil_Param
{
	S32     iP_Verst;
	S32     iI_Verst;
	S32     iTiefeMW;
	S32     iMaxAbw;
	S32     iDelay;
	S32     iI_Grenze;
	S32	    izDelay;
};

struct strPI_Anteil_Hilf
{
	S32     aiWerte[60];
	S32     iPos;
	S32     iI_AnteilSumme;
	S32     iI_Anteil;
	S32     iP_Anteil;
	S32     iAbtDiffBegr;
	S32     iWerteMittel;
};

struct PI_Anteil
{
	struct strPI_Anteil_Ein       Ein;
	struct strPI_Anteil_Aus       Aus;
	struct strPI_Anteil_Param     Param;
	struct strPI_Anteil_Hilf      Hilf;
};

// PI_Anteil PI_FGR;

//void PIAnteil(struct PI_Anteil* aPI_Anteil);
void LinInterpol(S32 si_WertX, S32* si_WertY, S32 si_lowX, S32 si_lowY, S32 si_highX, S32 si_highY);

//void PI_Controller_S(bool En_pid,U32 aiZ_Wait_PI_MODE_FGR,S32 iFGR_SollTemp,S32 iTem_R1_S,S32 iKanalsollwert_S, S32 iFre_Air_S_FAS_T);







class PI_Control {
public:
	PI_Control() {

		iDuctSollRoomsoll_S = 0;
		iKanalsollwert_S = 0;
		PI_time = 300000;
		PI_FGR.Ein.cPI_Aktiv = FALSE;
		PI_FGR.Param.iDelay = 5000;
		PI_FGR.Param.izDelay = 5000;
		PI_FGR.Param.iI_Verst = 5;
		PI_FGR.Param.iMaxAbw = 20;
		PI_FGR.Param.iP_Verst = 0;
		PI_FGR.Param.iTiefeMW = 48;
		PI_FGR.Param.iI_Grenze = 150;
	}
	PI_Anteil PI_FGR;
	S32 iTempSetpoint1_S; //常量
	S32 iDuctSollRoomsoll_S;
	S32 iKanalsollwert_S; //PI计算结果
	S32 PI_time;
	S32  temp_slope(S32 Tf); //设点温度曲线
	void PIAnteil(struct PI_Anteil* aPI_Anteil);
	void PI_Controller_S(bool En_pid, U32 aiZ_Wait_PI_MODE_FGR, S32 iFGR_SollTemp, S32 iTem_R1_S, S32 iKanalsollwert_S, S32 iFre_Air_S_FAS_T);
	void PI_Ctrl(BOOL PI_en, S32 Td, S32 Tr, S32 Tf, S32 Ts);
};














#endif//__AC_PI_H__