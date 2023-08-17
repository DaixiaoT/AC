#ifndef __AI_UDPATE_H__
#define __AI_UDPATE_H__

#include "ac_include.h"

typedef float AI_TYPE;

class AI_CTRL
{
public:

    AI_TYPE getValue(){
        return value;
    }
    




private:

    AI_TYPE value;

};

void AI_init();

void AI_Updata();


U16 ReadFreshAirDampFeedback();
U16 ReadHighPressureValue_1();

U16 ReadHighPressureValue_2();

void ai_init(void);

#endif //__AI_UDPATE_H__
