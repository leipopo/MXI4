#include "main.h"

MotorInfo fric[2], trig;
PID_regulator fricspid[2], trigapid, trigspid;

void init_shootmot_para(MotorInfo *fric[2], MotorInfo *trig)
{
    *trig                         = motparainit(m2006);
    trig->setup.motid             = trigmotid;
    *fric[0]                      = motparainit(m3508);
    fric[0]->setup.motid          = fricmotid_1;
    fric[0]->setup.reductionratio = 1;
    *fric[1]                      = *fric[0];
    fric[1]->setup.motid          = fricmotid_2;
}

void init_shootmot_pid(PID_regulator fspid[2],
                       PID_regulator *tapid,
                       PID_regulator *tspid)
{
    fspid[0].kp             = 0;
    fspid[0].ki             = 0;
    fspid[0].kd             = 0;
    fspid[0].outputMax      = fric[0].setup.speed_limit / fric[0].setup.reductionratio;
    fspid[0].componentKpMax = fspid[0].outputMax;
    fspid[0].componentKiMax = fspid[0].outputMax;
    fspid[0].componentKdMax = fspid[0].outputMax;

    fspid[1] = fspid[0];

    tapid->kp             = 0;
    tapid->ki             = 0;
    tapid->kd             = 0;
    tapid->outputMax      = trigspeed;
    tapid->componentKpMax = tapid->outputMax;
    tapid->componentKiMax = tapid->outputMax;
    tapid->componentKdMax = tapid->outputMax;

    tspid->kp             = 0;
    tspid->ki             = 0;
    tspid->kd             = 0;
    tspid->outputMax      = trig.setup.current_value_limit;
    tspid->componentKpMax = tspid->outputMax;
    tspid->componentKiMax = tspid->outputMax;
    tspid->componentKdMax = tspid->outputMax;
}
void get_trigangle_comd(MotorInfo *ti)
{
    if (fabsf(trigapid.err[0]) <= 1 && fabsf(trigapid.err[1]) <= 1 && comuinfo.rx_comd.triggeron == 0x01)
    {
        ti->tarmotorinfo.angle += trigclickangle;
    }
}

void get_fricspeed_comd(MotorInfo fi[2])
{
    if (comuinfo.rx_comd.fricwheelon == 0x01)
    {
        fi[0].tarmotorinfo.speed = fricwheelspeed;
        fi[1].tarmotorinfo.speed = -fi[0].tarmotorinfo.speed;
    }
}

void calc_shootmot_pid(PID_regulator fspid[2], PID_regulator *tapid, PID_regulator *tspid)
{
    fspid[0].tar = fric[0].tarmotorinfo.speed;
    fspid[0].cur = fric[0].curmotorinfo.speed;
    fspid[1].tar = fric[1].tarmotorinfo.speed;
    fspid[1].cur = fric[1].curmotorinfo.speed;
    PID_calc(&fspid[0], 1);
    PID_calc(&fspid[1], 1);

    tapid->tar = trig.tarmotorinfo.angle;
    tapid->cur = trig.curmotorinfo.angle;
    tspid->tar = tapid->output;
    tspid->cur = trig.curmotorinfo.speed;

    calc_mot_aspid(tapid, tspid, 2);
}

int16_t can1_mes20x200[4];
int16_t can2_mes20x200[4];

void pack_shootmot_ctrlmes(int16_t mes1[4], int16_t mes2[4])
{
    mes1[0] = fricspid[0].output;
    mes1[1] = fricspid[1].output;
    mes1[2] = 0x0000;
    mes1[3] = 0x0000;
    mes2[0] = trigspid.output;
    mes2[1] = 0x0000;
    mes2[2] = 0x0000;
    mes2[3] = 0x0000;
}

void shoottask()
{
    for (;;)
    {
        get_trigangle_comd(&trig);
        get_fricspeed_comd(fric);
        calc_shootmot_pid(fricspid, &trigapid, &trigspid);
        pack_shootmot_ctrlmes(can1_mes20x200, can2_mes20x200);
        if (comuinfo.rx_comd.moton)
        {
            CAN_send(0x200, hcan1, can1_mes20x200);
            CAN_send(0x200, hcan2, can2_mes20x200);
        }
        else
        {
             CAN_send(0x200, hcan1, zeromes);
            CAN_send(0x200, hcan2, zeromes);
        }

        osDelayUntil(mottaskperi);
    }
}
