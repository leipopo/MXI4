#include "main.h"
RobInfo robinfo;
float yawzeroangle[2];
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
    uint8_t yawinstallangle_toflash[2];
    yawinstallangle_toflash[0] = (uint16_t)yaw.setup.installationangle>>8;
    yawinstallangle_toflash[1] = (uint16_t)yaw.setup.installationangle;
    flash_write_single_address(ADDR_FLASH_SECTOR_11,(uint32_t *)yawinstallangle_toflash,(2 + 3) / 4);
}

void get_limits(RobInfo *ri)
{
    get_chassis_power_and_buffer(&ri->cur.chaspower, &ri->cur.powerbuffer, &ri->lim.chaspower_limit);
    get_shoot_heat0_limit_and_heat0(&ri->lim.heat0_limit, &ri->cur.heat0);
    // get_shoot_heat1_limit_and_heat0(ri->lim.heat1_limit,ri->cur.heat1);
}

void get_gimbtarangle_rc(RobInfo *ri)
{
    ri->tar.yawangle += RC_Data.rc.ch[0] * yawspenom_rc + RC_Data.mouse.x * yawspenom_ms;
    ri->tar.pitangle += RC_Data.rc.ch[1] * pitspenom_rc + RC_Data.mouse.y * pitspenom_ms;
    ri->tar.pitangle = LIMIT(ri->tar.pitangle, pit.setup.angle_limit[0], pit.setup.angle_limit[1]);
}

void get_gimbcurangle_imu(RobInfo *ri)
{
    ri->cur.yawangle = comuinfo.rx_imu.yawangle;
    ri->cur.pitangle = comuinfo.rx_imu.pitangle;
    ri->cur.yawspeed = comuinfo.rx_imu.yawspeed;
    ri->cur.pitspeed = comuinfo.rx_imu.pitspeed;
}

void get_zrelangle(RobInfo *ri)
{
    ri->cur.zrelangle = yaw.curmotorinfo.angle - yaw.setup.installationangle;
}

void robinfoupdater()
{
    
    for (;;)
    {
        robinfo.robid = get_robot_id();
        get_limits(&robinfo);
        get_gimbtarangle_rc(&robinfo);
        get_gimbcurangle_imu(&robinfo);
        get_zrelangle(&robinfo);
        
        osDelayUntil(infotaskperi);
    }
}
