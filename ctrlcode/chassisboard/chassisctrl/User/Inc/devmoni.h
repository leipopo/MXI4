#ifndef DEVINIT_H
#define DEVINIT_H
#include "robconfig.h"

#define zero(probe) probe=0x00

#define can1idlist \
{ \
    wheid, \
    wheid+1, \
    wheid+2, \
    wheid+3, \
    steid, \
    steid+1, \
    steid+2, \
    steid+3 \
} \

#define can2idlist \
{ \
    yawmotid, \
    pitmotid, \
    mpuid, \
    0x0000, \
    0x0000, \
    0x0000, \
    0x0000, \
    0x0000 \
} \

#endif // !DEVINIT_H
