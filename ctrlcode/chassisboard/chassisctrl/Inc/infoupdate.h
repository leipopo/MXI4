#ifndef INFOUPDATE_H
#define INFOUPDATE_H

#include "stdint.h"

#define mousespeed_normalize(a) ((float)a) / 0x7fff
#define rcchannel_normalize(a) ((float)a) / 660.f

#define yawspeedconst 20.f
#define pitspeedconst 10.f

#define expcvmovetime 0.75f

// #define yawspeedconst 20.f
// #define pitspeedconst 10.f

typedef struct COMD
{
    uint8_t fricwheelon; // 0x01 on
    uint8_t triggeron;   // 0x01 on
    uint8_t magopen;     // 0x01 open
    uint8_t moton;       // 0x01 on
    uint8_t cvon;        // 0x01 on 
    uint8_t spinning;    // 0x01 on
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
