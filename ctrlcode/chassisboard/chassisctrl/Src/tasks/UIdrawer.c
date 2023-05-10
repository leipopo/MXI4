
#include "main.h"



#define UI_NEED_REFRESH (userUI_control.module_refresh_timer > USERUI_MODULE_REFRESH_TIME)
#define UI_IS_EXTERN    (userUI_control.module_extern_flag)


/**
  * @brief          自定义UI初始化，数据初始化，绘制背景元素
  * @param[in]      none
  * @retval         none
  */
void userUI_init(void);

/**
  * @brief          自定义UI绘制背景元素
  * @param[in]      none
  * @retval         none
  */
void userUI_draw_background(void);

/**
  * @brief          自定义UI绘制密位瞄准靶
  * @param[in]      水平补偿（左负右正）
  * @param[in]      垂直补偿（上正下负）
  * @param[in]      缩放比例
  * @retval         none
  */
void userUI_draw_foresight(int16_t delta_x, int16_t delta_y, fp32 scale);

/**
  * @brief          自定义UI绘制小陀螺状态（画在图层2上）
  * @param[in]      强制刷新使能
  * @param[in]      显示使能
  * @retval         none
  */
//void userUI_draw_wipping(uint8_t refresh, uint8_t en);

/**
  * @brief          自定义UI绘制恒功率电源余量（画在图层3上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      图形结构体的指针
  * @param[in]      恒功率电源的电压
  * @retval         none
  */
//void userUI_draw_constant_power_allowance(uint8_t en, Graph_Data* graph, fp32 cap_volt);

/**
  * @brief          自定义UI绘制底盘姿态（画在图层4上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      图形：直线
  * @param[in]      图形：圆弧
  * @param[in]      底盘相对云台的角度 [-PI, PI]
  * @retval         none
  */
//void userUI_draw_chassis_angle(uint8_t en, Graph_Data* line, Graph_Data* arc, fp32 chassis_angle);

/**
  * @brief          自定义UI绘制各模块在线状态（画在图层5上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      强制刷新使能
  * @param[in]      D-BUS在线状态
  * @param[in]      CHASSIS在线状态
  * @param[in]      GIMBAL在线状态
  * @param[in]      SHOOT在线状态
  * @retval         none
  */
//void userUI_draw_module_state(uint8_t en, uint8_t refresh, uint8_t dbus_state, uint8_t chassis_state, uint8_t gimbal_state, uint8_t shoot_state);

/**
  * @brief          自定义UI绘制机器人控制模式（画在图层6上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      强制刷新使能
  * @param[in]      底盘行为模式
  * @retval         none
  */
//void userUI_draw_robot_control_mode(uint8_t en, uint8_t refresh, chassis_behaviour_e behaviour);


//将要显示的图形
Graph_Data graph1, graph2, graph3, graph4, graph5, graph6, graph7;
//将要显示的字符串
String_Data ui_str;

userUI_control_t userUI_control;

