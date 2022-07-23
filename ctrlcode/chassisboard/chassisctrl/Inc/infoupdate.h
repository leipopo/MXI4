#ifndef INFOUPDATE_H
#define INFOUPDATE_H

#include "stdint.h"

#define yawspenom_rc yaw.setup.speed_limit / 660.f
#define yawspenom_ms yaw.setup.speed_limit / 0x7fff

#define pitspenom_rc pit.setup.speed_limit / 660.f
#define pitspenom_ms pit.setup.speed_limit / 0x7fff

typedef struct COMD
{
    uint8_t fricwheelon; // 0x01 on
    uint8_t triggeron;   // 0x01 on
    uint8_t magopen;     // 0x01 open
    uint8_t moton;       // 0x01 on
    uint8_t cvon;        // 0x01 on
} Comd;

typedef struct ROB_INFO
{
    uint8_t robid;
    struct
    {
        float chaspower_limit;
        uint16_t heat0_limit;
        // uint16_t heat1_limit;
    } lim;
    struct
    {
        float chaspower;
        float yawangle; //
        float yawspeed; //
        float pitangle; //
        float pitspeed; //
        float xspeed;
        float yspeed;
        float zspeed;
    } tar;

    struct
    {
        uint16_t heat0; //
        // uint16_t heat1;
        float chaspower;   //
        float powerbuffer; //
        float yawangle;    //
        float yawspeed;    //
        float pitangle;    //
        float pitspeed;    //
        float zrelangle;
    } cur;

    Comd comd;

} RobInfo;

extern RobInfo robinfo;

#endif // !INFOUPDATE_H
