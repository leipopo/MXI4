#ifndef COMUTASK_H
#define COMUTASK_H

typedef struct _COMU_INFO
{
    struct 
    {
        float yawangle;
        float pitangle;
        float yawspeed;
        float pitspeed;
    }rx_imu;

    struct 
    {
        uint8_t fricwheelon;//0x01 on
        uint8_t triggeron;//0x01 on
        uint8_t magopen;//0x01 open
    }tx_shootcommend;

}ComuInfo;
extern ComuInfo comuinfo;
#endif