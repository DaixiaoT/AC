#ifndef __M_AI_H__
#define __M_AI_H__
#include "ac_include.h"
#include "MCP4728.h"
#define DA_NUM      (2)
typedef U16 DA_TYPE;

/*
Digital Analog 数字量 模拟量
*/
class DA_CTRL{
    public:
        DA_CTRL(){

        };
        BOOL set_DA(u16 da);
        DA_TYPE get_value(){
            return value;
        }



    private:
        DA_TYPE value;

};
extern DA_CTRL ctrl_DA[DA_NUM];//定义电压输出全局变量




// void AI_init();

// void AI_Updata();

void DA_Update(U16 Speed1, U16 Speed2);

void DA_init();

void ctrl_dump();












#endif