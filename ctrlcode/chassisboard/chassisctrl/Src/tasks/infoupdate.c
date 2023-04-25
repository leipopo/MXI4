#include "main.h"
RobInfo robinfo;
float yawzeroangle[2];

// 遥控器逻辑
//  1 1 关电机 --> 1 3 开电机/关弹舱/关摩擦轮 --> 1 2 开弹舱
//                      ||
//                      ||
//                      \/
//  3 1 开摩擦轮 --> 3 3 开电机无动作 --> 3 2 发弹
//                      ||
//                      ||
//                      \/
//  2 1 开自瞄 --> 2 3 开电机无动作 --> 2 2 开小陀螺     3 3 开电机无动作-->2 3 开电机无动作

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_SET)
    {
        yawzeroangle[0] = yaw.curmotorinfo.angle;
        yawzeroangle[1] = 0;
    }
    else if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_RESET)
    {
        yawzeroangle[1] = yaw.curmotorinfo.angle;
        yaw.setup.installationangle = (yawzeroangle[1] + yawzeroangle[0]) / 2;
        yawzeroangle[0] = 0;
    }
    // uint8_t yawinstallangle_toflash[2];
    // yawinstallangle_toflash[0] = (uint16_t)yaw.setup.installationangle >> 8;
    // yawinstallangle_toflash[1] = (uint16_t)yaw.setup.installationangle;
    //flash_write_single_address(ADDR_FLASH_SECTOR_11, (uint32_t *)yawinstallangle_toflash, (2 + 3) / 4);
}

void init_robinfo(RobInfo *ri)
{
    while (yaw.curmotorinfo.angle == 0.f)
    {
        osDelayUntil(10);
    }
    ri->tar.yawangle = yaw.curmotorinfo.angle;
}

void get_limits(RobInfo *ri)
{
    get_chassis_power_and_buffer(&ri->cur.chaspower, &ri->cur.powerbuffer, &ri->lim.chaspower_limit);
    get_shoot_heat0_limit_and_heat0(&ri->lim.heat0_limit, &ri->cur.heat0);
    // get_shoot_heat1_limit_and_heat0(ri->lim.heat1_limit,ri->cur.heat1);
    
}

void get_comd_rc(RobInfo *ri)
{

    if (RC_Data.rc.s[0] == 1 && RC_Data.rc.s[1] == 1)
    {
        ri->comd.moton = 0x00;
        ri->comd.fricwheelon = 0x00;
    }
    else
    {
        ri->comd.moton = 0x01;
    }

    if ((Key.key_r == 0x01) || (RC_Data.rc.s[0] == 1 && RC_Data.rc.s[1] == 2))
    {
        ri->comd.magopen = 0x01;
        ri->comd.fricwheelon = 0x00;
    }
    else
    {
        ri->comd.magopen = 0x00;
    }

    if (RC_Data.rc.s[0] == 3 && RC_Data.rc.s[1] == 1)
    {
        ri->comd.fricwheelon = 0x01;
    }

    if ((RC_Data.mouse.press_l == 0x01) || (RC_Data.rc.s[0] == 3 && RC_Data.rc.s[1] == 2))
    {
        if (robinfo.cur.heat0 < robinfo.lim.heat0_limit)
        {
            ri->comd.triggeron = 0x01;
        }
        else
        {
            ri->comd.triggeron = 0x00;
        }
        ri->comd.fricwheelon = 0x01;
    }
    else
    {
        ri->comd.triggeron = 0x00;
    }

    if(Key.key_z == 0x01&&Last_Key.key_z == 0x00)
    {
        ri->comd.cvon = 0x01;
    }
    else if(Key.key_x == 0x01&&Last_Key.key_x == 0x00)
    {
        ri->comd.cvon = 0x08;
    }

    if ((RC_Data.rc.s[0] == 2 && RC_Data.rc.s[1] == 1) || (RC_Data.mouse.press_r == 0x01))
    {
        ri->comd.cvon &= 0x0F;
    }
    else
    {
        ri->comd.cvon = ri->comd.cvon >> 4;
        ri->comd.cvon = ri->comd.cvon << 4;
    }

    if ((RC_Data.rc.s[0] == 2 && RC_Data.rc.s[1] == 2) || (Key.key_c == 0x01))
    {
        ri->comd.spinning = 0x01;
    }
    else
    {
        ri->comd.spinning = 0x00;
    }

    

}

