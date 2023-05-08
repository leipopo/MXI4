#include "main.h"
ComuInfo comuinfo;

int16_t can2_mes2gimb_comd[4];

// 遥控器逻辑
//  1 1 关电机 --> 1 3 开电机/关弹舱/关摩擦轮 --> 1 2 开弹舱
//                      ||
//                      ||
//                      \/
//  3 1 开摩擦轮 --> 3 3 开电机无动作 --> 3 2 发弹
//                      ||
//                      ||
//                      \/
//  2 1 开自瞄 --> 2 3 开电机无动作 --> 2 2 开小陀螺

// 3 3 开电机无动作-->2 3 开电机无动作  切换自瞄模式

void canrx2comuinfo_rxangle(uint8_t rx[8], ComuInfo *ci)
{
    float temp[4] = {0.f, 0.f, 0.f, 0.f};
    temp[0] = (int16_t)(rx[0] << 8 | rx[1]);
    temp[1] = -(int16_t)(rx[2] << 8 | rx[3]);
    temp[2] = (int16_t)(rx[4] << 8 | rx[5]);
    temp[3] = (int16_t)(rx[6] << 8 | rx[7]);

    if (ci->rx_imu.yawangle != 0)
    {
        if (fabsf(numcircle(180.f, -180.f, ci->rx_imu.yawangle - temp[0] / 100.f)) < 5.f)
        {
            ci->rx_imu.yawangle = temp[0] / 100.f;
        }
    }
    else
    {
        ci->rx_imu.yawangle = temp[0] / 100.f;
    }

    if (ci->rx_imu.pitangle != 0)
    {
        if (fabsf(numcircle(180.f, -180.f, ci->rx_imu.pitangle - temp[1] / 100.f)) < 5.f)
        {
            ci->rx_imu.pitangle = temp[1] / 100.f;
        }
    }
    else
    {
        ci->rx_imu.pitangle = temp[1] / 100.f;
    }

    // ci->rx_imu.yawangle = temp[0] / 100.f;
    // ci->rx_imu.pitangle = temp[1] / 100.f;
    ci->rx_cv.yawangle = temp[2] / 100.f;
    ci->rx_cv.pitangle = temp[3] / 100.f;
}

// void canrx2comuinfo_rximuspeed(uint8_t rx[8], ComuInfo *ci)
// {
//     float temp[4] = {0.f, 0.f, 0.f, 0.f};
//     temp[0] = (int16_t)(rx[0] << 8 | rx[1]);
//     temp[1] = (int16_t)(rx[2] << 8 | rx[3]);
//     temp[2] = (int16_t)(rx[4] << 8 | rx[5]);
//     temp[3] = (int16_t)(rx[6] << 8 | rx[7]);
//     if ((temp[0] == 0x1234) && (temp[3] == (temp[1] + temp[2])))
//     {
//         ci->rx_imu.yawspeed = temp[1];
//         ci->rx_imu.pitspeed = temp[2];
//     }
// }

// void canrx2comuinfo_rxcv(uint8_t rx[8], ComuInfo *ci)
// {
//     ci->rx_cv.yawangle = (int16_t)(rx[0] << 8 | rx[1]);
//     ci->rx_cv.pitangle = (int16_t)(rx[2] << 8 | rx[3]);
// }

void get_comuinfo_robinfo(ComuInfo *ci)
{
    ci->tx_comd = robinfo.comd;
}

void pack_2gimb_comdmes(int16_t mes[4])
{
    mes[0] = 0x22 << 8 | comuinfo.tx_comd.fricwheelon;
    mes[1] = comuinfo.tx_comd.triggeron << 8 | comuinfo.tx_comd.magopen;
    mes[2] = comuinfo.tx_comd.moton << 8 | comuinfo.tx_comd.cvon;
    mes[3] = (comuinfo.tx_comd.fricwheelon + comuinfo.tx_comd.triggeron + comuinfo.tx_comd.magopen + comuinfo.tx_comd.moton + comuinfo.tx_comd.cvon) << 8 | 0x33;
}

void comutask()
{
    for (;;)
    {
        get_comuinfo_robinfo(&comuinfo);
        pack_2gimb_comdmes(can2_mes2gimb_comd);
        if (robinfo.comd.moton == 0x01)
        {
            CAN_send(chasboardid, hcan2, can2_mes2gimb_comd);
        }
        osDelayUntil(comutaskperi);
    }
}