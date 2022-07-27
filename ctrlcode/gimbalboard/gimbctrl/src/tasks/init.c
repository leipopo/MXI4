#include "main.h"
void chasmotinit()
{
    init_shootmot_para(fric, &trig);
    init_shootmot_pid(fricspid, &trigapid, &trigspid);
}

void devinit()
{
    can_filter_init();
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}

void userinit()
{
    chasmotinit();
    devinit();
}
