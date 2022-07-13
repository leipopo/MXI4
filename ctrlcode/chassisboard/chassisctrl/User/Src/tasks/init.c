#include "main.h"

MotorInfo yaw, pit, whe[4],ste[4];

void chasmotinit()
{
    yaw=motparainit(gm6020);
    yaw.setup.motid=yawmotid;

    pit=motparainit(gm6020);
    pit.setup.motid=pitmotid;

    for(int8_t i=0;i<4;i++)
    {
        whe[i]=motparainit(m3508);
        whe[i].setup.motid=wheid+i; 
    }

    ste[0]=motparainit(gm6020);
    ste[0].setup.motid=steid;
    ste[1]=motparainit(gm6020);
    ste[1].setup.motid=steid+1;

    ste[2]=motparainit(gm6020);
    ste[2].setup.motid=steid;
    ste[3]=motparainit(gm6020);
    ste[3].setup.motid=steid+1;
}

void devices_init()
{
    
}
