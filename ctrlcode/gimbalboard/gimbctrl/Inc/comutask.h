#ifndef COMUTASK_H
#define COMUTASK_H

#include "infoupdate.h"
typedef struct COMD
{
    uint8_t fricwheelon; // 0x01 on
    uint8_t triggeron;   // 0x01 on
    uint8_t magopen;     // 0x01 open
    uint8_t moton;       // 0x01 on
    uint8_t cvon;        // 0x01 on
} Comd;

typedef struct _CV
{
    float yawangle;
    float pitangle;
} CV;

typedef struct _IMU
{
    float yawangle;
    float pitangle;
    float yawspeed;
    float pitspeed;
} IMU; 

typedef struct COMU_INFO
{
    IMU tx_imu;

    CV tx_cv;

    Comd rx_comd;

} ComuInfo;

extern ComuInfo comuinfo[3];
void canrx2comuinfo_comd(uint8_t rx[8], ComuInfo ci[3]);
#endif