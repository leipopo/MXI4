/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
/* Definitions for refereeread */
osThreadId_t refereereadHandle;
const osThreadAttr_t refereeread_attributes = {
  .name = "refereeread",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for devmonitor */
osThreadId_t devmonitorHandle;
const osThreadAttr_t devmonitor_attributes = {
  .name = "devmonitor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for gimbalcontrol */
osThreadId_t gimbalcontrolHandle;
const osThreadAttr_t gimbalcontrol_attributes = {
  .name = "gimbalcontrol",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for chassiscontrol */
osThreadId_t chassiscontrolHandle;
const osThreadAttr_t chassiscontrol_attributes = {
  .name = "chassiscontrol",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for boardcommunicat */
osThreadId_t boardcommunicatHandle;
const osThreadAttr_t boardcommunicat_attributes = {
  .name = "boardcommunicat",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UIdraw */
osThreadId_t UIdrawHandle;
const osThreadAttr_t UIdraw_attributes = {
  .name = "UIdraw",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for capsctrol */
osThreadId_t capsctrolHandle;
const osThreadAttr_t capsctrol_attributes = {
  .name = "capsctrol",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for roboinfoupdate */
osThreadId_t roboinfoupdateHandle;
const osThreadAttr_t roboinfoupdate_attributes = {
  .name = "roboinfoupdate",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void reftask(void *argument);
void devmoni(void *argument);
void gimbctrl(void *argument);
void chasctrl(void *argument);
void comutask(void *argument);
void UIdrawer(void *argument);
void capsctrl(void *argument);
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
  /* creation of refereeread */
  refereereadHandle = osThreadNew(reftask, NULL, &refereeread_attributes);

  /* creation of devmonitor */
  devmonitorHandle = osThreadNew(devmoni, NULL, &devmonitor_attributes);

  /* creation of gimbalcontrol */
  gimbalcontrolHandle = osThreadNew(gimbctrl, NULL, &gimbalcontrol_attributes);

  /* creation of chassiscontrol */
  chassiscontrolHandle = osThreadNew(chasctrl, NULL, &chassiscontrol_attributes);

  /* creation of boardcommunicat */
  boardcommunicatHandle = osThreadNew(comutask, NULL, &boardcommunicat_attributes);

  /* creation of UIdraw */
  UIdrawHandle = osThreadNew(UIdrawer, NULL, &UIdraw_attributes);

  /* creation of capsctrol */
  capsctrolHandle = osThreadNew(capsctrl, NULL, &capsctrol_attributes);

  /* creation of roboinfoupdate */
  roboinfoupdateHandle = osThreadNew(infoupdate, NULL, &roboinfoupdate_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_reftask */
/**
 * @brief  Function implementing the refereeread thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_reftask */
__weak void reftask(void *argument)
{
  /* USER CODE BEGIN reftask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END reftask */
}

/* USER CODE BEGIN Header_devmoni */
/**
 * @brief Function implementing the devmonitor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_devmoni */
__weak void devmoni(void *argument)
{
  /* USER CODE BEGIN devmoni */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END devmoni */
}

/* USER CODE BEGIN Header_gimbctrl */
/**
 * @brief Function implementing the gimbalcontrol thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_gimbctrl */
__weak void gimbctrl(void *argument)
{
  /* USER CODE BEGIN gimbctrl */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END gimbctrl */
}

/* USER CODE BEGIN Header_chasctrl */
/**
 * @brief Function implementing the chassiscontrol thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_chasctrl */
__weak void chasctrl(void *argument)
{
  /* USER CODE BEGIN chasctrl */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END chasctrl */
}

/* USER CODE BEGIN Header_comutask */
/**
 * @brief Function implementing the boardcommunicat thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_comutask */
__weak void comutask(void *argument)
{
  /* USER CODE BEGIN comutask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END comutask */
}

/* USER CODE BEGIN Header_UIdrawer */
/**
 * @brief Function implementing the UIdraw thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_UIdrawer */
__weak void UIdrawer(void *argument)
{
  /* USER CODE BEGIN UIdrawer */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END UIdrawer */
}

/* USER CODE BEGIN Header_capsctrl */
/**
 * @brief Function implementing the capsctrol thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_capsctrl */
__weak void capsctrl(void *argument)
{
  /* USER CODE BEGIN capsctrl */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END capsctrl */
}

/* USER CODE BEGIN Header_infoupdate */
/**
* @brief Function implementing the roboinfoupdate thread.
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

