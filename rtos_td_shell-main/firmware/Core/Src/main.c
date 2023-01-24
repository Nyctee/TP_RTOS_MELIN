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
#include "shell.h"
#include "FreeRTOS.h"
//#include <task.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
SemaphoreHandle_t xSemaphoreUart;
SemaphoreHandle_t xSemaphoreLed;
SemaphoreHandle_t xSemaphoreSpam;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int argT=0;
int nombre=0;
char* mot;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

	return ch;
}

int fonction(int argc, char ** argv)
{
	printf("Je suis une fonction bidon\r\n");
	printf("argc = %d\r\n", argc);

		for (int i = 0 ; i < argc ; i++)
		{
			printf("arg %d = %s\r\n", i, argv[i]);
		}
	return 0;
}

int fonctionLed(int argc, char ** argv)
{
	argT=atoi(argv[1]);
	printf("fonction led\r\n");
	printf("argc = %d\r\n", argc);
	printf("argT = %d\r\n", argT);
	if(argT==0){
		printf("yo");
		HAL_GPIO_WritePin(GPIOI, LED_Pin, RESET);
	}
	else {
		xSemaphoreGive(xSemaphoreLed);
	}
	return 0;
}

int fonctionSpam(int argc, char ** argv)
{
	mot=argv[1];
	nombre=atoi(argv[2]);
 printf("fonction Spam: mot nombre\r\n");
		xSemaphoreGive(xSemaphoreSpam);
		return 0;
}



void vTaskShell( void * pvParameters )
  {
	shell_init();
	shell_add('f', fonction, "Une fonction inutile");
	shell_add('l',fonctionLed,"fonction led");
	shell_add('s',fonctionSpam,"fonction spam");
	shell_run();
  }


void vTaskLed( void * pvParameters )
  {
	xSemaphoreTake(xSemaphoreLed ,portMAX_DELAY);
	for(;;){
		if(argT==0){HAL_GPIO_WritePin(GPIOI, LED_Pin, RESET);}
		else{
	HAL_GPIO_TogglePin(GPIOI, LED_Pin);
	vTaskDelay((1000/argT)/2);
	HAL_GPIO_TogglePin(GPIOI, LED_Pin);
	vTaskDelay((1000/argT)/2);
		}
	}
  }

void vTaskSpam( void * pvParameters )
  {
	xSemaphoreTake(xSemaphoreSpam ,portMAX_DELAY);
	int i;
	for(i=0;i<nombre;i++){
		printf("%s\r\n",mot);
		vTaskDelay(100);
	}
  }

void vApplicationStackOverflowHook( TaskHandle_t  xTask,
                                    signed char *pcTaskName ){
	printf("stack_overflow\r\n");
}

//void vTaskBidon1( void * pvParameters )
//  {
//	//int a[100000000];
//  }
//
//
//void vTaskBidon2( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//
//void vTaskBidon3( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//
//void vTaskBidon4( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//
//void vTaskBidon5( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//void vTaskBidon6( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//void vTaskBidon7( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//void vTaskBidon8( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//void vTaskBidon9( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }
//
//void vTaskBidon10( void * pvParameters )
//  {
//	printf("Je suis une fonction bidon1\r\n");
//  }




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 BaseType_t xReturned;
	TaskHandle_t xHandle1 = NULL;
	TaskHandle_t xHandle2 = NULL;
	TaskHandle_t xHandle3 = NULL;
//	TaskHandle_t xHandle4 = NULL;
//	TaskHandle_t xHandle5 = NULL;
//	TaskHandle_t xHandle6 = NULL;
//	TaskHandle_t xHandle7 = NULL;
//	TaskHandle_t xHandle8 = NULL;
//	TaskHandle_t xHandle9 = NULL;
//	TaskHandle_t xHandle10 = NULL;
//	TaskHandle_t xHandle11 = NULL;
//	TaskHandle_t xHandle12 = NULL;
//	TaskHandle_t xHandle13 = NULL;



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
    xSemaphoreUart = xSemaphoreCreateBinary();
    xSemaphoreLed = xSemaphoreCreateBinary();
    xSemaphoreSpam = xSemaphoreCreateBinary();

    xReturned = xTaskCreate(
                                 vTaskShell,       /* Function that implements the task. */
                                 "TaskShell",          /* Text name for the task. */
                                 1024,      /* Stack size in words, not bytes. */
                                 ( void * ) 1,    /* Parameter passed into the task. */
                                 3,/* Priority at which the task is created. */
                                 &xHandle1 );

    xReturned = xTaskCreate(
                                    vTaskLed,       /* Function that implements the task. */
                                    "TaskLed",          /* Text name for the task. */
                                    1024,      /* Stack size in words, not bytes. */
                                    ( void * ) 1,    /* Parameter passed into the task. */
                                    2,/* Priority at which the task is created. */
                                    &xHandle2 );

    xReturned = xTaskCreate(
                                       vTaskSpam,       /* Function that implements the task. */
                                       "TaskSpam",          /* Text name for the task. */
                                       1024,      /* Stack size in words, not bytes. */
                                       ( void * ) 1,    /* Parameter passed into the task. */
                                       1,/* Priority at which the task is created. */
                                       &xHandle3 );
