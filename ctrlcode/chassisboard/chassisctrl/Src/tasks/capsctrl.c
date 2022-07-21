#include "main.h"

void capsctrl()
{
    for(;;)
    {
        osDelayUntil(capstaskperi);
    }
}