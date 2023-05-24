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

    while (RC_ON == 0)
    {
        RC_INIT();
        HAL_Delay(50);
    }

    can_filter_init();
    HAL_Delay(50);

    REF_INIT();
    HAL_Delay(50);
}

void userinit()
{
    devinit();
    HAL_Delay(50);
    chasmotinit();
    HAL_Delay(1500);
}
