#include "main.h"
int16_t can2_mes2chas_imu[4];
int16_t can2_mes2chas_cv[4];
ComuInfo comuinfo;

void canrx2comuinfo_comd(uint8_t rx[8], ComuInfo *ci)
{
    ci->rx_comd.fricwheelon = rx[0];
    ci->rx_comd.triggeron   = rx[1];
    ci->rx_comd.magopen     = rx[2];
    ci->rx_comd.moton       = rx[3];
    ci->rx_comd.cvon        = rx[4];
}

void pack_mes2chas_imu(int16_t mes[4])
{
    mes[0] = (int16_t)comuinfo.tx_imu.yawangle;
    mes[1] = (int16_t)comuinfo.tx_imu.pitangle;
    mes[2] = (int16_t)comuinfo.tx_imu.yawspeed;
    mes[3] = (int16_t)comuinfo.tx_imu.pitspeed;
}
void pack_mes2chas_cv(int16_t mes[4])
{
    mes[0] = (int16_t)comuinfo.tx_cv.yawangle;
    mes[1] = (int16_t)comuinfo.tx_cv.pitangle;
}

void comutask()
{
    for (;;)
    {

        pack_mes2chas_imu(can2_mes2chas_imu);
        CAN_send(gimbboardid_imu, hcan2, can2_mes2chas_imu);
        osDelayUntil(comutaskperi);

        if (comuinfo.rx_comd.cvon << 4 != 0x00)
        {
            pack_mes2chas_cv(can2_mes2chas_cv);
            CAN_send(gimbboardid_cv, hcan2, can2_mes2chas_cv);
        }
    }
}