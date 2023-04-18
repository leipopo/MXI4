#include "main.h"

CAPSInfo capsinfo;
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
        mes[0] = (int16_t)(capsinfo.setpower * 100);
        CAN_send(0x211, hcan2,mes);
        osDelayUntil(capstaskperi);
    }
}