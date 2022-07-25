#include "main.h"
void chasmotinit()
{
    init_shootmot_para(fric, &trig);
    init_shootmot_pid(fricspid, &trigapid, &trigspid);
}

void devinit()
{
    can_filter_init();
}

void userinit()
{
    chasmotinit();
    devinit();
}
