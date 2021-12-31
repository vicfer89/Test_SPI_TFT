/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ili9341.h"
#include "xpt2046.h"
#include "ccs811.h"
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

/* USER CODE BEGIN PV */
ccs811_dev_t iaq_dev;
volatile uint8_t f_CCS811_DataAvail;
volatile int irq_counter;
volatile uint16_t CO2_Value = 400;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static uint8_t read_iaq_i2c(ccs811_dev_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t len);
static uint8_t write_iaq_i2c(ccs811_dev_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t len);
static void CCS811_Init(void);
int __io_putchar(int ch)
{
	return ITM_SendChar(ch); // Comparator 0 Enabled y seleccionado bit 0 de ITM Stimulus Port
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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_I2C3_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */

  printf("Inicializando el sistema\r\n");

  HAL_TIM_Base_Start_IT(&htim2);
  ILI9341_Init();
  XPT2046_Init();
  CCS811_Init();
  printf("Sistema Inicializado\r\n");


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 /* if(f_CCS811_DataAvail)
	  {
		  printf("[%d] Interupción lista\r\n", irq_counter);
		  irq_counter = irq_counter + 1;
		  f_CCS811_DataAvail = 0;
	  }*/
	  ccs811_status_t iaq_stat = ccs811_status(&iaq_dev);
	  if(1 == iaq_stat.data_ready)
	  {
		  ccs811_alg_results_t data = ccs811_get_data(&iaq_dev);
		  printf("[%d] eCO2: %u \t TVOC: %u \r\n", irq_counter, data.eCO2, data.TVOC);
		  CO2_Value = data.eCO2; // Guardamos en variable global
		  irq_counter++;
	  }
    /* USER CODE END WHILE */

  MX_TouchGFX_Process();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

extern void touchgfxSignalVSync(void);

uint8_t read_iaq_i2c(ccs811_dev_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t len)
{

	uint8_t result = 0;
	result = HAL_I2C_Master_Transmit(&hi2c3, dev->address, &reg_addr, 1, 100);

	if (result == HAL_OK) {
		result = HAL_I2C_Master_Receive(&hi2c3, dev->address, data, len, 100);
	}
	return hi2c3.ErrorCode;
}

uint8_t write_iaq_i2c(ccs811_dev_t *dev, uint8_t reg_addr, uint8_t *data, uint16_t len)
{

	HAL_I2C_Mem_Write(&hi2c3, dev->address, reg_addr, 1, data, len, 100);

	return hi2c3.ErrorCode;
}

void CCS811_Init(void)
{

  iaq_dev.address = (CCS811_DEFAULT_ADDRESS << 1);
  iaq_dev.read = &read_iaq_i2c;
  iaq_dev.write = &write_iaq_i2c;
  HAL_GPIO_WritePin(CCS811_RST_GPIO_Port, CCS811_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(40);
  HAL_GPIO_WritePin(CCS811_RST_GPIO_Port, CCS811_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(70); // Wait for 70 ms
  ccs811_error ret = ccs811_init(&iaq_dev);
  if(0 != ret)
  {
	  printf("Error en CCS811 \r\n");
  }
  f_CCS811_DataAvail = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_PIN_10 == GPIO_Pin)
	{
		f_CCS811_DataAvail = 1;
	}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim->Instance == TIM2)
  {
	  touchgfxSignalVSync();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
