#include "main.h"

MotorInfo motparainit(uint8_t motname)
{
    switch (motname)
    {
        case m3508:
        {
            MotorInfo mi = m3508_default_config;
            return mi;
        }
        case m2006:
        {
            MotorInfo mi = m2006_default_config;
            return mi;
        }
        case gm6020:
        {
            MotorInfo mi = gm6020_default_config;
            return mi;
        }
        default:
        {
            MotorInfo mi = motor_default_config;
            return mi;
        }
    }
}

void canrx2motinfo(uint8_t rx[8], MotorInfo *mi)
{
    float dir         = -(((float)mi->setup.reversed) - 0.5f) * 2.f;
    mi->temp.angle[1] = ((int16_t)((rx[0] << 8) | (rx[1]))) * 360.f / 0x1fff * dir;
    gearmotorangle_calc(mi);
    mi->curmotorinfo.speed = ((int16_t)((rx[2] << 8) | (rx[3]))) / mi->setup.reductionratio * dir;
    // mi->curmotorinfo.current[4] = ((int16_t)((rx[2] << 8) | (rx[3]))) / 0x1fff * dir;
}

void gearmotorangle_calc(MotorInfo *mi)
{

    mi->temp.angle[0] = mi->temp.angle[1] - mi->temp.angle[0];

    if (mi->temp.angle[0] > 150.f)
        mi->temp.angle[0] -= 360;
    else if (mi->temp.angle[0] < -150.f)
        mi->temp.angle[0] += 360;

    mi->temp.angle[0] /= mi->setup.reductionratio;

    if (mi->setup.sumangle == 0x01)
        mi->curmotorinfo.angle += mi->temp.angle[0];
    else
    {
        mi->curmotorinfo.angle = numcircle(180.f, -180.f, mi->temp.angle[0] + mi->curmotorinfo.angle);
    }
    mi->temp.angle[0] = mi->temp.angle[1];
}

void calc_mot_aspid(PID_regulator *apid,
                    PID_regulator *spid,
                    MotorInfo *mi)
{
    if (mi->temp.aspidcounter == mi->setup.outcirclerate)
    {
        PID_angle_calc(apid, 1, 300.f);
    }
    else if ((mi->temp.aspidcounter >= 0 )&& (mi->temp.aspidcounter < mi->setup.outcirclerate))
    {
        mi->temp.aspidcounter++;
    }
    else
    {
        mi->temp.aspidcounter = 0;
    }
    PID_calc(spid, 1);
}

float numcircle(float max, float min, float num)
{
    float rang = max - min;
    if (num >= min && num <= max)
        return num;
    else if (num > max)
        while ((num = num - rang) > max)
            ;
    else if (num < min)
        while ((num = num + rang) < min)
            ;
    return num;
}

uint32_t getmotid(MotorInfo *mi)
{
    return mi->setup.motid;
}
