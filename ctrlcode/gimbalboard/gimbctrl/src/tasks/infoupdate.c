#include "main.h"

void get_comuinfo_imu(ComuInfo *ci)
{
    ci->tx_imu.yawangle = rad2deg(INS_angle[0]);
    ci->tx_imu.pitangle = rad2deg(INS_angle[1]);

    ci->tx_imu.yawspeed = radps2rpm(INS_gyro[2]);
    ci->tx_imu.pitspeed = radps2rpm(INS_gyro[1]);
}

void get_comuinfo_cv(ComuInfo *ci)
{
    if (ci->rx_comd.cvon == 0x00)
    {
        ci->tx_cv.yawangle = 0.f;
        ci->tx_cv.pitangle = 0.f;
    }
    else
    {
        ci->tx_cv.yawangle = NUC_data.yaw;
        ci->tx_cv.pitangle = NUC_data.pitch;
    }
}

void infoupdate()
{
    for (;;)
    {
        get_comuinfo_cv(&comuinfo[0]);
        get_comuinfo_imu(&comuinfo[0]);
        osDelayUntil(infotaskperi);
    }
}
