/*
 * @file ydlidarx4_header.h
 * @brief Definition of the functions and structures of the YDLIDARX4's driver
 *
 *  Created on: Nov 27, 2024
 *      Author: jeanp
 */

#ifndef YDLIDARX4_DRIVER_YDLIDARX4_HEADER_H_
#define YDLIDARX4_DRIVER_YDLIDARX4_HEADER_H_

/* Includes ----------------------------------------------------------------- */
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include "math.h"
#include "usart.h"
#include "gpio.h"

/* Defines ------------------------------------------------------------------ */
#define DATA_BUFF_SIZE_LIDAR 1000 // Size of the data buffer received with DMA
#define POINT_BUFF_SIZE_LIDAR 360 // Size of the angle buffer
#define FRAME_BUFF_SIZE_LIDAR 90 // Max size of one lidar frame (Content + header)
#define CMD_BUFF_SIZE_LIDAR 2 // Size of the command buffer
#define INFO_BUFF_SIZE_LIDAR 27 // Size of the lidar's information data buffer
#define HEALTH_BUFF_SIZE_LIDAR 10 // Size of the lidar's health information buffer

#define CLUSTER_THRESHOLD 20  // Threshold difference for separating clusters
#define MAX_CLUSTERS 100      // Maximum number of clusters
#define NB_DEGREES 360        // Number of degrees in the LIDAR sweep

/* Typedef ------------------------------------------------------------------ */

/*
 * @brief
 */
typedef enum LIDAR_CMD{
	START_CMD_LIDAR = 0xA5, // Lidar start, 1st byte of the command buffer
	SCAN_CMD_LIDAR = 0x60, // Lidar scan command
	STOP_CMD_LIDAR = 0x65, // Lidar stop command
	GET_INFO_CMD_LIDAR = 0x90, // Get device information command
	GET_HEALTH_CMD_LIDAR = 0x91, // Get health information command
	RESTART_CMD_LIDAR = 0x80 // Lidar restart command

}LIDAR_CMD_t;

/*
 * @brief
 */
typedef struct LIDAR_ScanDataStruct{
	uint16_t PH; // Packet header (2B)
	uint8_t CT; // Package type (1B)
	uint8_t LSN; // Sample quantity (1B)
	uint16_t FSA; // Start angle (2B)
	uint16_t LSA; // End Angle (2B)
	uint16_t CS; // Check code (2B)
	uint8_t index; // Frame index (1B)
	uint8_t received_buff[DATA_BUFF_SIZE_LIDAR]; // received data with DMA
	uint8_t frame_buff[FRAME_BUFF_SIZE_LIDAR];
	int point_buff[POINT_BUFF_SIZE_LIDAR];
}LIDAR_ScanData_t;

/*
 * @brief Structure to store cluster information
 */
typedef struct LIDAR_ClusterStruct {
    int mean_angle;           // Mean angle of the cluster
    int mean_distance;        // Mean distance of the cluster
    int point_count;          // Number of points in the cluster
} LIDAR_Cluster_t;

/*
 * @brief
 */
typedef struct LIDAR_DeviceInfoStruct{
	uint16_t start_sign;
	uint32_t lenght;
	uint8_t mode;
	uint8_t type_code;
	uint8_t model;
	char firmware_version[6];
	uint8_t hardware_version;
	char serial_number[17];
}LIDAR_DeviceInfo_t;

/*
 * @brief
 */
typedef struct LIDAR_HealthStatusStruct{
	uint16_t start_sign;
	uint32_t lenght;
	uint8_t mode;
	uint8_t type_code;
	uint8_t status_code;
	uint16_t error_code;
}LIDAR_HealthStatus_t;

/*
 * @brief
 */
typedef struct LIDAR_HandleStruct{
	UART_HandleTypeDef *huart;		// huart 3 handle

	uint8_t info_buff[INFO_BUFF_SIZE_LIDAR];
	uint8_t health_buff[HEALTH_BUFF_SIZE_LIDAR];
	uint8_t data_buff[DATA_BUFF_SIZE_LIDAR];		// data buff received with DMA

	//Processing of the frames
	LIDAR_ScanData_t process_frame;

	// Detected clusters
	LIDAR_Cluster_t clusters[MAX_CLUSTERS];
	int cluster_count;                    // Total number of detected clusters
}LIDAR_HandleTypeDef_t;

/* Functions prototypes ----------------------------------------------------- */
void LIDAR_Init(LIDAR_HandleTypeDef_t * hlidar);
HAL_StatusTypeDef LIDAR_Start(LIDAR_HandleTypeDef_t * hlidar);
HAL_StatusTypeDef LIDAR_Stop(LIDAR_HandleTypeDef_t * hlidar);
HAL_StatusTypeDef LIDAR_Restart(LIDAR_HandleTypeDef_t * hlidar);
HAL_StatusTypeDef LIDAR_Get_Info(LIDAR_HandleTypeDef_t * hlidar);
HAL_StatusTypeDef LIDAR_Get_Health_Status(LIDAR_HandleTypeDef_t * hlidar);
void LIDAR_process_frame(LIDAR_HandleTypeDef_t * hlidar);
void LIDAR_get_point(LIDAR_HandleTypeDef_t *hlidar);
void LIDAR_Find_Clusters(LIDAR_HandleTypeDef_t *hlidar);
int LIDAR_Calculate_Average_Distance(int *distances, int start_index, int end_index);


#endif /* YDLIDARX4_DRIVER_YDLIDARX4_HEADER_H_ */
