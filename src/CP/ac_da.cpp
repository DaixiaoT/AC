#include "ac_da.h"
DA_CTRL ctrl_DA[DA_NUM];

/*
电压输出
数字量转模拟量
*/
void DA_Update(U16 Speed1, U16 Speed2)
{
    static U16 s_DA1 = 0;
    static U16 s_DA2 = 0;
    ctrl_DA[0].set_value(Speed1); 
    ctrl_DA[1].set_value(Speed2);
    if ((s_DA1 != ctrl_DA[0].get_value()) || (s_DA2 != ctrl_DA[1].get_value()))
    { // 当电压输出发生更改时，更新电压输出的值
        s_DA1 = ctrl_DA[0].get_value();
        s_DA2 = ctrl_DA[1].get_value();
        MCP4728_DA_OUT_Sequential(s_DA1, s_DA2, 0, 0);
    }
}

void DA_init()
{
}

void ctrl_dump()
{
}

BOOL DA_CTRL::set_value(u16 da)
{
    BOOL flag = false;
    if ((da >= max_value) || (da < min_value)){
        return FALSE;
    }else{
        value = da;
    }
    return flag;
}

DA_TYPE DA_CTRL::get_value()
{
    return value;
}
