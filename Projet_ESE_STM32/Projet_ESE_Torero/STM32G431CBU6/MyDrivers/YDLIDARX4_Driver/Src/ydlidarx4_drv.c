/*
 * ydlidarx4_drv.c
 *
 *  Created on: Nov 27, 2024
 *      Author: jeanp
 *
 *  About this driver :
 *
 */

/* Includes ----------------------------------------------------------------- */
#include "ydlidarx4_header.h"

/* Functions ---------------------------------------------------------------- */

/*
 * @brief Initialization of the lidar
 * @param
 */
void LIDAR_Init(LIDAR_HandleTypeDef_t * hlidar){
	hlidar->huart = &huart3;

	HAL_GPIO_WritePin(GPIOA, DEV_EN_LIDAR_Pin, GPIO_PIN_SET);
	// Enable M_EN lidar
	HAL_GPIO_WritePin(M_EN_LIDAR_GPIO_Port, M_EN_LIDAR_Pin, GPIO_PIN_SET);

}


/*
 * @brief
 * @param
 */
HAL_StatusTypeDef LIDAR_Start(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, SCAN_CMD_LIDAR};
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		HAL_UART_Receive(hlidar->huart, hlidar->data_buff, DATA_BUFF_SIZE_LIDAR, 2000);
		return status;
	}
	else{
		return status;
	}
}

/*
 * @brief
 * @param
 */
HAL_StatusTypeDef LIDAR_Stop(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, STOP_CMD_LIDAR};
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		return status;
	}
	else{
		return status;
	}
}

/*
 * @brief
 * @param
 */
HAL_StatusTypeDef LIDAR_Restart(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, RESTART_CMD_LIDAR};
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		return status;
	}
	else{
		return status;
	}
}

