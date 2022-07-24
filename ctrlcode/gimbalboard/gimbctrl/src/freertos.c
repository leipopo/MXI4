/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for INS_TASK */
osThreadId_t INS_TASKHandle;
const osThreadAttr_t INS_TASK_attributes = {
  .name = "INS_TASK",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for communicatetask */
osThreadId_t communicatetaskHandle;
const osThreadAttr_t communicatetask_attributes = {
  .name = "communicatetask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Shoottask */
osThreadId_t ShoottaskHandle;
const osThreadAttr_t Shoottask_attributes = {
  .name = "Shoottask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for devicemonitor */
osThreadId_t devicemonitorHandle;
const osThreadAttr_t devicemonitor_attributes = {
  .name = "devicemonitor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for infomationupdat */
osThreadId_t infomationupdatHandle;
const osThreadAttr_t infomationupdat_attributes = {
  .name = "infomationupdat",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void INS_task(void *argument);
void comutask(void *argument);
void shoottask(void *argument);
void devmoni(void *argument);
void infoupdate(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of INS_TASK */
  INS_TASKHandle = osThreadNew(INS_task, NULL, &INS_TASK_attributes);

  /* creation of communicatetask */
  communicatetaskHandle = osThreadNew(comutask, NULL, &communicatetask_attributes);

  /* creation of Shoottask */
  ShoottaskHandle = osThreadNew(shoottask, NULL, &Shoottask_attributes);

  /* creation of devicemonitor */
  devicemonitorHandle = osThreadNew(devmoni, NULL, &devicemonitor_attributes);

  /* creation of infomationupdat */
  infomationupdatHandle = osThreadNew(infoupdate, NULL, &infomationupdat_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_INS_task */
/**
  * @brief  Function implementing the INS_TASK thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_INS_task */
__weak void INS_task(void *argument)
{
  /* USER CODE BEGIN INS_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END INS_task */
}

/* USER CODE BEGIN Header_comutask */
/**
* @brief Function implementing the communicatetask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_comutask */
__weak void comutask(void *argument)
{
  /* USER CODE BEGIN comutask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END comutask */
}

/* USER CODE BEGIN Header_shoottask */
/**
* @brief Function implementing the Shoottask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_shoottask */
__weak void shoottask(void *argument)
{
  /* USER CODE BEGIN shoottask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END shoottask */
}

/* USER CODE BEGIN Header_devmoni */
/**
* @brief Function implementing the devicemonitor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_devmoni */
__weak void devmoni(void *argument)
{
  /* USER CODE BEGIN devmoni */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END devmoni */
}

/* USER CODE BEGIN Header_infoupdate */
/**
* @brief Function implementing the infomationupdat thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_infoupdate */
__weak void infoupdate(void *argument)
{
  /* USER CODE BEGIN infoupdate */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END infoupdate */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

