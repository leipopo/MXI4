/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       INS_task.c/h
  * @brief      use bmi088 to calculate the euler angle. no use ist8310, so only
  *             enable data ready pin to save cpu time.enalbe bmi088 data ready
  *             enable spi DMA to save the time spi transmit
  *             主要利用陀螺仪bmi088，磁力计ist8310，完成姿态解算，得出欧拉角，
  *             提供通过bmi088的data ready 中断完成外部触发，减少数据等待延迟
  *             通过DMA的SPI传输节约CPU时间.
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V2.0.0     Nov-11-2019     RM              1. support bmi088, but don't support mpu6500
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "main.h"

#define IMU_temp_PWM(pwm) imu_pwm_set(pwm) // pwm����
/*
#define BMI088_BOARD_INSTALL_SPIN_MATRIX \
    {0.0f, -1.0f, 0.0f},                  \
        {1.0f, 0.0f, 0.0f},             \
    {                                    \
        0.0f, 0.0f, 1.0f                \
    }
#define IST8310_BOARD_INSTALL_SPIN_MATRIX \
    {-1.0f, 0.0f, 0.0f},                   \
        {0.0f, -1.0f, 0.0f},               \
    {                                     \
        0.0f, 0.0f, 1.0f                 \
    }
*/

#define BMI088_BOARD_INSTALL_SPIN_MATRIX \
    {0.0f, 1.0f, 0.0f},                  \
        {-1.0f, 0.0f, 0.0f},             \
    {                                    \
        0.0f, 0.0f, 1.0f                 \
    }

#define IST8310_BOARD_INSTALL_SPIN_MATRIX \
    {1.0f, 0.0f, 0.0f},                   \
        {0.0f, 1.0f, 0.0f},               \
    {                                     \
        0.0f, 0.0f, 1.0f                  \
    }

/**
 * @brief          control the temperature of bmi088
 * @param[in]      temp: the temperature of bmi088
 * @retval         none
 */
/**
 * @brief          控制bmi088的温度
 * @param[in]      temp:bmi088的温度
 * @retval         none
 */
static void imu_temp_control(fp32 temp);
float ysp, psp, rsp;
float bysp, bpsp, brsp;
/**
 * @brief          open the SPI DMA accord to the value of imu_update_flag
 * @param[in]      none
 * @retval         none
 */
/**
 * @brief          根据imu_update_flag的值开启SPI DMA
 * @param[in]      none
 * @retval         none
 */
static void imu_cmd_spi_dma(void);
static void imu_cali_slove(fp32 gyro[3], fp32 accel[3], fp32 mag[3], bmi088_real_data_t *bmi088, ist8310_real_data_t *ist8310);

void AHRS_init(fp32 quat[4], fp32 accel[3], fp32 mag[3]);
void AHRS_update(fp32 quat[4], fp32 time, fp32 gyro[3], fp32 accel[3], fp32 mag[3]);
void get_angle(fp32 quat[4], fp32 *yaw, fp32 *pitch, fp32 *roll);

extern SPI_HandleTypeDef hspi1;

static TaskHandle_t INS_task_local_handler;

