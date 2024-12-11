/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
#define ADXL343_CS_GPIO GPIOA
#define ADXL343_CS_PIN GPIO_PIN_5
/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = SPI2_SCK_TO_SCLK_ACC_Pin|SPI2_MISO_TO_SDO_ACC_Pin|SPI2_MOSI_TO_SDI_ACC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, SPI2_SCK_TO_SCLK_ACC_Pin|SPI2_MISO_TO_SDO_ACC_Pin|SPI2_MOSI_TO_SDI_ACC_Pin);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void SPI_Write(uint8_t reg, uint8_t value) {
	uint8_t data[2];
	data[0] = reg | 0x40;
	data[1] = value;

	HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_SET);
}

uint8_t SPI_Read(uint8_t reg) {
    uint8_t tx_data = reg | 0x80;
    uint8_t rx_data = 0;

    HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &tx_data, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, &rx_data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_SET);

    return rx_data;
}

void Read_Acceleration(void) {
    uint8_t buffer[6];
    uint8_t reg = 0x32 | 0xC0;  // Commande de lecture multiple à partir de DATAX0

    HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi2, buffer, 6, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_SET);

    // Combinaison des octets pour obtenir des valeurs 16 bits signées
    int16_t x = (int16_t)((buffer[1] << 8) | buffer[0]);
    int16_t y = (int16_t)((buffer[3] << 8) | buffer[2]);
    int16_t z = (int16_t)((buffer[5] << 8) | buffer[4]);

    // Conversion en g (±2g, Full Resolution ou Fixed 10-bit)
    float scale = 3.9 / 1000.0;  // Sensibilité pour ±2g en g/LSB
    float ax = x * scale;
    float ay = y * scale;
    float az = z * scale;

    // Transmission des résultats via UART
    char msg[100];
    sprintf(msg, "Ax: %.3f g, Ay: %.3f g, Az: %.3f g\r\n", ax, ay, az);
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void ADXL343_Init(void) {
    uint8_t devid = SPI_Read(0x00);  // Lire le registre DEVID (0x00)

    if (devid == 0xE5) {
        // Le composant est détecté, procéder à l'initialisation
        SPI_Write(0x2C, 0x04);  // Configurer la bande passante à 1.56 Hz
        SPI_Write(0x2E, 0x80);  // Activer DATA_READY
        SPI_Write(0x31, 0x08);  // DATA_FORMAT : FULL_RES = 1, RANGE = ±2g
        SPI_Write(0x2D, 0x08);  // Activer le mode mesure
        Read_Acceleration();    // Lire les données pour effacer l'interruption

        char *msg = "ADXL343 detecte et initialise !\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    } else {
        char *error_msg = "Erreur : ADXL343 non detecte !\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)error_msg, strlen(error_msg), HAL_MAX_DELAY);
    }
}
/* USER CODE END 1 */
