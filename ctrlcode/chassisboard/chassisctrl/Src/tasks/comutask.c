#include "main.h"
ComuInfo comuinfo;

int16_t can2_mes2gimbboard[4];

void get_comuinfo_robinfo(ComuInfo *comuinfo)
{
    comuinfo->tx_comd= robinfo.comd;
}

void pack_gimb_ctrlmes(int16_t mes[4])
{
    mes[0] = comuinfo.tx_comd.fricwheelon << 8 | comuinfo.tx_comd.triggeron;
    mes[1] = comuinfo.tx_comd.magopen << 8 | comuinfo.tx_comd.moton;
    mes[2] = comuinfo.tx_comd.cvon << 8;
}

void comutask()
{
    for (;;)
    {
        pack_gimb_ctrlmes(can2_mes2gimbboard);

        osDelayUntil(comutaskperi);
    }
}