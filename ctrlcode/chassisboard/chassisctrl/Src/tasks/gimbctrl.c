#include "main.h"

MotorInfo yaw, pit;
PID_regulator pitapid = pid_default_config, yawapid = pid_default_config,
              pitspid = pid_default_config, yawspid = pid_default_config;

void init_gimbmot_para(MotorInfo *pi, MotorInfo *yi)
{
    *yi = motparainit(gm6020);
    yi->setup.motid = yawmotid;
    yi->setup.reductionratio = yawreductionratio;
    /***********************************************flashtest*************************************************
        uint8_t yawinstallangle_toflash[2];// test
        yawinstallangle_toflash[0] = 0xaa; // test
        yawinstallangle_toflash[1] = 0xbb; // test
        flash_write_single_address(ADDR_FLASH_SECTOR_11, (uint32_t *)yawinstallangle_toflash, (2 + 3) / 4);// test
    ********************************************************************************/
    uint8_t yawinstallangle_fromflash[2];
    flash_read(ADDR_FLASH_SECTOR_11, (uint32_t *)yawinstallangle_fromflash, (2 + 3) / 4);
    yi->setup.installationangle = (int16_t)(yawinstallangle_fromflash[0] << 8 | yawinstallangle_fromflash[1]);
    yi->setup.outcirclerate=3;


    *pi = motparainit(gm6020);
    pi->setup.motid = pitmotid;
    pi->setup.reductionratio = pitreductionratio;
    pi->setup.angle_limit[0] = -20.f;
    pi->setup.angle_limit[1] = 36.f;
    pi->setup.outcirclerate=3;
    pi->setup.reversed=0x01;
}

void init_gimbmot_pid(PID_regulator *papid,
                      PID_regulator *yapid,
                      PID_regulator *pspid,
                      PID_regulator *yspid)
{
    papid->kp = 3;
    papid->ki = 0.0;
    papid->kd = 0;
    papid->outputMax = pit.setup.speed_limit / pit.setup.reductionratio;
    papid->componentKpMax = papid->outputMax;
    papid->componentKiMax = papid->outputMax;
    papid->componentKdMax = papid->outputMax;

    yapid->kp = 5;
    yapid->ki = 0;
    yapid->kd = 0;
    yapid->outputMax = yaw.setup.speed_limit / yaw.setup.reductionratio;
    yapid->componentKpMax = yapid->outputMax;
    yapid->componentKiMax = yapid->outputMax;
    yapid->componentKdMax = yapid->outputMax;

    pspid->kp = 160;
    pspid->ki = 0.005;
    pspid->kd = 50;
    pspid->outputMax = pit.setup.current_value_limit;
    pspid->componentKpMax = 10000;
    pspid->componentKiMax = 2000;
    pspid->componentKdMax = 2000;

    yspid->kp = 130;
    yspid->ki = 0.00025;
    yspid->kd = 0;
    yspid->outputMax = yaw.setup.current_value_limit;
    yspid->componentKpMax = 10000;
    yspid->componentKiMax = 6000;
    yspid->componentKdMax = yspid->outputMax;
}

void clac_pitmot_aspid(PID_regulator *papid,
                       PID_regulator *pspid,
                       MotorInfo *mi)
{
    papid->tar = robinfo.tar.pitangle;
    papid->cur = robinfo.cur.pitangle;

    pspid->tar = papid->output;
    pspid->cur = robinfo.cur.pitspeed;
    calc_mot_aspid(papid, pspid, mi);
    pspid->output -=(500*sin((robinfo.cur.pitangle+5.f)/360.f*2.f*3.1415f)+robinfo.cur.yawspeed*120); 
    pspid->output=-pspid->output;
}

void clac_yawmot_aspid(PID_regulator *yapid,
                       PID_regulator *yspid,
                       MotorInfo *mi)
{
    yapid->tar = robinfo.tar.yawangle;
    yapid->cur = robinfo.cur.yawangle;

    yspid->tar = yapid->output;
    yspid->cur = robinfo.cur.yawspeed;
    calc_mot_aspid(yapid, yspid, mi);
    if (yspid->output > 0.f)
    {
        yspid->output += 1000.f;
    }
    else if (yspid->output < -0.f)
    {
        yspid->output -= 1000.f;
    }
}

int16_t can1_mes20x1ff[4];
void pack_pymot_ctrlmes(int16_t mes[4])
{
    if (robinfo.comd.moton == 0x01)
    {
        mes[0] = yawspid.output;
        mes[1] = pitspid.output;
    }
    else
    {
        mes[0] = 0x0000;
        mes[1] = 0x0000;
    }
}

void gimbctrl()
{

    for (;;)
    {
        clac_yawmot_aspid(&yawapid, &yawspid, &yaw);
        clac_pitmot_aspid(&pitapid, &pitspid, &pit);
        pack_pymot_ctrlmes(can1_mes20x1ff);
        if (robinfo.comd.moton)
        {
            CAN_send(0x1ff, hcan1, can1_mes20x1ff);
        }
        else
        {
            CAN_send(0x1ff, hcan1, zeromes);
        }

        osDelayUntil(mottaskperi);
    }
}
