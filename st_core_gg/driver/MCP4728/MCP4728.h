#ifndef __MCP4728_H__
#define __MCP4728_H__

//ch:ͨ��λ  0��3 data:�����ѹ 0��4095
BOOL MCP4728_DA_OUT(U8 ch,U16 vol);



BOOL MCP4728ReadDAC();


//˳��д vol1~4:�����ѹ 0��4095  mv
BOOL MCP4728_DA_OUT_Sequential(U16 vol1, U16 vol2, U16 vol3, U16 vol4);


//����4·�����ѹ��׼��ѹ    0��ʹ���ⲿ��ѹ     1��ʹ���ڲ���ѹ ���ڲ���ѹ������4096mv��
BOOL MCP4728_Verf_Set(BOOL vol1, BOOL vol2, BOOL vol3, BOOL vol4);





#endif//__MCP4728_H__