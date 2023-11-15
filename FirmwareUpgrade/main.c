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
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
#include "stdbool.h"
#include "File_Handling.h"
#include "ff.h"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DISPLAY_UART                   &huart1
#define DISPLAY_RXUART                 USART1
#define DISPLAY_DMA_RX                 hdma_usart1_rx
#define RX_BUFFER_SIZE_DISPLAY         200             /*  Set receive buffer size */
#define RX_BUFFER_SIZE                 200             /*  Set receive buffer size */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
HAL_StatusTypeDef FinalResult;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
char rxBufferDisplay[200];
char displayReceivedBuffer[200];
HAL_StatusTypeDef result;
int counter = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
char str1[]="DRAKJHSUYDGBNCJHGJKSHBDN\xFF\xFF\xFF";
char str2[]="connect\xFF\xFF\xFF";
char str3[]="ÿÿconnect\xFF\xFF\xFF";
char str4[]="whmi-wri 3467784,921600,res0\xFF\xFF\xFF"; /*here file size is harcode , you can get from the function f_size*/
bool bConnectionEstablish = false;
bool bDeviceModelConfirm = false;
volatile bool bDisplayAcknowledge= false;
bool bInitialAcknowledge = false;
#define UART_BUFFER_SIZE 4096
uint8_t uartTxBuffer[UART_BUFFER_SIZE];
uint8_t connect1[]="DRAKJHSUYDGBNCJHGJKSHBDN\xFF\xFF\xFF";
uint8_t connect2[]="connect\xFF\xFF\xFF";
uint8_t connect3[]="connect\xFF\xFF\xFF";
extern bool USBdetected;



uint8_t receiveDisplay[100] = {0};
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
FIL USBHfnonew;
extern UART_HandleTypeDef huart1;
/* USER CODE END PV */
DWORD sizeFile = 0;
/* USER CODE BEGIN PFP */
UINT bytesRead = 0;
UINT iTotalFileSize = 0;
/* USER CODE BEGIN PV */
char sMainBuffer[200]={0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *) rxBufferDisplay, 200);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
		if(rxBufferDisplay[0]=='c'&&rxBufferDisplay[1]=='o'&&rxBufferDisplay[2]=='m'&&rxBufferDisplay[3]=='o'&&rxBufferDisplay[4]=='k')
		{
			bConnectionEstablish = true;
			memcpy(displayReceivedBuffer,rxBufferDisplay,sizeof(rxBufferDisplay));
		}
		else if(rxBufferDisplay[0]==0x05)
		{

			bDisplayAcknowledge = true;
		}
		else if(rxBufferDisplay[0]==0x88)
		{
			bDisplayAcknowledge = true;
		}
		memset(rxBufferDisplay,'\0',200);
		HAL_UART_Receive_DMA(&huart1,(uint8_t *) rxBufferDisplay, 200);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit_DMA(&huart6, (uint8_t *)"error callback", 15);
	if(huart->ErrorCode == HAL_UART_ERROR_NE)           /*check for noise error*/
	{
		// Assuming you have a DMA handle called "hdma" and a specific DMA stream/channel like DMA_CHANNEL_1
		__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_NE);      /* Clearing the noise error flag*/
		huart->ErrorCode |= HAL_UART_ERROR_NE;          /* Clearing the error code */
		HAL_UARTEx_ReceiveToIdle_DMA(DISPLAY_UART, (uint8_t *) rxBufferDisplay, RX_BUFFER_SIZE);
		HAL_UART_Receive_DMA(DISPLAY_UART, (uint8_t *) rxBufferDisplay, RX_BUFFER_SIZE);
	}
	else if(huart->ErrorCode == HAL_UART_ERROR_ORE)     /* Check for overrun error*/
	{
		huart->ErrorCode |= HAL_UART_ERROR_ORE;         /* Clearing the error code */
		__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_ORE);
		HAL_UARTEx_ReceiveToIdle_DMA(DISPLAY_UART, (uint8_t *) rxBufferDisplay, RX_BUFFER_SIZE);
		HAL_UART_Receive_DMA(DISPLAY_UART, (uint8_t *) rxBufferDisplay, RX_BUFFER_SIZE);
	}
}

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

	strcat(sMainBuffer,"DRAKJHSUYDGBNCJHGJKSHBDN\xFF\xFF\xFF""connect\xFF\xFF\xFF");
	strcat(sMainBuffer,"\xFF\xFF connect\xFF\xFF\xFF");

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
  MX_DMA_Init();
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_FATFS_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */

  while (1)
  {
    /* USER CODE END WHILE */

	  if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)==GPIO_PIN_RESET) /*using a button to send the connect command*/
	  {
		  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)sMainBuffer,strlen(sMainBuffer));
		  HAL_Delay(500);
	  }
	  if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)==GPIO_PIN_RESET) /*using a button to send start firmware upgarde command str4*/
	  {
		  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str4, strlen(str4));
		  HAL_Delay(500);
	  }
	  if(bConnectionEstablish == true) /* if received the comok string*/
	  {
			char* tok ;
		    char* rest = displayReceivedBuffer;
			tok = strtok_r(displayReceivedBuffer,",",&rest);
			int count = 0;
			while (tok != 0) {
				switch(count)
				{
					case 2:
					{
						if(strcmp(tok,"NX3224K024_011R")==0) /*checking whether the received string for the model number*/
						{
							bDeviceModelConfirm = true;
						}
						break;
					}
					default:
					{
						break;
					}
				}
				tok = strtok_r(0, ",",&rest);
				count++;
	           }
			memset(displayReceivedBuffer,0,sizeof(displayReceivedBuffer));
	  }

    MX_USB_HOST_Process();

    if(USBdetected == true && bConnectionEstablish == true && bDisplayAcknowledge == true)    /*if usb is detected and received the acknowledge*/
    {
    	  Mount_USB();
    	  Check_USB_Details();   // check space details
    	  USBdetected = false;
     	  f_open(&USBHfnonew, "/new.tft", FA_READ); /*open the file*/
     	  sizeFile = f_size(&USBHfnonew);           /*to get the file size*/
     	  while(1)
     	  {
     		  if(bDisplayAcknowledge == true) /*wait for 0x05 acknowledge from the display after receiving each packet*/
     		  {
  				f_read(&USBHfnonew, uartTxBuffer, UART_BUFFER_SIZE, &bytesRead);
  				if(bytesRead == 0)
  				{
  					f_close(&USBHfnonew);
  					break;
  				}
  				HAL_UART_Transmit_DMA(&huart1,(uint8_t *) uartTxBuffer, sizeof(uartTxBuffer));
  				bDisplayAcknowledge = false;
     		  }

     	  }

    }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}



/*function to check the uart state is ready and to send the data*/

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
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
