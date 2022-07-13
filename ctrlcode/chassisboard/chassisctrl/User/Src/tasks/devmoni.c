#include "main.h"

uint16_t can1_idlist[8]=can1idlist,can2_idlist[8]=can2idlist;

uint8_t can_probe(uint16_t id,uint16_t idlist[8])
{
    static uint8_t candevsta;
    for(int8_t i=0;i<8;i++)
    {
        if(id==idlist[i])
        {
            candevsta|=0x01<<i;
            break;
        }
    }
    return candevsta;
}