//    xReturned = xTaskCreate(
//                                        vTaskBidon1,       /* Function that implements the task. */
//                                        "TaskBidon1",          /* Text name for the task. */
//                                        1024,      /* Stack size in words, not bytes. */
//                                        ( void * ) 1,    /* Parameter passed into the task. */
//                                        1,/* Priority at which the task is created. */
//                                        &xHandle4 );
//
//    xReturned = xTaskCreate(
//                                            vTaskBidon2,       /* Function that implements the task. */
//                                            "TaskBidon2",          /* Text name for the task. */
//                                            65536,      /* Stack size in words, not bytes. */
//                                            ( void * ) 1,    /* Parameter passed into the task. */
//                                            1,/* Priority at which the task is created. */
//                                            &xHandle5 );
//    xReturned = xTaskCreate(
//                                            vTaskBidon3,       /* Function that implements the task. */
//                                            "TaskBidon3",          /* Text name for the task. */
//                                            65536,      /* Stack size in words, not bytes. */
//                                            ( void * ) 1,    /* Parameter passed into the task. */
//                                            1,/* Priority at which the task is created. */
//                                            &xHandle6 );
//    xReturned = xTaskCreate(
//                                            vTaskBidon4,       /* Function that implements the task. */
//                                            "TaskBidon4",          /* Text name for the task. */
//                                            65536,      /* Stack size in words, not bytes. */
//                                            ( void * ) 1,    /* Parameter passed into the task. */
//                                            1,/* Priority at which the task is created. */
//                                            &xHandle7 );
//    xReturned = xTaskCreate(
//                                            vTaskBidon5,       /* Function that implements the task. */
//                                            "TaskBidon5",          /* Text name for the task. */
//                                            65536,      /* Stack size in words, not bytes. */
//                                            ( void * ) 1,    /* Parameter passed into the task. */
//                                            1,/* Priority at which the task is created. */
//                                            &xHandle8 );
//    xReturned = xTaskCreate(
//                                            vTaskBidon6,       /* Function that implements the task. */
//                                            "TaskBidon6",          /* Text name for the task. */
//                                            65536,      /* Stack size in words, not bytes. */
//                                            ( void * ) 1,    /* Parameter passed into the task. */
//                                            1,/* Priority at which the task is created. */
//                                            &xHandle9 );
//
//        xReturned = xTaskCreate(
//                                                vTaskBidon7,       /* Function that implements the task. */
//                                                "TaskBidon7",          /* Text name for the task. */
//                                                1024,      /* Stack size in words, not bytes. */
//                                                ( void * ) 1,    /* Parameter passed into the task. */
//                                                1,/* Priority at which the task is created. */
//                                                &xHandle10 );
//        xReturned = xTaskCreate(
//                                                vTaskBidon8,       /* Function that implements the task. */
//                                                "TaskBidon8",          /* Text name for the task. */
//                                                1024,      /* Stack size in words, not bytes. */
//                                                ( void * ) 1,    /* Parameter passed into the task. */
//                                                1,/* Priority at which the task is created. */
//                                                &xHandle11 );
//        xReturned = xTaskCreate(
//                                                vTaskBidon9,       /* Function that implements the task. */
//                                                "TaskBidon9",          /* Text name for the task. */
//                                                1024,      /* Stack size in words, not bytes. */
//                                                ( void * ) 1,    /* Parameter passed into the task. */
//                                                1,/* Priority at which the task is created. */
//                                                &xHandle12 );
//        xReturned = xTaskCreate(
//                                                vTaskBidon10,       /* Function that implements the task. */
//                                                "TaskBidon10",          /* Text name for the task. */
//                                                1024,      /* Stack size in words, not bytes. */
//                                                ( void * ) 1,    /* Parameter passed into the task. */
//                                                1,/* Priority at which the task is created. */
//                                                &xHandle13 );

            configASSERT(xReturned==pdPASS);
            if(xReturned!=pdPASS){
            	Error_Handler();
            }


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
