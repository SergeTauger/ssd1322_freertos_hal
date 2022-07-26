/**
 ****************************************************************************************
 *
 * \file SSD1322_HW_Driver.c
 *
 * \brief Hardware dependent functions for SSD1322 OLED display.
 *
 * This file contains functions that rely on hardware of used MCU. In this example functions
 * are filled with STM32F411RE hardware implementation. To use this library on any other MCU
 * you just have to provide its hardware implementations of functions from this file and higher
 * level functions should work without modification.
 *
 * Copyright (C) 2020 Wojciech Klimek
 * MIT license:
 * https://github.com/wjklimek1/SSD1322_OLED_library
 *
 ****************************************************************************************
 */

//=================== Include hardware HAL libraries =====================//
#include "stm32l4xx_hal.h"
#include "main.h"

#include "../SSD1322_OLED_lib/SSD1322_HW_Driver.h"

extern SPI_HandleTypeDef OLED_HSPI;
extern TaskHandle_t oled_task;
int32_t tx_index = 0; //тут хранится количество переданных байт
int32_t tx_len = 0;   //сколько всего байт нужно передать
uint8_t *tx_data;     //указатель на массив с передаваемыми данными

//extern volatile uint8_t OLED_SPI_TX_completed_flag;

//====================== CS pin low ========================//
/**
 *  @brief Drives CS (Chip Select) pin of SPI interface low.
 *
 *  CS pin may be also signed as "SS" or "NSS"
 */
void SSD1322_HW_drive_CS_low()
{
	HAL_GPIO_WritePin(SPI5_CS_GPIO_Port, SPI5_CS_Pin, 0);
}

//====================== CS pin high ========================//
/**
 *  @brief Drives CS (Chip Select) pin of SPI interface high.
 *
 *  CS pin may be also signed as "SS" or "NSS"
 */
void SSD1322_HW_drive_CS_high()
{
	HAL_GPIO_WritePin(SPI5_CS_GPIO_Port, SPI5_CS_Pin, 1);
}

//====================== DC pin low ========================//
/**
 *  @brief Drives DC (Data/Command) pin of OLED driver low.
 *
 *  High state is for data and low state is for command.
 */
void SSD1322_HW_drive_DC_low()
{
	HAL_GPIO_WritePin(SPI5_DC_GPIO_Port, SPI5_DC_Pin, 0);
}

//====================== DC pin high ========================//
/**
 *  @brief Drives DC (Data/Command) pin of of OLED driver high.
 *
 *  High state is for data and low state is for command.
 */
void SSD1322_HW_drive_DC_high()
{
	HAL_GPIO_WritePin(SPI5_DC_GPIO_Port, SPI5_DC_Pin, 1);
}

//====================== RESET pin low ========================//
/**
 *  @brief Drives RESET pin of of OLED driver low.
 *
 *  Logic low on RESET resets OLED driver.
 */
void SSD1322_HW_drive_RESET_low()
{
	HAL_GPIO_WritePin(SPI5_RESET_GPIO_Port, SPI5_RESET_Pin, 0);
}

//====================== RESET pin high ========================//
/**
 *  @brief Drives RESET pin of of OLED driver high.
 *
 *  Logic low on RESET resets OLED driver.
 */
void SSD1322_HW_drive_RESET_high()
{
	HAL_GPIO_WritePin(SPI5_RESET_GPIO_Port, SPI5_RESET_Pin, 1);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &OLED_HSPI){
		if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING){
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			SSD1322_HW_drive_CS_high();
			vTaskNotifyGiveFromISR(oled_task, &xHigherPriorityTaskWoken);
			if (xHigherPriorityTaskWoken){
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			}
		} else {
			SSD1322_HW_drive_CS_high();
			OLED_SPI_TX_completed_flag = 1;
		}
	}
}

//====================== Send single SPI byte ========================//
/**
 *  @brief Transmits single byte through SPI interface.
 *
 *  @param[in] byte_to_transmit byte that will be transmitted through SPI interface
 */
void SSD1322_HW_SPI_send_byte(uint8_t byte_to_transmit)
{
	//TODO may be change to intermediate buffer?
	OLED_SPI_TX_completed_flag = 0;
/*	  while(SPI2->SR & SPI_SR_BSY)
	    ;
	  SPI2->CR1 &= ~(1<<SPI_CR1_SPE_Pos); //Включаем SPI
	  tx_index = 0;
	  tx_len = 0;
	  (SPI2->DR) = (uint16_t)byte_to_transmit;
	  SPI2->CR1 |= 1<<SPI_CR1_SPE_Pos; //Включаем SPI
	  SPI2->CR2 |= (1<<SPI_CR2_TXEIE_Pos);
*/
	HAL_SPI_Transmit_IT(&OLED_HSPI, &byte_to_transmit, 1);

}

//====================== Send array of SPI bytes ========================//
/**
 *  @brief Transmits array of bytes through SPI interface.
 *
 *  @param[in] array_to_transmit array of bytes that will be transmitted through SPI interface
 *  @param[in] array_size amount of bytes to transmit
 */
void SSD1322_HW_SPI_send_array(uint8_t *array_to_transmit, uint32_t array_size)
{
	OLED_SPI_TX_completed_flag = 0;
	HAL_SPI_Transmit_IT(&OLED_HSPI, array_to_transmit, array_size);
}

//====================== Milliseconds delay ========================//
/**
 *  @brief Wait for x milliseconds.
 *
 *  NOTE: This function is only used in initialization sequence to pull reset down and high
 *  and later to wait for Vdd stabilization. It takes time ONLY on startup (around 200ms)
 *  and is NOT USED during normal operation.
 *
 *  @param[in] milliseconds time to wait
 */
void SSD1322_HW_msDelay(uint32_t milliseconds)
{
	vTaskDelay(milliseconds/portTICK_PERIOD_MS);
}
