#ifndef DEVINIT_H
#define DEVINIT_H
#include "robconfig.h"

#define zerostate(state) state = 0x00
#define serid(id, num) id + num

#define can1idlist                    \
    {                                 \
        serid(wheid, 0x00000000),     \
            serid(wheid, 0x00000001), \
            serid(wheid, 0x00000002), \
            serid(wheid, 0x00000003), \
            yawmotid,                 \
            pitmotid,                 \
            0x00000000,               \
            0x00000000                \
    }

#define can2idlist             \
    {                          \
        capsid,                \
            gimbboardid_yaw, \
            gimbboardid_pit, \
            gimbboardid_cv,    \
            0x00000000,        \
            0x00000000,        \
            0x00000000,        \
            0x00000000         \
    }

extern uint32_t can1_idlist[8], can2_idlist[8];
extern uint8_t can1devsta, can2devsta;
uint8_t can_probe(uint32_t id, uint32_t idlist[8]);

#endif // !DEVINIT_H
