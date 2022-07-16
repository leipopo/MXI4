#ifndef DEVINIT_H
#define DEVINIT_H
#include "robconfig.h"

#define zeroprobe(probe) probe=0x00
#define serid(id,num) id+num 

#define can1idlist \
{ \
    wheid, \
    serid(wheid,0x00000001), \
    serid(wheid,0x00000002), \
    serid(wheid,0x00000003), \
    steid, \
    serid(steid,0x00000001), \
    serid(steid,0x00000002), \
    serid(steid,0x00000003), \
} \

#define can2idlist \
{ \
    yawmotid, \
    pitmotid, \
    mpuid, \
    capsid, \
    0x00000000, \
    0x00000000, \
    0x00000000, \
    0x00000000 \
} \

extern uint32_t can1_idlist[8],can2_idlist[8];
uint8_t can_probe(uint32_t id,uint32_t idlist[8]);
#endif // !DEVINIT_H
