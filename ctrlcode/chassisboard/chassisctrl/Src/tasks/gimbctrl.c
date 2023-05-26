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
    // uint8_t yawinstallangle_fromflash[2];
    // flash_read(ADDR_FLASH_SECTOR_11, (uint32_t *)yawinstallangle_fromflash, (2 + 3) / 4);
    //  yi->setup.installationangle = (int16_t)(yawinstallangle_fromflash[0] << 8 | yawinstallangle_fromflash[1]);
    yi->setup.installationangle = 0;
    yi->setup.speed_limit = yi->setup.speed_limit / yi->setup.reductionratio;
    yi->setup.outcirclerate = 10;

    *pi = motparainit(gm6020);
    pi->setup.motid = pitmotid;
    pi->setup.reductionratio = pitreductionratio;
    pi->setup.angle_limit[0] = -40.f;
    pi->setup.angle_limit[1] = 25.f;
    pi->setup.speed_limit = pi->setup.speed_limit / pi->setup.reductionratio;
    pi->setup.outcirclerate = 5;
    pi->setup.reversed = 0x00;
}

void init_gimbmot_pid(PID_regulator *papid,
                      PID_regulator *yapid,
                      PID_regulator *pspid,
                      PID_regulator *yspid)
{
    papid->kp = 5;
    papid->ki = 0.0;
    papid->kd = 3.0;
    papid->outputMax = pit.setup.speed_limit;
    papid->componentKpMax = papid->outputMax;
    papid->componentKiMax = papid->outputMax / 10;
    papid->componentKdMax = papid->outputMax;

    yapid->kp = 5;
    yapid->ki = 0.0;
    yapid->kd = 10.0;
    yapid->outputMax = yaw.setup.speed_limit;
    yapid->componentKpMax = yapid->outputMax;
    yapid->componentKiMax = yapid->outputMax / 2;
    yapid->componentKdMax = yapid->outputMax;

    pspid->kp = 500;
    pspid->ki = 0.0005;
    pspid->kd = 2000;
    pspid->outputMax = pit.setup.current_value_limit;
    pspid->componentKpMax = 30000;
    pspid->componentKiMax = 20000;
    pspid->componentKdMax = 20000;

    yspid->kp = 1200;
    yspid->ki = 0.0005;
    yspid->kd = 1000;
    yspid->outputMax = yaw.setup.current_value_limit;
    yspid->componentKpMax = 30000;
    yspid->componentKiMax = 20000;
    yspid->componentKdMax = 20000;
}

void clac_pitmot_aspid(PID_regulator *papid,
                       PID_regulator *pspid,
                       MotorInfo *mi)
{
    // pit.tarmotorinfo.angle = numcircle(180.f, -180.f, pit.curmotorinfo.angle + (robinfo.tar.pitangle - robinfo.cur.pitangle + robinfo.tar.yawangle - robinfo.cur.yawangle));
    // papid->tar = pit.tarmotorinfo.angle;
    // papid->cur = pit.curmotorinfo.angle;

    papid->tar = robinfo.tar.pitangle;
    papid->cur = robinfo.cur.pitangle;

    pspid->tar = -papid->output;
    pspid->cur = robinfo.cur.pitspeed;
    papid->outputMax = pit.setup.speed_limit;

    calc_mot_aspid(papid, pspid, mi);
    // double Gangle = robinfo.cur.pitangle + 5.f;
    // pspid->output -= (500 * sin(Gangle / 360.f * 2 * 3.141592f));
    pspid->output = pspid->output -yaw.curmotorinfo.speed * 100.f;
}

void clac_yawmot_aspid(PID_regulator *yapid,
                       PID_regulator *yspid,
                       MotorInfo *mi)
{
    yapid->tar = robinfo.tar.yawangle;
    yapid->cur = robinfo.cur.yawangle;

    yspid->tar = -yapid->output;
    yspid->cur = robinfo.cur.yawspeed;

    yapid->outputMax = yaw.setup.speed_limit;

    calc_mot_aspid(yapid, yspid, mi);
    // if (yspid->output > 100.f)
    // {
    //     yspid->output += 2000.f;
    // }
    // else if (yspid->output < -100.f)
    // {
    //     yspid->output -= 2000.f;
    // }
    // yspid->output += robinfo.tar.zspeed * 100.f;
    // yspid->output +=yspid->cur *00.f;
}

int16_t can1_mes20x2ff[4];
void pack_pymot_ctrlmes(int16_t mes[4])
{
    if (robinfo.comd.moton == 0x01)
    {
        mes[1] = -yawspid.output;
        mes[2] = -pitspid.output;
    }
    else
    {
        mes[1] = 0x0000;
        mes[2] = 0x0000;
    }
}

void minpitangle_calc(MotorInfo *pi)
{
    if (Key.key_ctrl == 0x01)
    {
        pi->setup.angle_limit[1] = 30.f;
    }
    else
    {
        pi->setup.angle_limit[1] = 25.f;
    }
}

void gimbctrl()
{
    while (comuinfo.rx_imu.yawangle == 0 || comuinfo.rx_imu.pitangle == 0)
    {
        osDelayUntil(10);
    }
    robinfo.tar.yawangle = comuinfo.rx_imu.yawangle;
    robinfo.tar.pitangle = comuinfo.rx_imu.pitangle;

    for (;;)
    {
        minpitangle_calc(&pit);
        // HAL_IWDG_Refresh(&hiwdg);
        clac_yawmot_aspid(&yawapid, &yawspid, &yaw);
        clac_pitmot_aspid(&pitapid, &pitspid, &pit);
        pack_pymot_ctrlmes(can1_mes20x2ff);
        if (robinfo.comd.moton)
        {
            CAN_send(0x2ff, hcan1, can1_mes20x2ff);
        }
        else
        {
            CAN_send(0x2ff, hcan1, zeromes);
        }

        osDelayUntil(mottaskperi);
    }
}