uint8_t gyro_dma_rx_buf[SPI_DMA_GYRO_LENGHT];
uint8_t gyro_dma_tx_buf[SPI_DMA_GYRO_LENGHT] = {0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t accel_dma_rx_buf[SPI_DMA_ACCEL_LENGHT];
uint8_t accel_dma_tx_buf[SPI_DMA_ACCEL_LENGHT] = {0x92, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t accel_temp_dma_rx_buf[SPI_DMA_ACCEL_TEMP_LENGHT];
uint8_t accel_temp_dma_tx_buf[SPI_DMA_ACCEL_TEMP_LENGHT] = {0xA2, 0xFF, 0xFF, 0xFF};

volatile uint8_t gyro_update_flag       = 0;
volatile uint8_t accel_update_flag      = 0;
volatile uint8_t accel_temp_update_flag = 0;
volatile uint8_t mag_update_flag        = 0;
volatile uint8_t imu_start_dma_flag     = 0;
static PID_regulator imu_temp_pid;

bmi088_real_data_t bmi088_real_data;
fp32 gyro_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
fp32 gyro_offset[3];
fp32 gyro_cali_offset[3];

fp32 accel_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
fp32 accel_offset[3];
fp32 accel_cali_offset[3];

ist8310_real_data_t ist8310_real_data;
fp32 mag_scale_factor[3][3] = {IST8310_BOARD_INSTALL_SPIN_MATRIX};
fp32 mag_offset[3];
fp32 mag_cali_offset[3];

static uint8_t first_temperate;

static const float timing_time  = 0.001f; // tast run time , unit s.任务运行的时间 单位 s
//加速度计低通滤波
static fp32 accel_fliter_1[3]   = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_2[3]   = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_3[3]   = {0.0f, 0.0f, 0.0f};
static const fp32 fliter_num[3] = {1.929454039488895f, -0.93178349823448126f, 0.002329458745586203f};

fp32 INS_gyro[3]         = {0.0f, 0.0f, 0.0f};
static fp32 INS_accel[3] = {0.0f, 0.0f, 0.0f};
static fp32 INS_mag[3]   = {0.0f, 0.0f, 0.0f};
static fp32 INS_quat[4]  = {0.0f, 0.0f, 0.0f, 0.0f};
fp32 INS_angle[3]        = {0.0f, 0.0f, 0.0f}; // euler angle, unit rad.欧拉角 单位 rad

void instemp_PID_init(PID_regulator *tpid)
{
    tpid->kp             = TEMPERATURE_PID_KP;
    tpid->ki             = TEMPERATURE_PID_KI;
    tpid->kd             = TEMPERATURE_PID_KD;
    tpid->outputMax      = TEMPERATURE_PID_MAX_OUT;
    tpid->componentKiMax = TEMPERATURE_PID_MAX_IOUT;
}
/**
 * @brief          imu task, init bmi088, ist8310, calculate the euler angle
 * @param[in]      pvParameters: NULL
 * @retval         none
 */
/**
 * @brief          imu任务, 初始化 bmi088, ist8310, 计算欧拉角
 * @param[in]      pvParameters: NULL
 * @retval         none
 */
void INS_task()
{

    // wait a time
    osDelay(INS_TASK_INIT_TIME);
    while (BMI088_init())
    {
        osDelay(100);
    }
    while (ist8310_init())
    {
        osDelay(100);
    }

    BMI088_read(bmi088_real_data.gyro, bmi088_real_data.accel, &bmi088_real_data.temp);
    imu_cali_slove(INS_gyro, INS_accel, INS_mag, &bmi088_real_data, &ist8310_real_data);
    instemp_PID_init(&imu_temp_pid);
    AHRS_init(INS_quat, INS_accel, INS_mag);

    accel_fliter_1[0] = accel_fliter_2[0] = accel_fliter_3[0] = INS_accel[0];
    accel_fliter_1[1] = accel_fliter_2[1] = accel_fliter_3[1] = INS_accel[1];
    accel_fliter_1[2] = accel_fliter_2[2] = accel_fliter_3[2] = INS_accel[2];
    // get the handle of task
    //获取当前任务的任务句柄，
    INS_task_local_handler                                    = xTaskGetHandle(pcTaskGetName(NULL));

    // set spi frequency
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }

    SPI1_DMA_init((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);

    imu_start_dma_flag = 1;

    while (1)
    {
        // wait spi DMA tansmit done
        //等待SPI DMA传输
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }

        if (gyro_update_flag & (1 << IMU_NOTIFY_SHFITS))
        {
            gyro_update_flag &= ~(1 << IMU_NOTIFY_SHFITS);
            BMI088_gyro_read_over(gyro_dma_rx_buf + BMI088_GYRO_RX_BUF_DATA_OFFSET, bmi088_real_data.gyro);
        }

        if (accel_update_flag & (1 << IMU_UPDATE_SHFITS))
        {
            accel_update_flag &= ~(1 << IMU_UPDATE_SHFITS);
            BMI088_accel_read_over(accel_dma_rx_buf + BMI088_ACCEL_RX_BUF_DATA_OFFSET, bmi088_real_data.accel, &bmi088_real_data.time);
        }

        if (accel_temp_update_flag & (1 << IMU_UPDATE_SHFITS))
        {
            accel_temp_update_flag &= ~(1 << IMU_UPDATE_SHFITS);
            BMI088_temperature_read_over(accel_temp_dma_rx_buf + BMI088_ACCEL_RX_BUF_DATA_OFFSET, &bmi088_real_data.temp);
            imu_temp_control(bmi088_real_data.temp);
        }

        // rotate and zero drift
        imu_cali_slove(INS_gyro, INS_accel, INS_mag, &bmi088_real_data, &ist8310_real_data);

        //加速度计低通滤波
        // accel low-pass filter
        accel_fliter_1[0] = accel_fliter_2[0];
        accel_fliter_2[0] = accel_fliter_3[0];

        accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + INS_accel[0] * fliter_num[2];

        accel_fliter_1[1] = accel_fliter_2[1];
        accel_fliter_2[1] = accel_fliter_3[1];

        accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] + INS_accel[1] * fliter_num[2];

        accel_fliter_1[2] = accel_fliter_2[2];
        accel_fliter_2[2] = accel_fliter_3[2];

        accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + INS_accel[2] * fliter_num[2];

        AHRS_update(INS_quat, timing_time, INS_gyro, accel_fliter_3, INS_mag);
        get_angle(INS_quat, INS_angle + INS_YAW_ADDRESS_OFFSET, INS_angle + INS_PITCH_ADDRESS_OFFSET, INS_angle + INS_ROLL_ADDRESS_OFFSET);

        // because no use ist8310 and save time, no use
        if (mag_update_flag &= 1 << IMU_DR_SHFITS)
        {
            mag_update_flag &= ~(1 << IMU_DR_SHFITS);
            mag_update_flag |= (1 << IMU_SPI_SHFITS);
            //            ist8310_read_mag(ist8310_real_data.mag);
        }
    }
}

