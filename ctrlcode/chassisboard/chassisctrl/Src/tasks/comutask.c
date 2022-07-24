#include "main.h"
ComuInfo comuinfo;

int16_t can2_mes2gimb_comd[4];

void canrx2comuinfo_rximu(uint8_t rx[8], ComuInfo *ci)
{
    ci->rx_imu.yawangle = (int16_t)(rx[0] << 8 | rx[1]);
    ci->rx_imu.pitangle = (int16_t)(rx[2] << 8 | rx[3]);
    ci->rx_imu.yawspeed = (int16_t)(rx[4] << 8 | rx[5]);
    ci->rx_imu.pitspeed = (int16_t)(rx[6] << 8 | rx[7]);
}

void canrx2comuinfo_rxcv(uint8_t rx[8], ComuInfo *ci)
{
    ci->rx_cv.yawangle = (int16_t)(rx[0] << 8 | rx[1]);
    ci->rx_cv.pitangle = (int16_t)(rx[2] << 8 | rx[3]);
}

void get_comuinfo_robinfo(ComuInfo *ci)
{
    ci->tx_comd = robinfo.comd;
}

void pack_2gimb_comdmes(int16_t mes[4])
{
    mes[0] = comuinfo.tx_comd.fricwheelon << 8 | comuinfo.tx_comd.triggeron;
    mes[1] = comuinfo.tx_comd.magopen << 8 | comuinfo.tx_comd.moton;
    mes[2] = comuinfo.tx_comd.cvon << 8;
}

void comutask()
{
    for (;;)
    {
        get_comuinfo_robinfo(&comuinfo);
        pack_2gimb_comdmes(can2_mes2gimb_comd);
        CAN_send(chasboardid, hcan2, can2_mes2gimb_comd);
        osDelayUntil(comutaskperi);
    }
}