/**
  * @brief          自定义UI任务，在client上绘制自定义UI
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void userUI_task(void const* argument)
{
    //等待裁判系统上线
    // while (toe_is_error(REFEREE_TOE))
    // {
    //     osDelay(USERUI_TASK_CONTROL_TIME_MS);
    // }

    //初始化
    userUI_init();

    while (1)
    {
        //7. 手动全屏刷新触发
        if (userUI_control.rc->key.v & USERUI_MANUAL_REFRESH_KEY)
        {
            userUI_control.all_refresh_key_press_time += USERUI_TASK_CONTROL_TIME_MS;
            //当手动刷新按键被按下一段时间后，执行UI全部重绘
            if (userUI_control.all_refresh_key_press_time > USERUI_MANUAL_REFRESH_CONTROL_TIME && userUI_control.all_refresh_flag == 0)
            {
                userUI_control.all_refresh_flag = 1;
                userUI_init();
            }
            //防止计时器溢出
            else if (userUI_control.all_refresh_key_press_time > 60000)
            {
                userUI_control.all_refresh_key_press_time = 0;
            }
        }
        else
        {
            userUI_control.all_refresh_key_press_time = 0;
            userUI_control.all_refresh_flag = 0;
        }

        // //1. 绘制恒功率电源余量（图层3）
        // userUI_draw_constant_power_allowance(UI_IS_EXTERN, &graph1, get_constant_power_volt());
        // ui_display_1_graph(&graph1);

        // //2. 绘制底盘云台的相对姿态
        // userUI_draw_chassis_angle(UI_IS_EXTERN, &graph2, &graph3, get_gimbal_yaw_relative_angle());
        // ui_display_2_graph(&graph2, &graph3);

        // //3. 绘制图传相对俯仰


        // //4. 绘制机器人小陀螺状态（只有发生改变时才绘制，或每秒绘制一次）
        // userUI_draw_wipping(UI_NEED_REFRESH, get_chassis_is_whipping_flag());

        // //5. 绘制机器人行为模式（只有发生改变时才绘制，或每秒绘制一次）
        // userUI_draw_robot_control_mode(UI_IS_EXTERN, UI_NEED_REFRESH, get_chassis_behaviour());

        // //6. 绘制机器人设备离线状态（只有发生改变时才绘制，或每秒绘制一次）
        // userUI_draw_module_state(UI_IS_EXTERN, UI_NEED_REFRESH, toe_is_error(DBUS_TOE), get_chassis_state(), get_gimbal_state(), get_shoot_state());

        //8. 任务挂起
        userUI_control.module_extern_flag = 1;
           userUI_init();
        osDelay(USERUI_TASK_CONTROL_TIME_MS);
    }
}


/**
  * @brief          自定义UI初始化，数据初始化，绘制背景元素
  * @param[in]      none
  * @retval         none
  */
void userUI_init(void)
{
    //userUI_control.rc = get_remote_control_point();
    userUI_control.module_refresh_timer = 0;
    userUI_control.module_extern_flag = 0;

    //清屏
    //UI_delete(UI_Data_Del_ALL, 0);
    //绘制背景元素（图层0，本场比赛不再变动）
    //userUI_draw_background();
    Circle_Draw(&graph1, "BS1", UI_Graph_ADD, 0, UI_Color_Green, 2, 400, 730, 30);
     Circle_Draw(&graph2, "BS2", UI_Graph_ADD, 0, UI_Color_Black, 2, 400, 730, 200);
     ui_display_2_graph(&graph1,&graph2);
    //绘制密位靶（图层1）
    userUI_draw_foresight(0, 0, 1.0f);
}


/**
  * @brief          自定义UI绘制背景元素，背景元素指排在最下层的，不可变的UI元素
  * @param[in]      none
  * @retval         none
  */
// void userUI_draw_background(void)
// {
//     //画恒功率电源余量条 外框，警示线
//     Rectangle_Draw(&graph1, "CPB", UI_Graph_ADD, 0, UI_Color_Green, 3, 550, 90, 1370, 130);
//     Line_Draw(&graph2, "CLH", UI_Graph_ADD, 0, UI_Color_Green, 3, 1136, 91, 1136, 129);
//     Line_Draw(&graph3, "CLL", UI_Graph_ADD, 0, UI_Color_Yellow, 3, 823, 91, 823, 129);
//     //画姿态指示器 基准
//     Line_Draw(&graph4, "BSL", UI_Graph_ADD, 0, UI_Color_Green, 2, 1800, 730, 1800, 800);
//     Circle_Draw(&graph5, "BSR", UI_Graph_ADD, 0, UI_Color_Green, 2, 1800, 730, 20);
//     //画行车辅助线
//     Line_Draw(&graph6, "RC1", UI_Graph_ADD, 0, UI_Color_Green, 2, 400, 0, 800, 400);
//     Line_Draw(&graph7, "RC2", UI_Graph_ADD, 0, UI_Color_Green, 2, 1520, 0, 1120, 400);
//     ui_display_7_graph(&graph1, &graph2, &graph3, &graph4, &graph5, &graph6, &graph7);
//     //画图传俯仰指示器
//     Rectangle_Draw(&graph1, "IT1", UI_Graph_ADD, 0, UI_Color_Green, 2, 50, 700, 150, 750);
//     Circle_Draw(&graph2, "IT2", UI_Graph_ADD, 0, UI_Color_Green, 2, 50, 700, 15);
//     Circle_Draw(&graph3, "IT3", UI_Graph_ADD, 0, UI_Color_Green, 2, 150, 700, 15);
//     Rectangle_Draw(&graph4, "IT4", UI_Graph_ADD, 0, UI_Color_Green, 2, 75, 750, 100, 800);
//     Rectangle_Draw(&graph5, "IT5", UI_Graph_ADD, 0, UI_Color_Green, 2, 100, 775, 150, 795);
//     Circle_Draw(&graph6, "IT6", UI_Graph_ADD, 0, UI_Color_Green, 2, 140, 805, 10);
//     Line_Draw(&graph7, "IT7", UI_Graph_ADD, 0, UI_Color_Green, 2, 140, 785, 160, 785);
//     ui_display_7_graph(&graph1, &graph2, &graph3, &graph4, &graph5, &graph6, &graph7);
//     //画“MODE:"
//     string_Draw(&ui_str, "MDT", UI_Graph_ADD, 0, UI_Color_Green, 2, 20, 550, 155, "MODE:");
//     ui_display_string(&ui_str);
//     //画“12.5V”
//     string_Draw(&ui_str, "CVE", UI_Graph_ADD, 0, UI_Color_Purplish_red, 2, 10, 550, 85, "12.5v");
//     ui_display_string(&ui_str);
//     //画“16V”
//     string_Draw(&ui_str, "CVL", UI_Graph_ADD, 0, UI_Color_Yellow, 2, 10, 810, 85, "16v");
//     ui_display_string(&ui_str);
//     //画“20V”
//     string_Draw(&ui_str, "CVH", UI_Graph_ADD, 0, UI_Color_Green, 2, 10, 1121, 85, "20v");
//     ui_display_string(&ui_str);
//     //画“23V”
//     string_Draw(&ui_str, "CVF", UI_Graph_ADD, 0, UI_Color_Green, 2, 10, 1340, 85, "23v");
//     ui_display_string(&ui_str);
// }


