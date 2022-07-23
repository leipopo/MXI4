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

    *pi = motparainit(gm6020);
    pi->setup.motid = pitmotid;
    pi->setup.reductionratio = pitreductionratio;
    pi->setup.angle_limit[0] = -20.f;
    pi->setup.angle_limit[1] = 30.f;
}

void init_gimbmot_pid(PID_regulator *papid,
                      PID_regulator *yapid,
                      PID_regulator *pspid,
                      PID_regulator *yspid)
{
    papid->kp = 0;
    papid->ki = 0;
    papid->kd = 0;
    papid->outputMax = pit.setup.speed_limit / pit.setup.reductionratio;
    papid->componentKpMax = papid->outputMax;
    papid->componentKiMax = papid->outputMax;
    papid->componentKdMax = papid->outputMax;

    yapid->kp = 0;
    yapid->ki = 0;
    yapid->kd = 0;
    yapid->outputMax = yaw.setup.speed_limit / yaw.setup.reductionratio;
    yapid->componentKpMax = yapid->outputMax;
    yapid->componentKiMax = yapid->outputMax;
    yapid->componentKdMax = yapid->outputMax;

    pspid->kp = 0;
    pspid->ki = 0;
    pspid->kd = 0;
    pspid->outputMax = pit.setup.current_value_limit;
    pspid->componentKpMax = pspid->outputMax;
    pspid->componentKiMax = pspid->outputMax;
    pspid->componentKdMax = pspid->outputMax;

    yspid->kp = 0;
    yspid->ki = 0;
    yspid->kd = 0;
    yspid->outputMax = yaw.setup.current_value_limit;
    yspid->componentKpMax = yspid->outputMax;
    yspid->componentKiMax = yspid->outputMax;
    yspid->componentKdMax = yspid->outputMax;
}

void clac_pitmot_aspid(PID_regulator *papid,
                       PID_regulator *pspid,
                       uint8_t outcircrate)
{
    papid->tar = robinfo.tar.yawangle;
    papid->cur = robinfo.cur.yawangle;

    pspid->tar = papid->output;
    pspid->cur = robinfo.cur.yawspeed;
    clac_mot_aspid(papid, pspid, outcircrate);
}

void clac_yawmot_aspid(PID_regulator *yapid,
                       PID_regulator *yspid,
                       uint8_t outcircrate)
{
    yapid->tar = robinfo.tar.yawangle;
    yapid->cur = robinfo.cur.yawangle;

    yspid->tar = yapid->output;
    yspid->cur = robinfo.cur.yawspeed;
    clac_mot_aspid(yapid, yspid, outcircrate);
}

int16_t can1_mes20x2ff[4];
void pack_pymot_ctrlmes(int16_t mes[4])
{
    if (robinfo.comd.moton==0x01)
    {
        mes[0] = yawspid.output;
        mes[1] = pitspid.output;
    }
    else
    {
        mes[0]=0x0000;
        mes[1]=0x0000;
    }
}

void gimbctrl()
{
    init_gimbmot_para(&pit, &yaw);
    init_gimbmot_pid(&pitapid, &yawapid, &pitspid, &yawspid);
    for (;;)
    {
        clac_yawmot_aspid(&yawapid, &yawspid, 2);
        clac_pitmot_aspid(&pitapid, &pitspid, 3);
        pack_pymot_ctrlmes(can1_mes20x2ff);
        CAN_send(0x2ff, hcan1, can1_mes20x2ff);
        osDelayUntil(mottaskperi);
    }
}
