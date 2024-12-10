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

/*
 * @brief
 * @param
 */
HAL_StatusTypeDef LIDAR_Get_Info(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, GET_INFO_CMD_LIDAR};
	LIDAR_DeviceInfo_t * device_info;
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		HAL_UART_Receive(hlidar->huart, hlidar->info_buff, INFO_BUFF_SIZE_LIDAR, 2000);
		device_info->start_sign = (hlidar->info_buff[0]<<8)|hlidar->info_buff[1];
		device_info->lenght = (hlidar->info_buff[2])|(hlidar->info_buff[3]<<8)|(hlidar->info_buff[4]<<16);
		device_info->mode = hlidar->info_buff[5];
		device_info->type_code = hlidar->info_buff[6];
		device_info->model = hlidar->info_buff[7];
		snprintf(device_info->firmware_version,6,"%d.%d",hlidar->info_buff[8],hlidar->info_buff[9]);
		device_info->hardware_version  = hlidar->info_buff[10];
		snprintf(device_info->serial_number,17,"%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",hlidar->info_buff[11],hlidar->info_buff[12],hlidar->info_buff[13],hlidar->info_buff[14],hlidar->info_buff[15],hlidar->info_buff[16],hlidar->info_buff[17],hlidar->info_buff[18],hlidar->info_buff[19],hlidar->info_buff[20],hlidar->info_buff[21],hlidar->info_buff[22],hlidar->info_buff[23],hlidar->info_buff[24],hlidar->info_buff[25],hlidar->info_buff[26]);

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
HAL_StatusTypeDef LIDAR_Get_Health_Status(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, GET_HEALTH_CMD_LIDAR};
	LIDAR_HealthStatus_t * health_status;
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		HAL_UART_Receive(hlidar->huart, hlidar->info_buff, INFO_BUFF_SIZE_LIDAR, 2000);
		health_status->start_sign=(hlidar->health_buff[0]<<8)|hlidar->health_buff[1];
		health_status->lenght=(hlidar->health_buff[2])|(hlidar->health_buff[3]<<8)|(hlidar->health_buff[4]<<16);
		health_status->mode=hlidar->health_buff[5];
		health_status->type_code=hlidar->health_buff[6];
		health_status->status_code=hlidar->health_buff[7];
		health_status->error_code=hlidar->health_buff[8]|(hlidar->health_buff[9]<<8);

		return status;
	}
	else{
		return status;
	}
}
