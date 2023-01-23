/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <task.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 SemaphoreHandle_t xSemaphore1;
 QueueHandle_t xQueue1;
 TaskHandle_t xHandle3 = NULL;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	int i=100;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch){
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
/*void vTaskLED( void * pvParameters )
  {
      for( ;; )
      {
		          HAL_GPIO_TogglePin(GPIOI, LED_Pin);
	     //       printf("test");
		          vTaskDelay(100);
      }
  }*/

void vTaskTake( void * pvParameters )
  {
	  char RxTimerBuff[100];
      for( ;; )
      {
    	 /* printf("Task va prendre le semaphore\r\n");
    	 // xSemaphoreTake(xSemaphore1 ,portMAX_DELAY );
    		printf("Semaphore pris: \r\n");*/
    	  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    		if(xQueueReceive(xQueue1,(void*)RxTimerBuff, (TickType_t)5))
    		{
    	printf("timer: %s\r\n", RxTimerBuff);
    		}

      }
  }

void vTaskGive( void * pvParameters )
  {

	char TxTimerBuff[100];
	xQueue1 = xQueueCreate(10, sizeof(TxTimerBuff));
      for( ;; )
      {		  /*printf("Task va donner le semaphore\r\n");

    	  	  //xSemaphoreGive( xSemaphore1 );
    	  	  printf("Semaphore donné\r\n");*/
		     // printf("i= %d",i);

    	  sprintf(TxTimerBuff, "%d",i);
    	  xQueueSend(xQueue1, (void*) TxTimerBuff,(TickType_t) 0);
       	  xTaskNotifyGive(xHandle3);


    	  vTaskDelay(i);
    	  i=i+100;
    	// printf("%d\r\n",i);
		      if(i>1000){
		    	  printf("reset\r\n");
		    	  NVIC_SystemReset();
		      }

      }
  }




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    BaseType_t xReturned;
//  TaskHandle_t xHandle1 = NULL;
    TaskHandle_t xHandle2 = NULL;

    //SemaphoreHandle_t xSemaphore1;
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



        /* Create the task, storing the handle. */
   // xReturned = xTaskCreate(
     //                    vTaskLED,       /* Function that implements the task. */
       //                  "LED_BLINK",          /* Text name for the task. */
         //                128,      /* Stack size in words, not bytes. */
           ///              ( void * ) 1,    /* Parameter passed into the task. */
              //           1,/* Priority at which the task is created. */
                //         &xHandle1 );

    xReturned = xTaskCreate(
                             vTaskGive,       /* Function that implements the task. */
                             "taskGive",          /* Text name for the task. */
                             128,      /* Stack size in words, not bytes. */
                             ( void * ) 1,    /* Parameter passed into the task. */
                             3,/* Priority at which the task is created. */
                             &xHandle2 );

    xReturned = xTaskCreate(
                             vTaskTake,       /* Function that implements the task. */
                             "taskTake",          /* Text name for the task. */
                             128,      /* Stack size in words, not bytes. */
                             ( void * ) 1,    /* Parameter passed into the task. */
                             2,/* Priority at which the task is created. */
                             &xHandle3 );

    xSemaphore1 = xSemaphoreCreateBinary();


    vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//test

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