/**
 * @brief          rotate the gyro, accel and mag, and calculate the zero drift, because sensors have
 *                 different install derection.
 * @param[out]     gyro: after plus zero drift and rotate
 * @param[out]     accel: after plus zero drift and rotate
 * @param[out]     mag: after plus zero drift and rotate
 * @param[in]      bmi088: gyro and accel data
 * @param[in]      ist8310: mag data
 * @retval         none
 */
/**
 * @brief          ??��???????,????????????????,??????????,?��???����???????��?���㡤???
 * @param[out]     gyro: ????????????��?
 * @param[out]     accel: ????????????��?
 * @param[out]     mag: ????????????��?
 * @param[in]      bmi088: ????????????????????
 * @param[in]      ist8310: ??????????
 * @retval         none
 */
static void imu_cali_slove(fp32 gyro[3], fp32 accel[3], fp32 mag[3], bmi088_real_data_t *bmi088, ist8310_real_data_t *ist8310)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        gyro[i]  = bmi088->gyro[0] * gyro_scale_factor[i][0] + bmi088->gyro[1] * gyro_scale_factor[i][1] + bmi088->gyro[2] * gyro_scale_factor[i][2] + gyro_offset[i];
        accel[i] = bmi088->accel[0] * accel_scale_factor[i][0] + bmi088->accel[1] * accel_scale_factor[i][1] + bmi088->accel[2] * accel_scale_factor[i][2] + accel_offset[i];
        mag[i]   = ist8310->mag[0] * mag_scale_factor[i][0] + ist8310->mag[1] * mag_scale_factor[i][1] + ist8310->mag[2] * mag_scale_factor[i][2] + mag_offset[i];
    }
}

void AHRS_init(fp32 quat[4], fp32 accel[3], fp32 mag[3])
{
    quat[0] = 1.0f;
    quat[1] = 0.0f;
    quat[2] = 0.0f;
    quat[3] = 0.0f;
}

