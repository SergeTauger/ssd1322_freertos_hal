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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SSD1322_OLED_lib/SSD1322_HW_Driver.h"
#include "SSD1322_OLED_lib/SSD1322_API.h"
#include "SSD1322_OLED_lib/SSD1322_GFX.h"

#include "SSD1322_OLED_lib/Fonts/FreeMono12pt7b.h"
#include "SSD1322_OLED_lib/Fonts/FreeSansOblique9pt7b.h"

#include "tom_and_jerry.h"
#include "creeper.h"
#include "krecik.h"
#include "pat_i_mat.h"
#include "stars_4bpp.h"
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
 SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

volatile uint8_t OLED_SPI_TX_completed_flag = 1;
extern TaskHandle_t  oled_task;
uint8_t tx_buf[256 * 64 / 2];
uint8_t tx_buf2[256*256 / 2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void oled_test_task(void* par){
//	vTaskDelay(2000/portTICK_PERIOD_MS);
	for(;;){
		//Set frame buffer size in pixels - it is used to avoid writing to memory outside frame buffer
		//Normally it has to only be done once on initialization, but buffer size is changed near the end of while(1);.
		set_buffer_size(256, 64);
		// Fill buffer with zeros to clear any garbage values
		fill_buffer(tx_buf, 0);

		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		// Let's try some features of this OLED display

		// First, draw some pixels on frame buffer
		// draw_pixel(frame_buffer, x, y, brightness);
		draw_pixel(tx_buf, 10, 10, 1);
		draw_pixel(tx_buf, 15, 15, 5);
		draw_pixel(tx_buf, 20, 20, 9);
		draw_pixel(tx_buf, 25, 25, 15);

		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		// draw vertical and horizontal lines
		draw_hline(tx_buf, 31, 20, 50, 10);
		draw_vline(tx_buf, 31, 0, 31, 10);

		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		// draw simple oblique line
		draw_line(tx_buf, 40, 0, 80, 31, 12);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		// draw antialiased oblique line. It should appear softer and nicer than a simple one
		draw_AA_line(tx_buf, 50, 0, 90, 31, 12);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		//draw circle, empty rectangle and filled rectangle
		draw_circle(tx_buf, 180, 20, 20, 15);
		draw_rect(tx_buf, 100, 5, 120, 25, 15);
		draw_rect_filled(tx_buf, 124, 5, 144, 25, 8);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		//clean buffer
		fill_buffer(tx_buf, 0);

		//display 8-bit grayscale bitmap (ony first 4 bits are actually written to memory)
		draw_bitmap_8bpp(tx_buf, pat_i_mat, 0, 0, 64, 64);
		draw_bitmap_8bpp(tx_buf, krecik, 128, 0, 64, 64);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(5000);

		//display 4-bit grayscale bitmap (one byte in bitmap array corresponds to two pixels)
		draw_bitmap_4bpp(tx_buf, stars_4bpp, 0, 0, 256, 64);
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(3000);

		//you can invert screen colors using API function
		SSD1322_API_set_display_mode(SSD1322_MODE_INVERTED);
		SSD1322_HW_msDelay(2000);
		//pixels can be also turned on or off
		SSD1322_API_set_display_mode(SSD1322_MODE_ON);
		SSD1322_HW_msDelay(1000);
		SSD1322_API_set_display_mode(SSD1322_MODE_OFF);
		SSD1322_HW_msDelay(1000);
		//ok, go back to normal
		SSD1322_API_set_display_mode(SSD1322_MODE_NORMAL);
		SSD1322_HW_msDelay(500);

		//exact grayscale values can be set individually for each level from 0 to 15 - always send 16 byte array of values 0-180
		uint8_t grayscale_tab[16] = {0, 5, 10, 15, 20, 25, 30, 35, 145, 150, 155, 160, 165, 170, 175, 180};
		SSD1322_API_custom_grayscale(grayscale_tab);
		SSD1322_HW_msDelay(2000);
		//New grayscale values should be close to black in darker areas and close to white in brighter

		//reset grayscale to default linear values
		SSD1322_API_default_grayscale();
		SSD1322_HW_msDelay(2000);

		//display can be set to sleep mode and then woken up
		SSD1322_API_sleep_on();
		SSD1322_HW_msDelay(1000);
		SSD1322_API_sleep_off();

		//clean buffer
		fill_buffer(tx_buf, 0);

		// now let's try to write some text with a font
		// first thing to do is font selection
		select_font(&FreeMono12pt7b);
		// now text will we written with that font
		draw_text(tx_buf, "Lorem ipsum", 10, 20, 15);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		//change font to a differen one
		select_font(&FreeSansOblique9pt7b);
		draw_text(tx_buf, "dolor sit amet", 10, 45, 15);
		// send a frame buffer to the display
		send_buffer_to_OLED(tx_buf, 0, 0);
		SSD1322_HW_msDelay(2000);

		//you can use frame buffer that is bigger than default 256x64 pixels.
		//Remember to divide size by two, because one byte stores two pixels.

		set_buffer_size(256, 256);

		//now print a huge bitmap into frame buffer
		draw_bitmap_8bpp(tx_buf2, creeper, 0, 0, 256, 256);
		send_buffer_to_OLED(tx_buf2, 0, 0);
		SSD1322_HW_msDelay(2000);

		//only 1/4 of image is seen, so let's scroll the frame buffer down
		for(int i = 0; i < 192; i++)
		{
			send_buffer_to_OLED(tx_buf2, 0, i);
			SSD1322_HW_msDelay(5);
		}
		SSD1322_HW_msDelay(200);
		for (int i = 191; i >= 0; i--)
		{
			send_buffer_to_OLED(tx_buf2, 0, i);
			SSD1322_HW_msDelay(5);
		}
		SSD1322_HW_msDelay(2000);
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
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  __HAL_RCC_SPI2_CLK_ENABLE();

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /**SPI2 GPIO Configuration
  PC2     ------> SPI2_MISO
  PB10     ------> SPI2_SCK
  PB15     ------> SPI2_MOSI
  */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
/*
  SPI2->CR1 = 0<<SPI_CR1_CRCL_Pos  //Размер кадра 8 бит
      | 0<<SPI_CR1_LSBFIRST_Pos     //MSB first
      | 1<<SPI_CR1_SSM_Pos          //Программное управление SS
      | 1<<SPI_CR1_SSI_Pos          //SS в высоком состоянии
      | 0x04<<SPI_CR1_BR_Pos        //Скорость передачи: F_PCLK/32
      | 1<<SPI_CR1_MSTR_Pos         //Режим Master (ведущий)
      | 0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos; //Режим работы SPI: 0

  HAL_NVIC_SetPriority(SPI2_IRQn, 6, 0);
    NVIC_EnableIRQ(SPI2_IRQn); //Разрешаем прерывания от SPI1

    SPI2->CR1 |= 1<<SPI_CR1_SPE_Pos; //Включаем SPI
*/
  SSD1322_API_init();

	set_buffer_size(256, 64);
	// Fill buffer with zeros to clear any garbage values
	fill_buffer(tx_buf, 0);

	// send a frame buffer to the display
	send_buffer_to_OLED(tx_buf, 0, 0);
	HAL_Delay(2000);

	// Let's try some features of this OLED display

	// First, draw some pixels on frame buffer
	// draw_pixel(frame_buffer, x, y, brightness);
	draw_pixel(tx_buf, 10, 10, 1);
	draw_pixel(tx_buf, 15, 15, 5);
	draw_pixel(tx_buf, 20, 20, 9);
	draw_pixel(tx_buf, 25, 25, 15);


	xTaskCreate(oled_test_task, NULL, 2048, (void*)1, tskIDLE_PRIORITY+1, NULL);
	vTaskStartScheduler();
  /* USER CODE END 2 */

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV8;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA8 PA9 PA11 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM16 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM16) {
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
