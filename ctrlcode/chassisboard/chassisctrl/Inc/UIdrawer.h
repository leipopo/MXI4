
#ifndef _USER_UI_H_
#define _USER_UI_H_

#include "main.h"


#define USERUI_TASK_CONTROL_TIME_MS 10

//按下手动刷新键一段时间，UI全部重绘（当UI发生异常时使用）
#define USERUI_MANUAL_REFRESH_CONTROL_TIME  1000
#define USERUI_MANUAL_REFRESH_KEY           KEY_PRESSED_OFFSET_Z

//每隔一段时间，所有UI控件（不包括背景）重绘一次
#define USERUI_MODULE_REFRESH_TIME          1000


typedef struct  
{
    const RC_DATA* rc;
    uint16_t all_refresh_key_press_time;    //全屏刷新按键按下的时长
    uint8_t all_refresh_flag;               //全屏刷新标志位
    uint16_t module_refresh_timer;          //每隔一段时间，所有UI控件（不包括背景）重绘一次
    uint8_t module_extern_flag;             //UI元素已存在标志
} userUI_control_t;


/**
  * @brief          自定义UI任务，在client上绘制自定义UI
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern void userUI_task(void const* argument);

#endif
