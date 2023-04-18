#ifndef CAPCTRL_H
#define CAPCTRL_H

typedef struct CAPSINFO
{
    float inputvoltage;
    float capsvoltage;
    float inputcurrent;
    float setpower;
} CAPSInfo;

extern CAPSInfo capsinfo;
void canrx2capsinfo(uint8_t rx[8], CAPSInfo *ci);

#endif