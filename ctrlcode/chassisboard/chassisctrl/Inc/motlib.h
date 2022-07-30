#ifndef MOTOR_LIB
#define MOTOR_LIB
#include "stdint.h"
#include "pid.h"

// 3508 parameter
#define m3508                 0x00
#define maxspeed_3508         9600.f
#define maxcurrent_3508       20.f
#define maxcurrent_value_3508 0x4fff
#define currenttotorque_3508  1.f / 2.5f
#define gearratio_3508        3591.f / 187.f

// 2006 parameter
#define m2006                 0x01
#define maxspeed_2006         18000.f
#define maxcurrent_2006       10.f
#define maxcurrent_value_2006 10000
#define currenttotorque_2006  1.f / 3.f
#define gearratio_2006        36.f

// 6020 parameter
#define gm6020                0x02
#define maxspeed_6020         300.f
#define maxcurrent_6020       2.f
#define maxcurrent_value_6020 30000
#define currenttotorque_6020  1.f / 2.f
#define gearratio_6020        1.f

typedef struct _Motor_Info
{
    struct
    {
        uint8_t motname;
        uint8_t sumangle; // 0x01累计角度
        uint8_t reversed; /*0x01反转 轴线垂直时从上往下看逆时针为正
                            轴线水平时从出轴往电机屁股看逆时针为正*/
        uint32_t motid;

        float installationangle;   //安装角度6020标定用
        float angle_limit[2];      //无限位为{-180，180}
        float speed_limit;         // rpm
        float current_value_limit; //向电调输出值
        float inpositioncurrent;   // A
        float reductionratio;      //
        int8_t outcirclerate;
    } setup;

    struct
    {
        int8_t aspidcounter;
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
            1.f,                           \
            0,                             \
        },                                 \
            {                              \
                0,                         \
                {0.f, 0.f},                \
            },                             \
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
            1.f,                           \
            {-180.f, 180.f},               \
            maxspeed_3508,                 \
            maxcurrent_value_3508,         \
            0.f,                           \
            gearratio_3508,                \
            0,                             \
        },                                 \
            {                              \
                0,                         \
                {0.f, 0.f},                \
            },                             \
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
            maxcurrent_value_2006,         \
            0.f,                           \
            gearratio_2006,                \
        },                                 \
            {                              \
                0,                         \
                {0.f, 0.f},                \
            },                             \
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
            maxcurrent_value_6020,         \
            0.f,                           \
            gearratio_6020,                \
        },                                 \
            {                              \
                0,                         \
                {0.f, 0.f},                \
            },                             \
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
void calc_mot_aspid(PID_regulator *apid,
                    PID_regulator *spid,
                    MotorInfo *mi);
#endif
