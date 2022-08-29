#include "main.h"

//下面是一幅画
/****************************************************************

3         2
     /\
    //\\
     ||
     前
     ||
    //\\
   ======
4         1
****************************************************************/

void xyzspeed2wheelspeed(MotorInfo mi[4])
{
    float xytempspeed[2] = {0.f, 0.f};
    xytempspeed[0] = robinfo.tar.xspeed * cos(robinfo.cur.zrelangle / 360.f * 2 * 3.1415f) - robinfo.tar.yspeed * sin(robinfo.cur.zrelangle / 360.f * 2 * 3.1415f);
    xytempspeed[1] = robinfo.tar.xspeed * sin(robinfo.cur.zrelangle / 360.f * 2 * 3.1415f) + robinfo.tar.yspeed * sin(robinfo.cur.zrelangle / 360.f * 2 * 3.1415f);
    mi[0].tarmotorinfo.speed = -xytempspeed[0] - xytempspeed[1] - robinfo.tar.zspeed;
    mi[1].tarmotorinfo.speed = -xytempspeed[0] + xytempspeed[1] - robinfo.tar.zspeed;
    mi[2].tarmotorinfo.speed = xytempspeed[0] + xytempspeed[1] - robinfo.tar.zspeed;
    mi[3].tarmotorinfo.speed = xytempspeed[0] - xytempspeed[1] - robinfo.tar.zspeed;
}

void chasctrl()
{
    for (;;)
    {
        osDelayUntil(mottaskperi);
    }
}