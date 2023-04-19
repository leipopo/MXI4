#include "main.h"

CAPSInfo capsinfo;

float calcmaxsumcurrentvalue(CAPSInfo *ci)
{
    if(ci->capsvoltage>slowdowncapsvoltage)
    {

        //功率优先策略
        // if(robinfo.lim.chaspower_limit>40.f&&robinfo.lim.chaspower_limit<50.f)
        //     return maxsumcurrentvalue_1;
        // else if(robinfo.lim.chaspower_limit>=50.f&&robinfo.lim.chaspower_limit<55.f)
        //     return maxsumcurrentvalue_2;
        // else if (robinfo.lim.chaspower_limit>=55.f&&robinfo.lim.chaspower_limit<60.f)
        // {
        //     return maxsumcurrentvalue_3;
        // }
            
        return maxsumcurrentvalue_3;
    }
    else if(ci->capsvoltage>deadcapsvoltage)
    {
        return (maxsumcurrentvalue_3-maxsumcurrentvalue_1)*(ci->capsvoltage-deadcapsvoltage)/(slowdowncapsvoltage-deadcapsvoltage)+maxsumcurrentvalue_1;
    }
    else
    {
        return maxsumcurrentvalue_1;
    }
}

void powerctrl(uint8_t motornum, PID_regulator whe[], float maxsumcurrentvalue)
{
    float sumcurrent = 0.f;
    for(uint8_t i=0;i<motornum;i++)
    {
        sumcurrent += whe[i].output;
    }
    if(sumcurrent>maxsumcurrentvalue)
    {
        for(uint8_t i=0;i<motornum;i++)
        {
            whe[i].output = whe[i].output / sumcurrent * maxsumcurrentvalue;
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
        capsinfo.setpower = fminf(120, robinfo.lim.chaspower_limit);
        mes[0] = (int16_t)(capsinfo.setpower * 100);
        CAN_send(0x210, hcan2, mes);
        osDelayUntil(capstaskperi);
    }
}