/**
  * @brief          自定义UI绘制密位瞄准靶（画在图层1上）
  * @param[in]      水平补偿（右正左负）
  * @param[in]      垂直补偿（上正下负）
  * @param[in]      缩放比例（保留）
  * @retval         none
  */
void userUI_draw_foresight(int16_t delta_x, int16_t delta_y, fp32 scale)
{
    Line_Draw(&graph1, "FS1", UI_Graph_ADD, 1, UI_Color_Cyan, 1, 960 + delta_x, 370 + delta_y, 960 + delta_x, 580 + delta_y);
    Line_Draw(&graph2, "FS2", UI_Graph_ADD, 1, UI_Color_Cyan, 2, 900 + delta_x, 540 + delta_y, 1020 + delta_x, 540 + delta_y);
    Line_Draw(&graph3, "FS3", UI_Graph_ADD, 1, UI_Color_Cyan, 2, 920 + delta_x, 510 + delta_y, 1000 + delta_x, 510 + delta_y);
    Line_Draw(&graph4, "FS4", UI_Graph_ADD, 1, UI_Color_Cyan, 2, 930 + delta_x, 470 + delta_y, 990 + delta_x, 470 + delta_y);
    Line_Draw(&graph5, "FS5", UI_Graph_ADD, 1, UI_Color_Cyan, 2, 940 + delta_x, 420 + delta_y, 980 + delta_x, 420 + delta_y);
    Line_Draw(&graph6, "FS6", UI_Graph_ADD, 1, UI_Color_Cyan, 2, 950 + delta_x, 380 + delta_y, 970 + delta_x, 380 + delta_y);
    Circle_Draw(&graph7, "FS7", UI_Graph_ADD, 1, UI_Color_Cyan, 1, 960 + delta_x, 540 + delta_y, 20);
    ui_display_7_graph(&graph1, &graph2, &graph3, &graph4, &graph5, &graph6, &graph7);
}


/**
  * @brief          自定义UI绘制小陀螺状态（画在图层2上）
  * @param[in]      强制刷新使能
  * @param[in]      显示使能
  * @retval         none
  */
// void userUI_draw_wipping(uint8_t refresh, uint8_t display)
// {
//     static uint8_t is_working = 0;

