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
 * @brief
 */
int lidar_Start(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, SCAN_CMD_LIDAR};
	HAL_UART_Transmit(&huart3, lidar_command, 2, 2000);
	if(HAL_UART_Receive(&huart3, data_lidar, DATA_SIZE_LIDAR, 2000)==HAL_OK){
		for (int i = 0; i < 10; i++) {
//		    printf("Data lidar%d : %d\r\n", i + 1, data_lidar[i]);
		}
		return 1;
	}
	else{
//		printf("Erreur\r\n");
		return 0;
	}
}

