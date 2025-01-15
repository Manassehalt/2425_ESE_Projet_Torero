/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT2_ACC_Pin GPIO_PIN_13
#define INT2_ACC_GPIO_Port GPIOC
#define INT2_ACC_EXTI_IRQn EXTI15_10_IRQn
#define RESET_Pin GPIO_PIN_10
#define RESET_GPIO_Port GPIOG
#define CS_ACC_Pin GPIO_PIN_5
#define CS_ACC_GPIO_Port GPIOA
#define LED_NE_Pin GPIO_PIN_7
#define LED_NE_GPIO_Port GPIOA
#define LED_SE_Pin GPIO_PIN_4
#define LED_SE_GPIO_Port GPIOC
#define CAPTEUR_G_Pin GPIO_PIN_0
#define CAPTEUR_G_GPIO_Port GPIOB
#define CAPTEUR_G_EXTI_IRQn EXTI0_IRQn
#define CAPTEUR_D_Pin GPIO_PIN_1
#define CAPTEUR_D_GPIO_Port GPIOB
#define CAPTEUR_D_EXTI_IRQn EXTI1_IRQn
#define M_EN_LIDAR_Pin GPIO_PIN_2
#define M_EN_LIDAR_GPIO_Port GPIOB
#define USART3_TX_LIDAR_Pin GPIO_PIN_10
#define USART3_TX_LIDAR_GPIO_Port GPIOB
#define USART3_RX_LIDAR_Pin GPIO_PIN_11
#define USART3_RX_LIDAR_GPIO_Port GPIOB
#define LED_NW_Pin GPIO_PIN_12
#define LED_NW_GPIO_Port GPIOB
#define SPI2_SCK_TO_SCLK_ACC_Pin GPIO_PIN_13
#define SPI2_SCK_TO_SCLK_ACC_GPIO_Port GPIOB
#define SPI2_MISO_TO_SDO_ACC_Pin GPIO_PIN_14
#define SPI2_MISO_TO_SDO_ACC_GPIO_Port GPIOB
#define SPI2_MOSI_TO_SDI_ACC_Pin GPIO_PIN_15
#define SPI2_MOSI_TO_SDI_ACC_GPIO_Port GPIOB
#define LED_SW_Pin GPIO_PIN_6
#define LED_SW_GPIO_Port GPIOC
#define INT1_ACC_Pin GPIO_PIN_10
#define INT1_ACC_GPIO_Port GPIOA
#define INT1_ACC_EXTI_IRQn EXTI15_10_IRQn
#define DEV_EN_LIDAR_Pin GPIO_PIN_15
#define DEV_EN_LIDAR_GPIO_Port GPIOA
#define Status_LED_Pin GPIO_PIN_10
#define Status_LED_GPIO_Port GPIOC
#define Status_LED_debbug_Pin GPIO_PIN_11
#define Status_LED_debbug_GPIO_Port GPIOC
#define LED_W_Pin GPIO_PIN_4
#define LED_W_GPIO_Port GPIOB
#define LED_N_Pin GPIO_PIN_5
#define LED_N_GPIO_Port GPIOB
#define LED_E_Pin GPIO_PIN_6
#define LED_E_GPIO_Port GPIOB
#define LED_S_Pin GPIO_PIN_7
#define LED_S_GPIO_Port GPIOB
#define TIM4_CH4_M_SCTR_LIDAR_Pin GPIO_PIN_9
#define TIM4_CH4_M_SCTR_LIDAR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define ADXL343_CS_GPIO GPIOA
#define ADXL343_CS_PIN GPIO_PIN_5
#define FWD_GPIO_PIN GPIO_PIN_8  // PA8 -> FWD
#define REV_GPIO_PIN GPIO_PIN_9  // PA9 -> REV
#define FWD_GPIO_PORT GPIOA      // Port pour FWD
#define REV_GPIO_PORT GPIOA      // Port pour REV
#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64

#define STACK_SIZE 256

#define PRIORITY_LIDAR 0
#define PRIORITY_MOTOR 1
#define PRIORITY_ETAT 2
#define PRIORITY_EDGE 3

#define TAP_THRESHOLD 50
#define TAP_DUR 15
#define TAP_LATENT 0xFF

#define MAX_SPEED_FORWARD 70
#define MAX_SPEED_REVERSE -70
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
