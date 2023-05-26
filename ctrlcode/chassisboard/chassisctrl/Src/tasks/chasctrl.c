#include "main.h"
PID_regulator whespid[4];
// 下面是一幅画
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
    float sumspeed[4] = {0.f, 0.f, 0.f, 0.f};
    //float czrangle=numcircle(180.f,-180.f,robinfo.cur.zrelangle+yaw.curmotorinfo.speed*0.3f) ;
    float czrangle=robinfo.cur.zrelangle;
    
    xytempspeed[0] = robinfo.tar.xspeed * cos(czrangle / 360.f * 2 * 3.1415f) + robinfo.tar.yspeed * sin(czrangle / 360.f * 2 * 3.1415f);
    xytempspeed[1] = -robinfo.tar.xspeed * sin(czrangle / 360.f * 2 * 3.1415f) + robinfo.tar.yspeed * cos(czrangle / 360.f * 2 * 3.1415f);

    sumspeed[0] = -xytempspeed[0] - xytempspeed[1] + robinfo.tar.zspeed;
    sumspeed[1] = -xytempspeed[0] + xytempspeed[1] + robinfo.tar.zspeed;
    sumspeed[2] = xytempspeed[0] + xytempspeed[1] + robinfo.tar.zspeed;
    sumspeed[3] = xytempspeed[0] - xytempspeed[1] + robinfo.tar.zspeed;

    if (fabsf(sumspeed[0]) > 400.f)
    {
        mi[0].tarmotorinfo.speed = -xytempspeed[0] / fabs(sumspeed[0]) * 400.f - xytempspeed[1] / fabs(sumspeed[0]) * 400.f + robinfo.tar.zspeed / fabs(sumspeed[0]) * 400.f;
    }
    else
    {
        mi[0].tarmotorinfo.speed = sumspeed[0];
    }
    if (fabsf(sumspeed[1]) > 400.f)
    {
        mi[1].tarmotorinfo.speed = -xytempspeed[0] / fabs(sumspeed[1]) * 400.f + xytempspeed[1] / fabs(sumspeed[1]) * 400.f + robinfo.tar.zspeed / fabs(sumspeed[1]) * 400.f;
    }
    else
    {
        mi[1].tarmotorinfo.speed = sumspeed[1];
    }
    if (fabsf(sumspeed[2]) > 400.f)
    {
        mi[2].tarmotorinfo.speed = xytempspeed[0] / fabs(sumspeed[2]) * 400.f + xytempspeed[1] / fabs(sumspeed[2]) * 400.f + robinfo.tar.zspeed / fabs(sumspeed[2]) * 400.f;
    }
    else
    {
        mi[2].tarmotorinfo.speed = sumspeed[2];
    }
    if (fabsf(sumspeed[3]) > 400.f)
    {
         mi[3].tarmotorinfo.speed = xytempspeed[0] / fabs(sumspeed[3]) * 400.f - xytempspeed[1] / fabs(sumspeed[3]) * 400.f + robinfo.tar.zspeed / fabs(sumspeed[3]) * 400.f;
    }
    else
    {
        mi[3].tarmotorinfo.speed = sumspeed[3];
    }

    // if (sumspeed[0] - mi[0].tarmotorinfo.speed > movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[0].tarmotorinfo.speed += movespeed / fre(mottaskperi) / acctime;
    // }
    // else if (sumspeed[0] - mi[0].tarmotorinfo.speed < -movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[0].tarmotorinfo.speed -= movespeed / fre(mottaskperi) / acctime;
    // }
    // else
    // {
    //     mi[0].tarmotorinfo.speed = sumspeed[0];
    // }

    // if (sumspeed[1] - mi[1].tarmotorinfo.speed > movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[1].tarmotorinfo.speed += movespeed / fre(mottaskperi) / acctime;
    // }
    // else if (sumspeed[1] - mi[1].tarmotorinfo.speed < -movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[1].tarmotorinfo.speed -= movespeed / fre(mottaskperi) / acctime;
    // }
    // else
    // {
    //     mi[1].tarmotorinfo.speed = sumspeed[1];
    // }

    // if (sumspeed[2] - mi[2].tarmotorinfo.speed > movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[2].tarmotorinfo.speed += movespeed / fre(mottaskperi) / acctime;
    // }
    // else if (sumspeed[2] - mi[2].tarmotorinfo.speed < -movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[2].tarmotorinfo.speed -= movespeed / fre(mottaskperi) / acctime;
    // }
    // else
    // {
    //     mi[2].tarmotorinfo.speed = sumspeed[2];
    // }

    // if (sumspeed[3] - mi[3].tarmotorinfo.speed > movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[3].tarmotorinfo.speed += movespeed / fre(mottaskperi) / acctime;
    // }
    // else if (sumspeed[3] - mi[3].tarmotorinfo.speed < -movespeed / fre(mottaskperi) / acctime)
    // {
    //     mi[3].tarmotorinfo.speed -= movespeed / fre(mottaskperi) / acctime;
    // }
    // else
    // {
    //     mi[3].tarmotorinfo.speed = sumspeed[3];
    // }


}

void init_whemot_pid(PID_regulator wspid[4])
{
    for (int8_t i = 0; i < 4; i++)
    {
        wspid[i].kp = 100;
        wspid[i].ki = 0.5;
        wspid[i].kd = 10;
        wspid[i].componentKpMax = 10000;
        wspid[i].componentKiMax = 3000;
        wspid[i].componentKdMax = 5000;
        wspid[i].outputMax = 8000;
    }
}

void calc_whemot_spid(PID_regulator wspid[4])
{
    for (int8_t i = 0; i < 4; i++)
    {
        wspid[i].tar = whe[i].tarmotorinfo.speed;
        wspid[i].cur = whe[i].curmotorinfo.speed;
        PID_calc(&wspid[i], 1);
    }
}

int16_t can1_mes20x200[4];

void pack_whemot_ctrlmes(int16_t mes[4])
{
    if (robinfo.comd.moton == 0x01)
    {
        for (int8_t i = 0; i < 4; i++)
        {
            mes[i] = whespid[i].output;
        }
    }
    else
    {
        for (int8_t i = 0; i < 4; i++)
        {
            mes[i] = 0;
        }
    }
}

void chasctrl()
{
    init_whemot_pid(whespid);
    for (;;)
    {
        xyzspeed2wheelspeed(whe);
        calc_whemot_spid(whespid);
        powerctrl(4, whespid, calcmaxsumcurrentvalue(&robinfo, &capsinfo));
        pack_whemot_ctrlmes(can1_mes20x200);
        if (robinfo.comd.moton)
        {
            CAN_send(0x200, hcan1, can1_mes20x200);
        }
        else
        {
            CAN_send(0x200, hcan1, zeromes);
        }
        osDelayUntil(mottaskperi);
    }
}