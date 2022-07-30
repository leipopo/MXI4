#include "main.h"
int16_t can2_mes2chas_imuangle[4];
int16_t can2_mes2chas_imuspeed[4];
int16_t can2_mes2chas_cv[4];
ComuInfo comuinfo[3];

//遥控器逻辑
// 1 1 关电机 --> 1 3 开电机/关弹舱/关摩擦轮 --> 1 2 开弹舱
//                     ||
//                     ||
//                     \/
// 3 1 开摩擦轮 --> 3 3 开电机无动作 --> 3 2 发弹
//                     ||
//                     ||
//                     \/
// 2 1 开自瞄 --> 2 3 开电机无动作 --> 2 2 开小陀螺

// 3 3 开电机无动作-->2 3 开电机无动作  切换自瞄模式

void canrx2comuinfo_comd(uint8_t rx[8], ComuInfo ci[3])
{
    if ((rx[0] == 0x22 && rx[7] == 0x33) && (rx[6] == (rx[1] + rx[2] + rx[3] + rx[4] + rx[5])))
    {
        ci[0].rx_comd.fricwheelon = ci[1].rx_comd.fricwheelon;
        ci[1].rx_comd.fricwheelon = ci[2].rx_comd.fricwheelon;
        ci[2].rx_comd.fricwheelon = rx[1];
        ci[0].rx_comd.fricwheelon = ci[0].rx_comd.fricwheelon & ci[1].rx_comd.fricwheelon & ci[2].rx_comd.fricwheelon;

        ci[0].rx_comd.triggeron = ci[1].rx_comd.triggeron;
        ci[1].rx_comd.triggeron = ci[2].rx_comd.triggeron;
        ci[2].rx_comd.triggeron = rx[2];
        ci[0].rx_comd.triggeron = ci[0].rx_comd.triggeron & ci[1].rx_comd.triggeron & ci[2].rx_comd.triggeron;

        ci[0].rx_comd.magopen = ci[1].rx_comd.magopen;
        ci[1].rx_comd.magopen = ci[2].rx_comd.magopen;
        ci[2].rx_comd.magopen = rx[3];
        ci[0].rx_comd.magopen = ci[0].rx_comd.magopen & ci[1].rx_comd.magopen & ci[2].rx_comd.magopen;

        ci[0].rx_comd.moton = ci[1].rx_comd.moton;
        ci[1].rx_comd.moton = ci[2].rx_comd.moton;
        ci[2].rx_comd.moton = rx[4];
        ci[0].rx_comd.moton = ci[0].rx_comd.moton & ci[1].rx_comd.moton & ci[2].rx_comd.moton;

        ci[0].rx_comd.cvon = ci[1].rx_comd.cvon;
        ci[1].rx_comd.cvon = ci[2].rx_comd.cvon;
        ci[2].rx_comd.cvon = rx[5];
        ci[0].rx_comd.cvon = ci[0].rx_comd.cvon & ci[1].rx_comd.cvon & ci[2].rx_comd.cvon;
    }
}

void pack_mes2chas_imuangle(int16_t mes[4])
{
    mes[1] = (int16_t)comuinfo[0].tx_imu.yawangle;
    mes[2] = (int16_t)comuinfo[0].tx_imu.pitangle;
    mes[0] = 0x4321 ;
    mes[3] = mes[1]+mes[2];
}
void pack_mes2chas_imuspeed(int16_t mes[4])
{

    mes[1] = (int16_t)comuinfo[0].tx_imu.yawspeed;
    mes[2] = (int16_t)comuinfo[0].tx_imu.pitspeed;

    mes[0] = 0x1234 ;
    mes[3] = mes[1]+mes[2];
}
void pack_mes2chas_cv(int16_t mes[4])
{
    mes[0] = (int16_t)comuinfo[0].tx_cv.yawangle;
    mes[1] = (int16_t)comuinfo[0].tx_cv.pitangle;
}

void comutask()
{
    for (;;)
    {

        pack_mes2chas_imuangle(can2_mes2chas_imuangle);
        CAN_send(gimbboardid_imuangle, hcan2, can2_mes2chas_imuangle);
        osDelayUntil(comutaskperi);

        //pack_mes2chas_imuspeed(can2_mes2chas_imuspeed);
        //CAN_send(gimbboardid_imuspeed, hcan2, can2_mes2chas_imuspeed);
        //osDelayUntil(comutaskperi);
        

        // if (comuinfo[0].rx_comd.cvon << 4 != 0x00)
        // {
        //     pack_mes2chas_cv(can2_mes2chas_cv);
        //     CAN_send(gimbboardid_cv, hcan2, can2_mes2chas_cv);
        // }
    }
}