//     if (display == 1)
//     {
//         if (is_working == 0 || refresh)
//         {
//             is_working = 1;
//             string_Draw(&ui_str, "WIP", UI_Graph_ADD, 2, UI_Color_Yellow, 2, 20, 1200, 155, "WHIPPING");
//             ui_display_string(&ui_str);
//         }
//     }
//     else
//     {
//         if (is_working == 1 || refresh)
//         {
//             is_working = 0;
//             string_Draw(&ui_str, "WIP", UI_Graph_Del, 2, UI_Color_Yellow, 2, 20, 1200, 155, "WHIPPING");
//             ui_display_string(&ui_str);
//         }
//     }
// }


/**
  * @brief          自定义UI绘制恒功率电源余量（画在图层3上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      图形结构体的指针
  * @param[in]      恒功率电源的电压
  * @retval         none
  */
// void userUI_draw_constant_power_allowance(uint8_t en, Graph_Data* graph, fp32 cap_volt)
// {
//     //1. 将 12.5v - 23v 处理成 [0.0, 1.0]
//     cap_volt = (cap_volt - 12.5f) / 10.5f; 
//     if (cap_volt < 0.0f)
//         cap_volt = 0.0f;
//     else if (cap_volt > 1.0f)
//         cap_volt = 1.0f;

//     //2. 绘制图形
//     if (en == 0)
//     {
//         //当电压在20v以上时，显示绿色
//         if (cap_volt > 0.71429f)
//             Line_Draw(graph, "CPN", UI_Graph_ADD, 3, UI_Color_Green, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//         //当电压在16v以上时，显示黄色
//         else if (cap_volt > 0.33333f)
//             Line_Draw(graph, "CPN", UI_Graph_ADD, 3, UI_Color_Yellow, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//         //当电压在16v以下时，显示紫红色
//         else
//             Line_Draw(graph, "CPN", UI_Graph_ADD, 3, UI_Color_Purplish_red, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//     }
//     else
//     {
//         //当电压在20v以上时，显示绿色
//         if (cap_volt > 0.71429f)
//             Line_Draw(graph, "CPN", UI_Graph_Change, 3, UI_Color_Green, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//         //当电压在16v以上时，显示黄色
//         else if (cap_volt > 0.33333f)
//             Line_Draw(graph, "CPN", UI_Graph_Change, 3, UI_Color_Yellow, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//         //当电压在16v以下时，显示紫红色
//         else
//             Line_Draw(graph, "CPN", UI_Graph_Change, 3, UI_Color_Purplish_red, 40, 550, 110, (uint16_t)(550 + 820 * cap_volt), 110);
//     }
// }


/**
  * @brief          自定义UI绘制底盘姿态（画在图层4上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      图形：直线
  * @param[in]      图形：圆弧
  * @param[in]      底盘相对云台的角度 [-PI, PI]
  * @retval         none
  */
// void userUI_draw_chassis_angle(uint8_t en, Graph_Data* line, Graph_Data* arc, fp32 chassis_angle)
// {
//     int16_t delta_x, delta_y;
//     int32_t angle;

//     //计算底盘线段终点的相对坐标
//     delta_x = (int16_t)(50 * sin(-chassis_angle));
//     delta_y = (int16_t)(50 * cos(-chassis_angle));
//     //角度换算成 [0, 360]
//     angle = (int32_t)((-chassis_angle) * 180 / PI);
//     if (angle < 0)
//         angle += 360;
//     if (angle > 360)
//         angle = 360;
//     else if (angle < 0)
//         angle = 0;

//     //绘制图形
//     Line_Draw(line, "RCC", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 4, UI_Color_Cyan, 3, 1800, 730, 1800 + delta_x, 730 + delta_y);
//     if (angle > 180)
//         Arc_Draw(arc, "RCA", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 4, UI_Color_Yellow, angle, 0, 5, 1800, 730, 30, 30);
//     else
//         Arc_Draw(arc, "RCA", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 4, UI_Color_Yellow, 0, angle, 5, 1800, 730, 30, 30);
// }


/**
  * @brief          自定义UI绘制各模块在线状态（画在图层5上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      强制刷新使能
  * @param[in]      D-BUS在线状态
  * @param[in]      CHASSIS在线状态
  * @param[in]      GIMBAL在线状态
  * @param[in]      SHOOT在线状态
  * @retval         none
  */
// void userUI_draw_module_state(uint8_t en, uint8_t refresh, uint8_t dbus_state, uint8_t chassis_state, uint8_t gimbal_state, uint8_t shoot_state)
// {
//     static uint8_t last_dbus, last_chassis, last_gimbal, last_shoot;

