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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "retarget.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  RTC_TimeTypeDef sTime;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
uint8_t buf[1];
uint8_t res1[4];
uint8_t res[]={1,2,3,4};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t Cmd_End[3] = {0xFF,0xFF,0xFF};
void NEXTION_SendString (char *ID, char *string)
{
	char buf[50];
	int len = sprintf(buf, "%s.pic=%s", ID, string);
	//HAL_UART_Transmit(&huart2, (uint8_t *)buf, len,100);
	//HAL_UART_Transmit(&huart2, Cmd_End, 3, 100);
	//HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
	//HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, len);
	//HAL_DMA_Start(&hdma_usart1_tx, (uint32_t)buf, (uint32_t)&huart1.Instance->DR, 1en);
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf,len);

	// Wait for DMA transfer to complete
	HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

	// Configure DMA transfer for the second string
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)Cmd_End, (uint32_t)&huart2.Instance->DR, 3);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Cmd_End, 3);

}
void NEXTION_SendString1(char *ID, char *string)
{
	char buf[50];
	int len = sprintf(buf, "%s.pic=%s", ID, string);
	//HAL_UART_Transmit(&huart2, (uint8_t *)buf, len,100);
	//HAL_UART_Transmit(&huart2, Cmd_End, 3, 100);
	//HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
	//HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, len);
	//HAL_DMA_Start(&hdma_usart1_tx, (uint32_t)buf, (uint32_t)&huart1.Instance->DR, 1en);
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf,len);

	// Wait for DMA transfer to complete
	HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

	// Configure DMA transfer for the second string
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)Cmd_End, (uint32_t)&huart2.Instance->DR, 3);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Cmd_End, 3);

}

void Disp_time(int hour,int minute)
{
	    char buf[50];
		int len = sprintf (buf, "t0.txt=%d",hour);
		HAL_UART_Transmit(&huart2, (uint8_t *)buf, len,10);
		HAL_UART_Transmit(&huart2, Cmd_End, 3, 10);

		char buf1[50];
		len = sprintf (buf, "t5.en=%d",minute);
		HAL_UART_Transmit(&huart2, (uint8_t *)buf1, len,10);
		HAL_UART_Transmit(&huart2, Cmd_End, 3, 10);
}
void NEXTION_PageChange(char *string)
{
	char buf[50];
	int len = sprintf(buf, "page %s",string);
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf,len);
	HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
	HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)Cmd_End, (uint32_t)&huart2.Instance->DR, 3);
	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Cmd_End, 3);

}
void  prog_bar( char *val)
{
	    char buf[50];
		int len = sprintf(buf, "j0.val=%s",val);
		HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf,len);
		HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)Cmd_End, (uint32_t)&huart2.Instance->DR, 3);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Cmd_End, 3);
}void  var_val( int val)
{
	    char buf[50];
		int len = sprintf(buf, "va0.val=%d",val);
		HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)buf, (uint32_t)&huart2.Instance->DR, len);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf,len);
		HAL_DMA_PollForTransfer(&hdma_usart2_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
		HAL_DMA_Start(&hdma_usart2_tx, (uint32_t)Cmd_End, (uint32_t)&huart2.Instance->DR, 3);
		HAL_UART_Transmit_DMA(&huart2, (uint8_t*)Cmd_End, 3);
}
void get_data()
{





}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   HAL_UART_Transmit(&huart1, buf, sizeof(buf),10);
   HAL_UART_Receive_IT(&huart2, buf,sizeof(buf));

}
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
  RetargetInit(&huart1);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  //NEXTION_SendString("p9","0");


  //var_val(1);
  //NEXTION_SendString1("p9","0");
  //HAL_UART_Transmit(&huart2, Cmd_End, 3, 100);
  /* USER CODE END 2 */
printf("hello\r\n");
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  HAL_UART_Receive_IT(&huart2, buf,sizeof(buf));

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	  //printf("%02d : %02d\r\n",sTime.Hours,sTime.Minutes);

	  	//HAL_DMA_Start(&hdma_usart2_rx, (uint32_t)&huart2.Instance->DR, (uint32_t)buf, sizeof(buf));
	  	//HAL_UART_Receive_DMA(&huart2,(uint8_t*)buf , sizeof(buf));

       HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
       HAL_Delay(500);
	  //HAL_UART_Transmit(&huart1, buf, 20, 100);



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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x1;
  sTime.Minutes = 0x5;
  sTime.Seconds = 0x0;
  sTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
