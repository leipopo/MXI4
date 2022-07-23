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
#endif