void get_chastarspeed_rc(RobInfo *ri)
{
    ri->tar.xspeed = (rcchannel_normalize(RC_Data.rc.ch[3]) + (Key.key_w - Key.key_s)) * movespeed;
    ri->tar.yspeed = (rcchannel_normalize(RC_Data.rc.ch[2]) + (Key.key_d - Key.key_a)) * movespeed;
    ri->tar.zspeed = (rcchannel_normalize(RC_Data.rc.ch[4]) + (ri->comd.spinning)) * spinningspeed;
}

void get_gimbtarangle_cv(RobInfo *ri)
{

    ri->tar.yawangle -= numcircle(180.f, -180.f, comuinfo.rx_cv.yawangle * robinfo.comd.cvon / fre(infotaskperi));
    ri->tar.pitangle += comuinfo.rx_cv.pitangle * robinfo.comd.cvon / fre(infotaskperi);
    ri->tar.pitangle = LIMIT(ri->tar.pitangle, pit.setup.angle_limit[0], pit.setup.angle_limit[1]);

    //HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ri->comd.cvon, sizeof(ri->comd.cvon));
}

void get_gimbtarangle_rc(RobInfo *ri)
{
    ri->tar.yawangle += (-rcchannel_normalize(RC_Data.rc.ch[0]) + LIMIT(RC_Data.mouse.x,-10,10) / 8.f ) / fre(infotaskperi) * yawspeedconst;
    ri->tar.pitangle += (rcchannel_normalize(RC_Data.rc.ch[1]) + LIMIT(RC_Data.mouse.y,-10,10) / 20.f) / fre(infotaskperi) * pitspeedconst;
    ri->tar.yawangle = numcircle(180.f, -180.f, ri->tar.yawangle);
    ri->tar.pitangle = LIMIT(ri->tar.pitangle, pit.setup.angle_limit[0], pit.setup.angle_limit[1]);
}

void get_gimbcurangle_imu(RobInfo *ri)
{
    ri->cur.yawangle = comuinfo.rx_imu.yawangle * 0.5 + ri->cur.yawangle * 0.5; // 低通一下
    ri->cur.pitangle = comuinfo.rx_imu.pitangle * 0.5 + ri->cur.pitangle * 0.5; // 低通一下
}

void get_gimbcurangle_mot(RobInfo *ri)
{
    // 本来是有读角度的

    ri->cur.yawspeed = yaw.curmotorinfo.speed * 0.3 + ri->cur.yawspeed * 0.7; // 低通一下
    ri->cur.pitspeed = pit.curmotorinfo.speed * 0.3 + ri->cur.pitspeed * 0.7; // 低通一下
}

void get_zrelangle(RobInfo *ri)
{
    ri->cur.zrelangle = numcircle(180.f, -180.f, yaw.curmotorinfo.angle - yaw.setup.installationangle - 90.f);
}

void infoupdate()
{
    init_robinfo(&robinfo);
    for (;;)
    {
        robinfo.robid = get_robot_id();

        get_limits(&robinfo);
        get_gimbtarangle_rc(&robinfo);
        get_gimbtarangle_cv(&robinfo);
        get_gimbcurangle_imu(&robinfo);
        get_gimbcurangle_mot(&robinfo);
        get_comd_rc(&robinfo);
        get_zrelangle(&robinfo);
        get_chastarspeed_rc(&robinfo);

        osDelayUntil(infotaskperi);
    }
}
