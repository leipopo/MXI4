#ifndef MOTOR_LIB
#define MOTOR_LIB
#include "main.h"

//单位
// deg
// rpm
// A

// 3508 parameter
#define m3508                 0x00
#define maxspeed_3508         500.f
#define maxcurrent_3508       20.f
#define current_maxvalue_3508 0x4fff
#define currenttotorque_2006  1.f / 3.f
#define gearratio_3508        3591.f / 187.f

// 2006 parameter
#define m2006                 0x01
#define maxspeed_2006         500.f
#define maxcurrent_2006       10.f
#define current_maxvalue_2006 10000
#define currenttotorque_2006  1.f / 3.f
#define gearratio_2006        36.f

// 6020 parameter
#define gm6020                0x02
#define maxspeed_6020         300.f
#define maxcurrent_6020       2.f
#define current_maxvalue_6020 30000
#define currenttotorque_6020  1.f / 2.f
#define gearratio_6020        1.f

typedef struct _Motor_Info
{
    struct
    {
        uint8_t motname;
        uint8_t sumangle; // 0x1累计角度
        uint8_t reversed; // 0x1反装
        uint32_t motid;

        float installationangle; //安装角度，即电机归零时的角度，
        float angle_limit[2];    //角度限制，无限制则为{-180，180}，代码内使用单位度
        float speed_limit;       //速度限制，查询电机电调套装手册，代码内使用单位rpm
        float current_limit;     //电流限制，查询电机电调套装手册，代码内使用单位A
        float inpositioncurrent; //撞限位的电流值，使用单位A
        float reductionratio;    //电机所在机构构成的减速倍率，减速时代码内此值应大于一

    } setup;

    struct
    {
        float angle[2];
    } temp;

    struct
    {
        float angle;
        float speed;
        float current;
    } tarmotorinfo;

    struct
    {
        float angle;
        float speed;
        float current[5];
    } curmotorinfo;

} MotorInfo;

#define motor_default_config               \
    {                                      \
        {                                  \
            0x00,                          \
            0x00,                          \
            0x00,                          \
            0x00000000,                    \
            0.f,                           \
            {-180.f, 180.f},               \
            0.f,                           \
            0.f,                           \
            0.f,                           \
            0.f,                           \
        },                                 \
            {{0.f, 0.f}},                  \
            {                              \
                0.f,                       \
                0.f,                       \
                0.f,                       \
            },                             \
            {                              \
                0.f,                       \
                0.f,                       \
                {0.f, 0.f, 0.f, 0.f, 0.f}, \
            },                             \
    }

#define m3508_default_config               \
    {                                      \
        {                                  \
            0x00,                          \
            0x00,                          \
            0x00,                          \
            0x00000000,                    \
            0.f,                           \
            {-180.f, 180.f},               \
            maxspeed_3508,                 \
            maxcurrent_3508,               \
            0.f,                           \
            gearratio_3508,                \
        },                                 \
            {{0.f, 0.f}},                  \
            {                              \
                0.f,                       \
                0.f,                       \
                0.f,                       \
            },                             \
            {                              \
                0.f,                       \
                0.f,                       \
                {0.f, 0.f, 0.f, 0.f, 0.f}, \
            },                             \
    }

#define m2006_default_config               \
    {                                      \
        {                                  \
            0x01,                          \
            0x00,                          \
            0x00,                          \
            0x00000000,                    \
            0.f,                           \
            {-180.f, 180.f},               \
            maxspeed_2006,                 \
            maxcurrent_2006,               \
            0.f,                           \
            gearratio_2006,                \
        },                                 \
            {{0.f, 0.f}},                  \
            {                              \
                0.f,                       \
                0.f,                       \
                0.f,                       \
            },                             \
            {                              \
                0.f,                       \
                0.f,                       \
                {0.f, 0.f, 0.f, 0.f, 0.f}, \
            },                             \
    }

#define gm6020_default_config              \
    {                                      \
        {                                  \
            0x02,                          \
            0x00,                          \
            0x00,                          \
            0x00000000,                    \
            0.f,                           \
            {-180.f, 180.f},               \
            maxspeed_6020,                 \
            maxcurrent_6020,               \
            0.f,                           \
            gearratio_6020,                \
        },                                 \
            {{0.f, 0.f}},                  \
            {                              \
                0.f,                       \
                0.f,                       \
                0.f,                       \
            },                             \
            {                              \
                0.f,                       \
                0.f,                       \
                {0.f, 0.f, 0.f, 0.f, 0.f}, \
            },                             \
    }

float numcircle(float max, float min, float num);
MotorInfo motparainit(uint8_t motname);
void gearmotorangle_calc(MotorInfo *mi);
uint32_t getmotid(MotorInfo *mi);
void canrx2motinfo(uint8_t rx[8], MotorInfo *mi);
#endif
