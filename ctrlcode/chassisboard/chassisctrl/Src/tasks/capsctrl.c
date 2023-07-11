#include "main.h"

CAPSInfo capsinfo = {0, 0, 0, 0};

float calcmaxsumcurrentvalue(RobInfo *ri, CAPSInfo *ci)
{
    float chasumcurrentlimit = 0.f;

    if (ci->capsvoltage == 0.f)
    {

        chasumcurrentlimit = (ri->lim.chaspower_limit + powerlimit_offset + (fmax(fminf(ri->cur.powerbuffer - safepowerbuff, 1), 0) * 60 / exppowerpeaktime)) / 24.f + chasumcurrentlimit_offset;
    }
    else
    {
        if (ci->capsvoltage <= deadcapsvoltage)
        {
            chasumcurrentlimit = (ri->lim.chaspower_limit + powerlimit_offset) / ci->capsvoltage + chasumcurrentlimit_offset;
        }
        else if (ci->capsvoltage < slowdowncapsvoltage)
        {
            chasumcurrentlimit = ((maxoutputpower - ri->lim.chaspower_limit) * (ci->capsvoltage - deadcapsvoltage) / (slowdowncapsvoltage - deadcapsvoltage) + ri->lim.chaspower_limit) / ci->capsvoltage + chasumcurrentlimit_offset;
        }
        else
        {
            chasumcurrentlimit = maxoutputpower / ci->capsvoltage + chasumcurrentlimit_offset;
        }
    }
    return chasumcurrentlimit / maxcurrent_3508 * maxcurrent_value_3508;
}

void powerctrl(uint8_t motornum, PID_regulator whe[], float maxsumcurrentvalue)
{
    float sumcurrentvalue = 0.f;
    for (uint8_t i = 0; i < motornum; i++)
    {
        sumcurrentvalue += fabsf(whe[i].output);
    }
    if (sumcurrentvalue > maxsumcurrentvalue)
    {
        for (uint8_t i = 0; i < motornum; i++)
        {
            whe[i].output = whe[i].output / sumcurrentvalue * maxsumcurrentvalue;
        }
    }
}

void canrx2capsinfo(uint8_t rx[8], CAPSInfo *ci)
{
    ci->inputvoltage = ((int16_t)((rx[1] << 8) | (rx[0]))) / 100.f;
    ci->capsvoltage = ((int16_t)((rx[3] << 8) | (rx[2]))) / 100.f;
    ci->inputcurrent = ((int16_t)((rx[5] << 8) | (rx[4]))) / 100.f;
    // ci->setpower = ((int16_t)((rx[7] << 8) | (rx[6]))) / 100.f;
}

void capsctrl()
{
    for (;;)
    {
        int16_t mes[4];
        capsinfo.setpower = fminf(maxoutputpower, robinfo.lim.chaspower_limit + powerlimit_offset + (robinfo.cur.powerbuffer / exppowerpeaktime + safepowerbuff));
        mes[0] = (int16_t)(capsinfo.setpower * 100);
        CAN_send(0x210, hcan2, mes);
        osDelayUntil(capstaskperi);
    }
}