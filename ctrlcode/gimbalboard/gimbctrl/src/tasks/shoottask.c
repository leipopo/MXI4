#include "main.h"
//遥控器逻辑
// 1 1 关电机 --> 1 3 开电机/关弹舱/关摩擦轮 --> 1 2 开弹舱
//                     ||
//                     ||
//                     \/
// 3 1 开摩擦轮 --> 3 3 开电机无动作 --> 3 2 发弹
//                     ||
//                     ||
//                     \/
// 2 1 开自瞄 --> 2 3 开电机无动作 --> 2 2 开小陀螺

// 3 3 开电机无动作-->2 3 开电机无动作  切换自瞄模式F
MotorInfo fric[2], trig;
PID_regulator fricspid[2] = {pid_default_config, pid_default_config}, trigapid = pid_default_config, trigspid = pid_default_config;

void init_shootmot_para(MotorInfo fric[2], MotorInfo *trig)
{
    *trig                        = motparainit(m2006);
    trig->setup.motid            = trigmotid;
    trig->setup.outcirclerate =2;

    fric[0]                      = motparainit(m3508);
    fric[0].setup.motid          = fricmotid_1;
    fric[0].setup.reductionratio = 1.f;
    fric[1]                      = fric[0];
    fric[1].setup.motid          = fricmotid_2;

}

void init_shootmot_pid(PID_regulator fspid[2],
                       PID_regulator *tapid,
                       PID_regulator *tspid)
{
    fspid[0].kp             = 20;
    fspid[0].ki             = 0.005;
    fspid[0].kd             = 0;
    fspid[0].outputMax      = 20000;
    fspid[0].componentKpMax = fspid[0].outputMax;
    fspid[0].componentKiMax = fspid[0].outputMax;
    fspid[0].componentKdMax = fspid[0].outputMax;

    fspid[1] = fspid[0];

    tapid->kp             = 3;
    tapid->ki             = 0;
    tapid->kd             = 0;
    tapid->outputMax      = trigspeed;
    tapid->componentKpMax = tapid->outputMax;
    tapid->componentKiMax = tapid->outputMax;
    tapid->componentKdMax = tapid->outputMax;

    tspid->kp             = 120;
    tspid->ki             = 0.01;
    tspid->kd             = 0;
    tspid->outputMax      = 8000;
    tspid->componentKpMax = tspid->outputMax;
    tspid->componentKiMax = tspid->outputMax;
    tspid->componentKdMax = tspid->outputMax;
}
void get_trigangle_comd(MotorInfo *ti)
{
    if (fabsf(trigapid.err[0]) <= 10 && fabsf(trigapid.err[1]) <= 10 && comuinfo[0].rx_comd.triggeron == 0x01)
    {
        ti->tarmotorinfo.angle = numcircle(180.f, -180.f, ti->tarmotorinfo.angle - trigclickangle);
    }
}

void get_fricspeed_comd(MotorInfo fi[2])
{
    if (comuinfo[0].rx_comd.fricwheelon == 0x01)
    {
        fi[0].tarmotorinfo.speed = fricwheelspeed;
        fi[1].tarmotorinfo.speed = -fi[0].tarmotorinfo.speed;
    }
    else if (comuinfo[0].rx_comd.fricwheelon == 0x00)
    {
        fi[0].tarmotorinfo.speed = 0;
        fi[1].tarmotorinfo.speed = -fi[0].tarmotorinfo.speed;
    }
}

void calc_shootmot_pid(PID_regulator fspid[2], PID_regulator *tapid, PID_regulator *tspid, MotorInfo *mi)
{
    fspid[0].tar = fric[0].tarmotorinfo.speed;
    fspid[0].cur = fric[0].curmotorinfo.speed;
    fspid[1].tar = fric[1].tarmotorinfo.speed;
    fspid[1].cur = fric[1].curmotorinfo.speed;
    PID_calc(&fspid[0], 1);
    PID_calc(&fspid[1], 1);

    if (comuinfo->rx_comd.magopen == 0x01)
    {
        trig.tarmotorinfo.angle = trig.curmotorinfo.angle;
    }
    tapid->tar = trig.tarmotorinfo.angle;
    tapid->cur = trig.curmotorinfo.angle;
    tspid->tar = tapid->output;
    tspid->cur = trig.curmotorinfo.speed;

    calc_mot_aspid(tapid, tspid, mi);
    if (comuinfo->rx_comd.magopen == 0x01)
    {
        tspid->output = 500.f;
    }
}

int16_t can1_mes20x200[4];
int16_t can2_mes20x200[4];

void pack_shootmot_ctrlmes(int16_t mes1[4])
{
    mes1[0] = fricspid[0].output;
    mes1[1] = fricspid[1].output;
    mes1[2] = trigspid.output;
    mes1[3] = 0x0000;
}

void setmag(int8_t mc)
{
    if (mc == 1)
    {
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, magopenpusle);
    }
    else
    {
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, magclosepusle);
    }
}

void shoottask()
{
    for (;;)
    {
        get_trigangle_comd(&trig);
        get_fricspeed_comd(fric);

        calc_shootmot_pid(fricspid, &trigapid, &trigspid, &trig);
        pack_shootmot_ctrlmes(can1_mes20x200);
        if (comuinfo[0].rx_comd.moton)
        {

            CAN_send(0x200, hcan1, can1_mes20x200);
            osDelayUntil(mottaskperi);
        }
        else
        {
            CAN_send(0x200, hcan1, zeromes);
            osDelayUntil(mottaskperi);
        }
        setmag(comuinfo[0].rx_comd.magopen);
    }
}
