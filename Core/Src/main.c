/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    uint32_t len;
    uint32_t wid;
    uint32_t hei;
    uint32_t wei;
} properties_t;

typedef struct {
    double vol;
    double amp;
    double res;
} msg_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myBuyTask */
osThreadId_t myBuyTaskHandle;
const osThreadAttr_t myBuyTask_attributes = {
    .name = "myBuyTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mySellTask */
osThreadId_t mySellTaskHandle;
const osThreadAttr_t mySellTask_attributes = {
    .name = "mySellTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mySendEvent */
osThreadId_t mySendEventHandle;
const osThreadAttr_t mySendEvent_attributes = {
    .name = "mySendEvent",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myRcvEvent */
osThreadId_t myRcvEventHandle;
const osThreadAttr_t myRcvEvent_attributes = {
    .name = "myRcvEvent",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myPoolTask01 */
osThreadId_t myPoolTask01Handle;
const osThreadAttr_t myPoolTask01_attributes = {
    .name = "myPoolTask01",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myPoolTask02 */
osThreadId_t myPoolTask02Handle;
const osThreadAttr_t myPoolTask02_attributes = {
    .name = "myPoolTask02",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myMailSendTask */
osThreadId_t myMailSendTaskHandle;
const osThreadAttr_t myMailSendTask_attributes = {
    .name = "myMailSendTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myMailRcvTask */
osThreadId_t myMailRcvTaskHandle;
const osThreadAttr_t myMailRcvTask_attributes = {
    .name = "myMailRcvTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mySemaphore */
osSemaphoreId_t mySemaphoreHandle;
const osSemaphoreAttr_t mySemaphore_attributes = {
    .name = "mySemaphore"
};
/* Definitions for SendFlag */
osEventFlagsId_t SendFlagHandle;
const osEventFlagsAttr_t SendFlag_attributes = {
    .name = "SendFlag"
};
/* USER CODE BEGIN PV */
uint8_t g_iUartRx;
int g_iBuyFlag;
int g_iTicket;
int g_iSendFlag;

properties_t  *properties;
osMemoryPoolId_t poolHandle;

osMessageQueueId_t msgHandle;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);
void BuyTask(void *argument);
void SellTask(void *argument);
void SendEvent(void *argument);
void RcvEvent(void *argument);
void PoolTask01(void *argument);
void PoolTask02(void *argument);
void MailSendTask(void *argument);
void MailRcvTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
* @brief  The application entry point.
* @retval int
*/
int main(void)
{
    /* USER CODE BEGIN 1 */
    
    /* USER CODE END 1 */
    
    /* MCU Configuration--------------------------------------------------------*/
    
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    
    /* USER CODE BEGIN Init */
    
    /* USER CODE END Init */
    
    /* Configure the system clock */
    SystemClock_Config();
    
    /* USER CODE BEGIN SysInit */
    
    /* USER CODE END SysInit */
    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    g_iBuyFlag = 0;
    g_iSendFlag = 0;
    g_iTicket = 100000;
    char buf[32];
    sprintf(buf, "USART Test\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
    HAL_UART_Receive_IT(&huart1, &g_iUartRx, sizeof(g_iUartRx));
    
    poolHandle = osMemoryPoolNew(16, sizeof(properties_t), NULL);
    
    /* USER CODE END 2 */
    
    /* Init scheduler */
    osKernelInitialize();
    
    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */
    
    /* Create the semaphores(s) */
    /* creation of mySemaphore */
    mySemaphoreHandle = osSemaphoreNew(1, 1, &mySemaphore_attributes);
    
    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */
    
    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */
    
    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    msgHandle = osMessageQueueNew(16, sizeof(msg_t), NULL);
    /* USER CODE END RTOS_QUEUES */
    
    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
    
    /* creation of myBuyTask */
    myBuyTaskHandle = osThreadNew(BuyTask, (void *)mySemaphoreHandle, &myBuyTask_attributes);
    
    /* creation of mySellTask */
    mySellTaskHandle = osThreadNew(SellTask, (void *)mySemaphoreHandle, &mySellTask_attributes);
    
    /* creation of mySendEvent */
    mySendEventHandle = osThreadNew(SendEvent, NULL, &mySendEvent_attributes);
    
    /* creation of myRcvEvent */
    myRcvEventHandle = osThreadNew(RcvEvent, NULL, &myRcvEvent_attributes);
    
    /* creation of myPoolTask01 */
    myPoolTask01Handle = osThreadNew(PoolTask01, (void *)properties, &myPoolTask01_attributes);
    
    /* creation of myPoolTask02 */
    myPoolTask02Handle = osThreadNew(PoolTask02, (void *)properties, &myPoolTask02_attributes);
    
    /* creation of myMailSendTask */
    myMailSendTaskHandle = osThreadNew(MailSendTask, NULL, &myMailSendTask_attributes);
    
    /* creation of myMailRcvTask */
    myMailRcvTaskHandle = osThreadNew(MailRcvTask, NULL, &myMailRcvTask_attributes);
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
    
    /* Create the event(s) */
    /* creation of SendFlag */
    SendFlagHandle = osEventFlagsNew(&SendFlag_attributes);
    
    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    /* Start scheduler */
    osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */
        
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /** Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    
    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_0;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
            |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
* @brief USART1 Initialization Function
* @param None
* @retval None
*/
static void MX_USART1_UART_Init(void)
{
    
    /* USER CODE BEGIN USART1_Init 0 */
    
    /* USER CODE END USART1_Init 0 */
    
    /* USER CODE BEGIN USART1_Init 1 */
    
    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */
    
    /* USER CODE END USART1_Init 2 */
    
}

/**
* @brief GPIO Initialization Function
* @param None
* @retval None
*/
static void MX_GPIO_Init(void)
{
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    g_iBuyFlag = 1;
    if(g_iUartRx == '1')
    {
        g_iSendFlag = 1;
    }
    HAL_UART_Receive_IT(&huart1, &g_iUartRx, sizeof(g_iUartRx));
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief  Function implementing the defaultTask thread.
* @param  argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_BuyTask */
/**
* @brief Function implementing the myBuyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BuyTask */
void BuyTask(void *argument)
{
    /* USER CODE BEGIN BuyTask */
    osSemaphoreId semaphore = (osSemaphoreId)argument;
    /* Infinite loop */
    for(;;)
    {
        if(g_iBuyFlag == 1)
        {
            if(semaphore != NULL)
            {
                if(osSemaphoreAcquire(semaphore, 0) == osOK)
                {
                    g_iTicket--;
                    g_iBuyFlag = 0;
                }
                osSemaphoreRelease(semaphore);
            }
        }
        
        osDelay(100);
    }
    /* USER CODE END BuyTask */
}

/* USER CODE BEGIN Header_SellTask */
/**
* @brief Function implementing the mySellTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SellTask */
void SellTask(void *argument)
{
    /* USER CODE BEGIN SellTask */
    osSemaphoreId semaphore = (osSemaphoreId)argument;
    char buf[16];
    
    /* Infinite loop */
    for(;;)
    {
        if(semaphore != NULL)
        {
            if(osSemaphoreAcquire(semaphore, 0) == osOK)
            {
                sprintf(buf, "%d\r\n", g_iTicket);
                HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 10);
            }
            osSemaphoreRelease(semaphore);
        }
        osDelay(500);
    }
    /* USER CODE END SellTask */
}

/* USER CODE BEGIN Header_SendEvent */
/**
* @brief Function implementing the mySendEvent thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendEvent */
void SendEvent(void *argument)
{
    /* USER CODE BEGIN SendEvent */
    
    /* Infinite loop */
    for(;;)
    {
        if(g_iSendFlag == 1)
        {
            osEventFlagsSet(SendFlagHandle, 1);
            g_iSendFlag = 0;
        }
        osDelay(100);
    }
    /* USER CODE END SendEvent */
}

/* USER CODE BEGIN Header_RcvEvent */
/**
* @brief Function implementing the myRcvEvent thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RcvEvent */
void RcvEvent(void *argument)
{
    /* USER CODE BEGIN RcvEvent */
    uint32_t flags;
    char buf[32];
    
    /* Infinite loop */
    for(;;)
    {
        //        flags = osEventFlagsWait(SendFlagHandle, 1, osFlagsWaitAny, osWaitForever);
        flags = osEventFlagsWait(SendFlagHandle, 1, osFlagsWaitAny, 1000);
        if(flags == 1)
        {
            sprintf(buf, "Rcv Cplt\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
        }
        else
        {
            sprintf(buf, "TimeOut\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
        }
        osDelay(100);
    }
    /* USER CODE END RcvEvent */
}

/* USER CODE BEGIN Header_PoolTask01 */
/**
* @brief Function implementing the myPoolTask01 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PoolTask01 */
void PoolTask01(void *argument)
{
    /* USER CODE BEGIN PoolTask01 */
    properties_t *arg_properties = (properties_t *)argument;
    arg_properties[0].hei = 0;
    arg_properties[0].len = 10;
    arg_properties[0].wei = 100;
    arg_properties[0].wid = 1000;
    char buf[64];
    
    /* Infinite loop */
    for(;;)
    {
        arg_properties[0].hei++;
        arg_properties[0].len++;
        arg_properties[0].wei++;
        arg_properties[0].wid++;
        
        sprintf(buf, "PoolTask01 : (%d, %d, %d, %d)\r\n", arg_properties[0].hei, arg_properties[0].len, arg_properties[0].wei, arg_properties[0].wid);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, strlen(buf), 100);
        
        osDelay(500);
    }
    /* USER CODE END PoolTask01 */
}

/* USER CODE BEGIN Header_PoolTask02 */
/**
* @brief Function implementing the myPoolTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PoolTask02 */
void PoolTask02(void *argument)
{
    /* USER CODE BEGIN PoolTask02 */
    properties_t *arg_properties = (properties_t *)argument;
    char buf[64];
    /* Infinite loop */
    for(;;)
    {
        arg_properties[0].hei++;
        arg_properties[0].len++;
        arg_properties[0].wei++;
        arg_properties[0].wid++;
        
        sprintf(buf, "PoolTask02 : (%d, %d, %d, %d)\r\n", arg_properties[0].hei, arg_properties[0].len, arg_properties[0].wei, arg_properties[0].wid);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, strlen(buf), 100);
        
        osDelay(800);
    }
    /* USER CODE END PoolTask02 */
}

/* USER CODE BEGIN Header_MailSendTask */
/**
* @brief Function implementing the myMailSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MailSendTask */
void MailSendTask(void *argument)
{
    /* USER CODE BEGIN MailSendTask */
    msg_t msg;
    msg.vol = 0.0;
    msg.amp = 0.0;
    msg.res = 0.0;
    int i = 30;
    /* Infinite loop */
    for(;;)
    {
        msg.vol = 3.3 * i;
        msg.amp = 20.0;
        msg.res = msg.vol / msg.amp;
        osMessageQueuePut(msgHandle, &msg, 0, 0);
        osDelay(300);
        i++;
    }
    /* USER CODE END MailSendTask */
}

/* USER CODE BEGIN Header_MailRcvTask */
/**
* @brief Function implementing the myMailRcvTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MailRcvTask */
void MailRcvTask(void *argument)
{
    /* USER CODE BEGIN MailRcvTask */
    msg_t msg;
    osStatus_t status;
    char buf[48];
    /* Infinite loop */
    for(;;)
    {
        status = osMessageQueueGet(msgHandle, &msg, 0, 0);
        if(status == osOK)
        {
            sprintf(buf, "Vol : %.1f, Amp : %.1f, Res : %.1f\r\n", msg.vol, msg.amp, msg.res);
            HAL_UART_Transmit(&huart1, (uint8_t *)buf, strlen(buf), 100);
        }
        osDelay(1);
    }
    /* USER CODE END MailRcvTask */
}

/**
* @brief  Period elapsed callback in non blocking mode
* @note   This function is called  when TIM7 interrupt took place, inside
* HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
* a global variable "uwTick" used as application time base.
* @param  htim : TIM handle
* @retval None
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */
    
    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */
    
    /* USER CODE END Callback 1 */
}

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
