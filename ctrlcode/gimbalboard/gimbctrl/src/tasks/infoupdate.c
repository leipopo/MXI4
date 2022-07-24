#include "main.h"

void get_comuinfo_imu(ComuInfo *ci)
{
    ci->tx_imu.yawangle = rad2deg(INS_angle[0]);
    ci->tx_imu.pitangle = rad2deg(INS_angle[2]);

    ci->tx_imu.yawspeed = radps2rpm(bmi088_real_data.gyro[0]);
    ci->tx_imu.pitspeed = radps2rpm(bmi088_real_data.gyro[2]);
}

void infoupdate()
{
    for(;;)
    {
        get_comuinfo_imu(&comuinfo);
        osDelayUntil(infotaskperi);
    }
}
