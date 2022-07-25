#include "main.h"
MotorInfo whe[4];
void chasmotinit()
{
    init_gimbmot_para(&pit, &yaw);
    init_gimbmot_pid(&pitapid, &yawapid, &pitspid, &yawspid);
    for (int8_t i = 0; i < 4; i++)
    {
        whe[i] = motparainit(m3508);
        whe[i].setup.motid = wheid + i;
    }
}

void devinit()
{

    RC_INIT();
    can_filter_init();
}

void userinit()
{
    devinit();
    chasmotinit();
}