//     if (en == 0)
//     {
//         //输出DBUS状态
//         string_Draw(&ui_str, "MD1", UI_Graph_ADD, 5, dbus_state == 0 ? UI_Color_Green : UI_Color_Purplish_red, 2, 15, 1460, 350, "DBUS");
//         ui_display_string(&ui_str);
//         last_dbus = dbus_state;
//         //输出CHASSIS状态
//         switch (chassis_state)
//         {
//         case 1:
//             string_Draw(&ui_str, "MD2", UI_Graph_ADD, 5, UI_Color_Purplish_red, 2, 15, 1550, 350, "CHASSIS");
//             break;

//         case 2:
//             string_Draw(&ui_str, "MD2", UI_Graph_ADD, 5, UI_Color_Cyan, 2, 15, 1550, 350, "CHASSIS");
//             break;

//         case 3:
//             string_Draw(&ui_str, "MD2", UI_Graph_ADD, 5, UI_Color_Yellow, 2, 15, 1550, 350, "CHASSIS");
//             break;

//         default:
//             string_Draw(&ui_str, "MD2", UI_Graph_ADD, 5, UI_Color_Green, 2, 15, 1550, 350, "CHASSIS");
//             break;
//         }
//         ui_display_string(&ui_str);
//         last_chassis = chassis_state;
//         //输出GIMBAL状态
//         switch (gimbal_state)
//         {
//         case 1:
//             string_Draw(&ui_str, "MD3", UI_Graph_ADD, 5, UI_Color_Purplish_red, 2, 15, 1685, 350, "GIMBAL");
//             break;

//         case 2:
//             string_Draw(&ui_str, "MD3", UI_Graph_ADD, 5, UI_Color_Cyan, 2, 15, 1685, 350, "GIMBAL");
//             break;

//         case 3:
//             string_Draw(&ui_str, "MD3", UI_Graph_ADD, 5, UI_Color_Yellow, 2, 15, 1685, 350, "GIMBAL");
//             break;

//         default:
//             string_Draw(&ui_str, "MD3", UI_Graph_ADD, 5, UI_Color_Green, 2, 15, 1685, 350, "GIMBAL");
//             break;
//         }
//         ui_display_string(&ui_str);
//         last_gimbal = gimbal_state;
//         //输出SHOOT状态
//         switch (shoot_state)
//         {
//         case 1:
//             string_Draw(&ui_str, "MD4", UI_Graph_ADD, 5, UI_Color_Purplish_red, 2, 15, 1805, 350, "SHOOT");
//             break;

//         case 2:
//             string_Draw(&ui_str, "MD4", UI_Graph_ADD, 5, UI_Color_Cyan, 2, 15, 1805, 350, "SHOOT");
//             break;

//         case 3:
//             string_Draw(&ui_str, "MD4", UI_Graph_ADD, 5, UI_Color_Yellow, 2, 15, 1805, 350, "SHOOT");
//             break;

//         default:
//             string_Draw(&ui_str, "MD4", UI_Graph_ADD, 5, UI_Color_Green, 2, 15, 1805, 350, "SHOOT");
//             break;
//         }
//         ui_display_string(&ui_str);
//         last_shoot = shoot_state;
//     }
//     else
//     {
//         //输出DBUS状态
//         if (dbus_state != last_dbus || refresh)
//         {
//             string_Draw(&ui_str, "MD1", UI_Graph_Change, 5, dbus_state == 0 ? UI_Color_Green : UI_Color_Purplish_red, 2, 15, 1460, 350, "DBUS");
//             ui_display_string(&ui_str);
//             last_dbus = dbus_state;
//         }
//         //输出CHASSIS状态
//         if (chassis_state != last_chassis || refresh)
//         {
//             switch (chassis_state)
//             {
//             case 1:
//                 string_Draw(&ui_str, "MD2", UI_Graph_Change, 5, UI_Color_Purplish_red, 2, 15, 1550, 350, "CHASSIS");
//                 break;

//             case 2:
//                 string_Draw(&ui_str, "MD2", UI_Graph_Change, 5, UI_Color_Cyan, 2, 15, 1550, 350, "CHASSIS");
//                 break;

