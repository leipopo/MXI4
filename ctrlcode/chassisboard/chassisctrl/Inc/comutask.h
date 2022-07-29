#ifndef COMUTASK_H
#define COMUTASK_H

#include "infoupdate.h"

typedef struct _COMU_INFO
{
    struct
    {
        float yawangle;
        float pitangle;
        float yawspeed;
        float pitspeed;
    } rx_imu;

    struct
    {
        float yawangle;
        float pitangle;
    } rx_cv;

    Comd tx_comd;

} ComuInfo;


extern ComuInfo comuinfo;
void canrx2comuinfo_rxcv(uint8_t rx[8], ComuInfo *ci);
void canrx2comuinfo_rximuspeed(uint8_t rx[8], ComuInfo *ci);
void canrx2comuinfo_rximuangle(uint8_t rx[8], ComuInfo *ci);
#endif