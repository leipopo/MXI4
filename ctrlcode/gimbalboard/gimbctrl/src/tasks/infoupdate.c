#include "main.h"

void get_comuinfo_imu(ComuInfo *ci)
{
    ci->tx_imu.yawangle = rad2deg(INS_angle[0]);
    ci->tx_imu.pitangle = rad2deg(INS_angle[1]);

    ci->tx_imu.yawspeed = radps2rpm(INS_gyro[2]);
    ci->tx_imu.pitspeed = radps2rpm(INS_gyro[1]);
}

void infoupdate()
{
    for (;;)
    {
        get_comuinfo_imu(&comuinfo[0]);
        osDelayUntil(infotaskperi);
    }
}