void AHRS_update(fp32 quat[4], fp32 time, fp32 gyro[3], fp32 accel[3], fp32 mag[3])
{
    MahonyAHRSupdate(quat, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], 0.f, 0.f, 0.f);
}
void get_angle(fp32 q[4], fp32 *yaw, fp32 *pitch, fp32 *roll)
{
    *yaw   = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 2.0f * (q[0] * q[0] + q[1] * q[1]) - 1.0f);
    *pitch = asinf(-2.0f * (q[1] * q[3] - q[0] * q[2]));
    *roll  = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 2.0f * (q[0] * q[0] + q[3] * q[3]) - 1.0f);
}

/**
 * @brief          control the temperature of bmi088
 * @param[in]      temp: the temperature of bmi088
 * @retval         none
 */
/**
 * @brief          ����bmi088���¶�
 * @param[in]      temp:bmi088���¶�
 * @retval         none
 */
static void imu_temp_control(fp32 temp)
{
    uint16_t tempPWM;
    static uint8_t temp_constant_time = 0;
    if (first_temperate)
    {
        PID_calc(&imu_temp_pid, 1);
        if (imu_temp_pid.output < 0.0f)
        {
            imu_temp_pid.output = 0.0f;
        }
        tempPWM = (uint16_t)imu_temp_pid.output;
        IMU_temp_PWM(tempPWM);
    }
    else
    {
        //��û�дﵽ���õ��¶ȣ�һֱ����ʼ���
        // in beginning, max power
        if (temp > 35.0f)
        {
            temp_constant_time++;
            if (temp_constant_time > 200)
            {
                //�ﵽ�����¶ȣ�������������Ϊһ������ʣ���������
                //
                first_temperate          = 1;
                imu_temp_pid.componentKi = MPU6500_TEMP_PWM_MAX / 2.0f;
            }
        }

        IMU_temp_PWM(MPU6500_TEMP_PWM_MAX - 1);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == INT1_ACCEL_Pin)
    {
        accel_update_flag |= 1 << IMU_DR_SHFITS;
        accel_temp_update_flag |= 1 << IMU_DR_SHFITS;
        if (imu_start_dma_flag)
        {
            imu_cmd_spi_dma();
        }
    }
    else if (GPIO_Pin == INT1_GYRO_Pin)
    {
        gyro_update_flag |= 1 << IMU_DR_SHFITS;
        if (imu_start_dma_flag)
        {
            imu_cmd_spi_dma();
        }
    }
    else if (GPIO_Pin == DRDY_IST8310_Pin)
    {
        mag_update_flag |= 1 << IMU_DR_SHFITS;

        if (mag_update_flag &= 1 << IMU_DR_SHFITS)
        {
            mag_update_flag &= ~(1 << IMU_DR_SHFITS);
            mag_update_flag |= (1 << IMU_SPI_SHFITS);

            ist8310_read_mag(ist8310_real_data.mag);
        }
    }
    else if (GPIO_Pin == GPIO_PIN_0)
    {
        // wake up the task
        //��������
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
        {
            static BaseType_t xHigherPriorityTaskWoken;
            vTaskNotifyGiveFromISR(INS_task_local_handler, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

/**
 * @brief          open the SPI DMA accord to the value of imu_update_flag
 * @param[in]      none
 * @retval         none
 */
/**
 * @brief          ����imu_update_flag��ֵ����SPI DMA
 * @param[in]      temp:bmi088���¶�
 * @retval         none
 */
static void imu_cmd_spi_dma(void)
{
    UBaseType_t uxSavedInterruptStatus;
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    //���������ǵ�DMA����
    if ((gyro_update_flag & (1 << IMU_DR_SHFITS)) && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN) && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN) && !(accel_update_flag & (1 << IMU_SPI_SHFITS)) && !(accel_temp_update_flag & (1 << IMU_SPI_SHFITS)))
    {
        gyro_update_flag &= ~(1 << IMU_DR_SHFITS);
        gyro_update_flag |= (1 << IMU_SPI_SHFITS);

        HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
        SPI1_DMA_enable((uint32_t)gyro_dma_tx_buf, (uint32_t)gyro_dma_rx_buf, SPI_DMA_GYRO_LENGHT);
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return;
    }
    //�������ٶȼƵ�DMA����
    if ((accel_update_flag & (1 << IMU_DR_SHFITS)) && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN) && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN) && !(gyro_update_flag & (1 << IMU_SPI_SHFITS)) && !(accel_temp_update_flag & (1 << IMU_SPI_SHFITS)))
    {
        accel_update_flag &= ~(1 << IMU_DR_SHFITS);
        accel_update_flag |= (1 << IMU_SPI_SHFITS);

        HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
        SPI1_DMA_enable((uint32_t)accel_dma_tx_buf, (uint32_t)accel_dma_rx_buf, SPI_DMA_ACCEL_LENGHT);
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return;
    }

    if ((accel_temp_update_flag & (1 << IMU_DR_SHFITS)) && !(hspi1.hdmatx->Instance->CR & DMA_SxCR_EN) && !(hspi1.hdmarx->Instance->CR & DMA_SxCR_EN) && !(gyro_update_flag & (1 << IMU_SPI_SHFITS)) && !(accel_update_flag & (1 << IMU_SPI_SHFITS)))
    {
        accel_temp_update_flag &= ~(1 << IMU_DR_SHFITS);
        accel_temp_update_flag |= (1 << IMU_SPI_SHFITS);

        HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
        SPI1_DMA_enable((uint32_t)accel_temp_dma_tx_buf, (uint32_t)accel_temp_dma_rx_buf, SPI_DMA_ACCEL_TEMP_LENGHT);
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        return;
    }
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

void DMA2_Stream2_IRQHandler(void)
{

    if (__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx));

        // gyro read over
        //�����Ƕ�ȡ���
        if (gyro_update_flag & (1 << IMU_SPI_SHFITS))
        {
            gyro_update_flag &= ~(1 << IMU_SPI_SHFITS);
            gyro_update_flag |= (1 << IMU_UPDATE_SHFITS);

            HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
        }

        // accel read over
        //���ٶȼƶ�ȡ���
        if (accel_update_flag & (1 << IMU_SPI_SHFITS))
        {
            accel_update_flag &= ~(1 << IMU_SPI_SHFITS);
            accel_update_flag |= (1 << IMU_UPDATE_SHFITS);

            HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
        }
        // temperature read over
        //�¶ȶ�ȡ���
        if (accel_temp_update_flag & (1 << IMU_SPI_SHFITS))
        {
            accel_temp_update_flag &= ~(1 << IMU_SPI_SHFITS);
            accel_temp_update_flag |= (1 << IMU_UPDATE_SHFITS);

            HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
        }

        imu_cmd_spi_dma();

        if (gyro_update_flag & (1 << IMU_UPDATE_SHFITS))
        {
            gyro_update_flag &= ~(1 << IMU_UPDATE_SHFITS);
            gyro_update_flag |= (1 << IMU_NOTIFY_SHFITS);
            __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_0);
        }
    }
}
/**
 * @brief          calculate gyro zero drift
 * @param[out]     gyro_offset:zero drift
 * @param[in]      gyro:gyro data
 * @param[out]     offset_time_count: +1 auto
 * @retval         none
 */
