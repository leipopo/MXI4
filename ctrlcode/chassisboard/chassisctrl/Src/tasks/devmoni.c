#include "main.h"

uint32_t can1_idlist[8] = can1idlist, can2_idlist[8] = can2idlist;
uint8_t can1devsta = 0x00, can2devsta = 0x00;
uint8_t can_probe(uint32_t id, uint32_t idlist[8])
{
    static uint8_t candevsta;
    for (int8_t i = 0; i < 8; i++)
    {
        if (id == idlist[i])
        {
            candevsta |= 0x01 << i;
            break;
        }
    }
    return candevsta;
}

void devmoni()
{
    for (;;)
    {
        if (can1devsta == 0xff && can2devsta == 0x06 && RC_ON == 0x01)
        {
            // HAL_IWDG_Refresh(&hiwdg);
            zerostate(can1devsta);
            zerostate(can2devsta);
            RC_ON = 0x00;
        }
        osDelayUntil(1000);
    }
}
