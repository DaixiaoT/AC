#ifndef __M_AI_H__
#define __M_AI_H__
#include "ac_include.h"
#include "MCP4728.h"
#define DA_NUM      (2)
typedef U16 DA_TYPE;

/*
Digital Analog ������ ģ����
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
extern DA_CTRL ctrl_DA[DA_NUM];//�����ѹ���ȫ�ֱ���




// void AI_init();

// void AI_Updata();

void DA_Update(U16 Speed1, U16 Speed2);

void DA_init();

void ctrl_dump();












#endif