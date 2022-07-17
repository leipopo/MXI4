#include "main.h"
MotorInfo yaw, pit, whe[4], ste[4];
MotorInfo *can1motlist[8];
void chasmotinit()
{
    can1motlist[0]  = whe;
    yaw             = motparainit(gm6020);
    yaw.setup.motid = yawmotid;

    pit             = motparainit(gm6020);
    pit.setup.motid = pitmotid;

    for (int8_t i = 0; i < 4; i++)
    {
        whe[i]             = motparainit(m3508);
        whe[i].setup.motid = wheid + i;

        ste[i]             = motparainit(gm6020);
        ste[i].setup.motid = serid(steid, i);
    }
}

void devices_init()
{
    
}