/**
 * @brief          ??????????????
 * @param[out]     gyro_offset:????????
 * @param[in]      gyro:??????????
 * @param[out]     offset_time_count: ��?????1
 * @retval         none
 */
void gyro_offset_calc(fp32 gyro_offset[3], fp32 gyro[3], uint16_t *offset_time_count)
{
    if (gyro_offset == NULL || gyro == NULL || offset_time_count == NULL)
    {
        return;
    }

    gyro_offset[0] = gyro_offset[0] - 0.0003f * gyro[0];
    gyro_offset[1] = gyro_offset[1] - 0.0003f * gyro[1];
    gyro_offset[2] = gyro_offset[2] - 0.0003f * gyro[2];
    (*offset_time_count)++;
}

/**
 * @brief          calculate gyro zero drift
 * @param[out]     cali_scale:scale, default 1.0
 * @param[out]     cali_offset:zero drift, collect the gyro ouput when in still
 * @param[out]     time_count: time, when call gyro_offset_calc
 * @retval         none
 */
/**
 * @brief          ??��???????
 * @param[out]     ????????��????����???1.0f????????????????
 * @param[out]     ????????????????????????????????????����??offset
 * @param[out]     ?????????��??????????gyro_offset?��???��??1,
 * @retval         none
 */
void INS_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3], uint16_t *time_count)
{
    if (*time_count == 0)
    {
        gyro_offset[0] = gyro_cali_offset[0];
        gyro_offset[1] = gyro_cali_offset[1];
        gyro_offset[2] = gyro_cali_offset[2];
    }
    gyro_offset_calc(gyro_offset, INS_gyro, time_count);

    cali_offset[0] = gyro_offset[0];
    cali_offset[1] = gyro_offset[1];
    cali_offset[2] = gyro_offset[2];
    cali_scale[0]  = 1.0f;
    cali_scale[1]  = 1.0f;
    cali_scale[2]  = 1.0f;
}

