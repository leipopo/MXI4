#ifndef CAPCTRL_H
#define CAPCTRL_H

#define deadcapsvoltage 13.0f
#define slowdowncapsvoltage 18.f
#define chasumcurrentlimit_offset 0.5f
#define powerlimit_offset 0.f

#define safepowerbuff 0.f
#define maxoutputpower 100.f


typedef struct CAPSINFO
{
    float inputvoltage;
    float capsvoltage;
    float inputcurrent;
    float setpower;
} CAPSInfo;

float calcmaxsumcurrentvalue(RobInfo *ri ,CAPSInfo *ci);

void powerctrl(uint8_t motornum, PID_regulator whe[], float maxsumcurrentvalue);

extern CAPSInfo capsinfo;
void canrx2capsinfo(uint8_t rx[8], CAPSInfo *ci);

#endif