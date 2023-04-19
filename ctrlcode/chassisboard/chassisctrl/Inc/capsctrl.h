#ifndef CAPCTRL_H
#define CAPCTRL_H

#define deadcapsvoltage 3.0f
#define slowdowncapsvoltage 10.f
typedef struct CAPSINFO
{
    float inputvoltage;
    float capsvoltage;
    float inputcurrent;
    float setpower;
} CAPSInfo;

float calcmaxsumcurrentvalue(CAPSInfo *ci);

void powerctrl(uint8_t motornum, PID_regulator whe[], float maxsumcurrentvalue);

extern CAPSInfo capsinfo;
void canrx2capsinfo(uint8_t rx[8], CAPSInfo *ci);

#endif