/**
 * @brief          get gyro zero drift from flash
 * @param[in]      cali_scale:scale, default 1.0
 * @param[in]      cali_offset:zero drift,
 * @retval         none
 */
/**
 * @brief          ??��????????��????????flash?��????????��???????��???
 * @param[in]      ????????��????����???1.0f????????????????
 * @param[in]      ????????????
 * @retval         none
 */
void INS_set_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3])
{
    gyro_cali_offset[0] = cali_offset[0];
    gyro_cali_offset[1] = cali_offset[1];
    gyro_cali_offset[2] = cali_offset[2];
    gyro_offset[0]      = gyro_cali_offset[0];
    gyro_offset[1]      = gyro_cali_offset[1];
    gyro_offset[2]      = gyro_cali_offset[2];
}

/**
 * @brief          get the quat
 * @param[in]      none
 * @retval         the point of INS_quat
 */
/**
 * @brief          ??????????
 * @param[in]      none
 * @retval         INS_quat??????
 */
const fp32 *get_INS_quat_point(void)
{
    return INS_quat;
}
/**
 * @brief          get the euler angle, 0:yaw, 1:pitch, 2:roll unit rad
 * @param[in]      none
 * @retval         the point of INS_angle
 */
/**
 * @brief          ?????��????, 0:yaw, 1:pitch, 2:roll ???? rad
 * @param[in]      none
 * @retval         INS_angle??????
 */
const fp32 *get_INS_angle_point(void)
{
    return INS_angle;
}

/**
 * @brief          get the rotation speed, 0:x-axis, 1:y-axis, 2:roll-axis,unit rad/s
 * @param[in]      none
 * @retval         the point of INS_gyro
 */
/**
 * @brief          ??????????,0:x?��, 1:y?��, 2:roll?�� ???? rad/s
 * @param[in]      none
 * @retval         INS_gyro??????
 */
extern const fp32 *get_gyro_data_point(void)
{
    return INS_gyro;
}
/**
 * @brief          get aceel, 0:x-axis, 1:y-axis, 2:roll-axis unit m/s2
 * @param[in]      none
 * @retval         the point of INS_accel
 */
/**
 * @brief          ??????????,0:x?��, 1:y?��, 2:roll?�� ???? m/s2
 * @param[in]      none
 * @retval         INS_accel??????
 */
extern const fp32 *get_accel_data_point(void)
{
    return INS_accel;
}
/**
 * @brief          get mag, 0:x-axis, 1:y-axis, 2:roll-axis unit ut
 * @param[in]      none
 * @retval         the point of INS_mag
 */
/**
 * @brief          ??????????,0:x?��, 1:y?��, 2:roll?�� ???? ut
 * @param[in]      none
 * @retval         INS_mag??????
 */
extern const fp32 *get_mag_data_point(void)
{
    return INS_mag;
}