//             case 3:
//                 string_Draw(&ui_str, "MD2", UI_Graph_Change, 5, UI_Color_Yellow, 2, 15, 1550, 350, "CHASSIS");
//                 break;

//             default:
//                 string_Draw(&ui_str, "MD2", UI_Graph_Change, 5, UI_Color_Green, 2, 15, 1550, 350, "CHASSIS");
//                 break;
//             }
//             ui_display_string(&ui_str);
//             last_chassis = chassis_state;
//         }
//         //输出GIMBAL状态
//         if (gimbal_state != last_gimbal || refresh)
//         {
//             switch (gimbal_state)
//             {
//             case 1:
//                 string_Draw(&ui_str, "MD3", UI_Graph_Change, 5, UI_Color_Purplish_red, 2, 15, 1685, 350, "GIMBAL");
//                 break;

//             case 2:
//                 string_Draw(&ui_str, "MD3", UI_Graph_Change, 5, UI_Color_Cyan, 2, 15, 1685, 350, "GIMBAL");
//                 break;

//             case 3:
//                 string_Draw(&ui_str, "MD3", UI_Graph_Change, 5, UI_Color_Yellow, 2, 15, 1685, 350, "GIMBAL");
//                 break;

//             default :
//                 string_Draw(&ui_str, "MD3", UI_Graph_Change, 5, UI_Color_Green, 2, 15, 1685, 350, "GIMBAL");
//                 break;
//             }
//             ui_display_string(&ui_str);
//             last_gimbal = gimbal_state;
//         }
//         //输出SHOOT状态
//         if (shoot_state != last_shoot || refresh)
//         {
//             switch (shoot_state)
//             {
//             case 1:
//                 string_Draw(&ui_str, "MD4", UI_Graph_Change, 5, UI_Color_Purplish_red, 2, 15, 1805, 350, "SHOOT");
//                 break;

//             case 2:
//                 string_Draw(&ui_str, "MD4", UI_Graph_Change, 5, UI_Color_Cyan, 2, 15, 1805, 350, "SHOOT");
//                 break;

//             case 3:
//                 string_Draw(&ui_str, "MD4", UI_Graph_Change, 5, UI_Color_Yellow, 2, 15, 1805, 350, "SHOOT");
//                 break;

//             default:
//                 string_Draw(&ui_str, "MD4", UI_Graph_Change, 5, UI_Color_Green, 2, 15, 1805, 350, "SHOOT");
//                 break;
//             }
//             ui_display_string(&ui_str);
//             last_shoot = shoot_state;
//         }
//     }
// }


/**
  * @brief          自定义UI绘制机器人控制模式（画在图层6上）
  * @param[in]      控件存在标志    0：将添加该控件    1：将修改该控件
  * @param[in]      强制刷新使能
  * @param[in]      底盘行为模式
  * @retval         none
  */
// void userUI_draw_robot_control_mode(uint8_t en, uint8_t refresh, chassis_behaviour_e behaviour)
// {
//     switch (behaviour)
//     {
//     case CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW:
//     {
//         string_Draw(&ui_str, "CTM", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 6, UI_Color_Yellow, 3, 20, 650, 155, "CHASSIS_FOLLOW_GIMBAL");
//         break;
//     }

//     case CHASSIS_NO_FOLLOW_YAW:
//     {
//         string_Draw(&ui_str, "CTM", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 6, UI_Color_Yellow, 3, 20, 650, 155, "INDEPENDENCE");
//         break;
//     }

//     case CHASSIS_NO_MOVE:
//     {
//         string_Draw(&ui_str, "CTM", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 6, UI_Color_Cyan, 3, 20, 650, 155, "CHASSIS_NO_MOVE");
//         break;
//     }

//     case CHASSIS_ZERO_FORCE:
//     {
//         string_Draw(&ui_str, "CTM", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 6, UI_Color_Purplish_red, 3, 20, 650, 155, "CHASSIS_ZERO_FORCE");
//         break;
//     }

//     default:
//     {
//         string_Draw(&ui_str, "CTM", en == 0 ? UI_Graph_ADD : UI_Graph_Change, 6, UI_Color_Purplish_red, 3, 20, 650, 155, "__ERROR__");
//         break;
//     }
//     }
//     ui_display_string(&ui_str);
// }
