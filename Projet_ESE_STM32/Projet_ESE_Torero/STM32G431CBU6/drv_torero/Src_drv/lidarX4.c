/**
  ******************************************************************************
  * @file    lidar.c
  * @brief   This file provides the necessary functions to initialize, send commands
  * 		 and parse the received data of YD LidarX4
  ******************************************************************************
  */


/* Includes ----------------------------------------------------------------- */
#include "lidarX4.h"
#include "gpio.h"

/* Functions ---------------------------------------------------------------- */

/*
 * @brief Initialization of the lidar
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval None
 */
void LIDAR_Init(LIDAR_HandleTypeDef_t * hlidar){
	hlidar->huart = &huart3;

	HAL_GPIO_WritePin(GPIOA, DEV_EN_LIDAR_Pin, GPIO_PIN_SET);
	// Enable M_EN lidar
	HAL_GPIO_WritePin(M_EN_LIDAR_GPIO_Port, M_EN_LIDAR_Pin, GPIO_PIN_SET);

}


/*
 * @brief Send the start/scan command to the lidar and
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval HAL status
 */
HAL_StatusTypeDef LIDAR_Start(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, SCAN_CMD_LIDAR};
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		HAL_UART_Receive_DMA(hlidar->huart, hlidar->data_buff, DATA_BUFF_SIZE_LIDAR);
		return status;
	}
	else{
		return status;
	}
}

/*
 * @brief Send the stop command to the lidar to halt its operation
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval HAL status
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
 * @brief Restart the lidar device by sending the restart command
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval HAL status
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
 * @brief Retrieve device information from the lidar
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval HAL status
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
 * @brief Retrieve the health status of the lidar device
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval HAL status
 */
HAL_StatusTypeDef LIDAR_Get_Health_Status(LIDAR_HandleTypeDef_t * hlidar){
	uint8_t lidar_command[2] = {START_CMD_LIDAR, GET_HEALTH_CMD_LIDAR};
	LIDAR_HealthStatus_t * health_status;
	HAL_StatusTypeDef status = HAL_UART_Transmit(hlidar->huart, lidar_command, 2, 2000);
	if(status == HAL_OK){
		HAL_UART_Receive(hlidar->huart, hlidar->health_buff, HEALTH_BUFF_SIZE_LIDAR, 2000);
		health_status->start_sign = (hlidar->health_buff[0]<<8)|hlidar->health_buff[1];
		health_status->lenght = (hlidar->health_buff[2])|(hlidar->health_buff[3]<<8)|(hlidar->health_buff[4]<<16);
		health_status->mode = hlidar->health_buff[5];
		health_status->type_code = hlidar->health_buff[6];
		health_status->status_code = hlidar->health_buff[7];
		health_status->error_code = hlidar->health_buff[8]|(hlidar->health_buff[9]<<8);

		return status;
	}
	else{
		return status;
	}
}

/*
 * @brief Process a data frame received from the lidar to extract points
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval None
 */
void LIDAR_process_frame(LIDAR_HandleTypeDef_t * hlidar) {
    uint16_t Si;
    float Di;
    float Ai;
    float AngleFSA = (hlidar->process_frame.FSA >> 1) / 64.0; // Initial angle (in degrees)
    float AngleLSA = (hlidar->process_frame.LSA >> 1) / 64.0; // Final angle (in degrees)
    float diffAngle = AngleLSA - AngleFSA; // Angle difference (in degrees)
    int LSN = hlidar->process_frame.LSN;     // Number of samples in the packet
    int index;

    for (int i = 0; i < hlidar->process_frame.index / 2; i++) {
        // Extraction of distance data
        Si = hlidar->process_frame.frame_buff[2 * i] | (hlidar->process_frame.frame_buff[2 * i + 1] << 8);
        Di = Si / 4.0; // Distance in mm

        // Calculation of the angle without correction
        Ai = (diffAngle / (LSN - 1)) * i + AngleFSA;

        // Angle correction
        float AngCorrect = 0.0;
        if (Di != 0) {
            AngCorrect = atan(21.8 * (155.3 - Di) / (155.3 * Di)) * (180.0 / 3.141592653589793); // Conversion to degrees
        }
        Ai += AngCorrect;

        // Index of the point in the circular buffer
        index = (int)round(Ai);

        // Filtering points that are too close or too far
        if (Di > 2000 || Di < 40) { // Above 2m and closer than 4 cm
            hlidar->process_frame.point_buff[index] = 0;
        } else {
            hlidar->process_frame.point_buff[index] = (int)Di;
        }
    }
}



/*
 * @brief Extract points from the lidar's circular buffer and classify them in frames
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval None
 */
void LIDAR_get_point(LIDAR_HandleTypeDef_t *hlidar) {

    for (int i = 0; i < DATA_BUFF_SIZE_LIDAR; i++) {
        // Response to the SCAN command to detect the beginning of frames
        if (i == 0) {
            if ((hlidar->data_buff[i] == 0xA5) &&
                (hlidar->data_buff[i + 1] == 0x5A) &&
                (hlidar->data_buff[i + 2] == 0x05) &&
                (hlidar->data_buff[i + 3] == 0x00) &&
                (hlidar->data_buff[i + 4] == 0x00) &&
                (hlidar->data_buff[i + 5] == 0x40) &&
                (hlidar->data_buff[i + 6] == 0x81)) {
                printf("Scan Command Reply\r\n");
                hlidar->process_frame.index = 0;
                i = 6;
                frame_start = 7;
                frame_end = frame_start + 4;
            }
        }

        // Data frame extraction
        if (i == frame_start) {
            hlidar->process_frame.PH = hlidar->data_buff[i];
        }
        else if (i == frame_start + 1) {
            hlidar->process_frame.PH = hlidar->process_frame.PH | (hlidar->data_buff[i] << 8);
        }
        else if (i == frame_start + 2) {
            hlidar->process_frame.CT = hlidar->data_buff[i];
        }
        else if (i == frame_start + 3) {
            frame_end = frame_start + 9 + 2 * hlidar->data_buff[i];
            hlidar->process_frame.LSN = hlidar->data_buff[i];
        }
        else if (i == frame_start + 4) {
            hlidar->process_frame.FSA = hlidar->data_buff[i];
        }
        else if (i == frame_start + 5) {
            hlidar->process_frame.FSA = hlidar->process_frame.FSA | (hlidar->data_buff[i] << 8);
        }
        else if (i == frame_start + 6) {
            hlidar->process_frame.LSA = hlidar->data_buff[i];
        }
        else if (i == frame_start + 7) {
            hlidar->process_frame.LSA = hlidar->process_frame.LSA | (hlidar->data_buff[i] << 8);
        }
        else if (i == frame_start + 8) {
            hlidar->process_frame.CS = hlidar->data_buff[i];
        }
        else if (i == frame_start + 9) {
            hlidar->process_frame.CS = hlidar->process_frame.CS | (hlidar->data_buff[i] << 8);
        }
        else if (i == frame_end) {
            hlidar->process_frame.frame_buff[hlidar->process_frame.index++] = hlidar->data_buff[i];

            if (frame_end - frame_start > 11) {
                // Extracting the points using the frame previously calculated
                LIDAR_process_frame(hlidar);
            }

            hlidar->process_frame.index = 0;
            frame_start = frame_end + 1;
            frame_end = frame_start + 5;
        }
        else {
            hlidar->process_frame.frame_buff[hlidar->process_frame.index++] = hlidar->data_buff[i];
        }
    }

    // Handle circular indices for the buffers
    frame_start = frame_start - DATA_BUFF_SIZE_LIDAR;
    frame_end = frame_end - DATA_BUFF_SIZE_LIDAR;
}



/*
 * @brief Apply a median filter the points buffer to eliminate the aberrant values
 * 		  due to the noise.
 * @param hlidar Pointer to a LIDAR_HandleStruct structure that contains
 *               the YD lidarX4 configuration information.
 * @retval None
 */
void LIDAR_median_filter(LIDAR_HandleTypeDef_t *hlidar){
    int data_to_filter[POINT_BUFF_SIZE_LIDAR];
    int block_size = 5; // Size of the sliding window
    int middle = block_size / 2;

    // Copy the original data to process
    for (int i = 0; i < POINT_BUFF_SIZE_LIDAR; i++) {
        data_to_filter[i] = hlidar->process_frame.point_buff[i];
    }

    // Apply the median filter
    for (int i = 0; i < POINT_BUFF_SIZE_LIDAR; i++) {
        int window[block_size];
        int count = 0;

        // Build the window (handle edges with zero-padding)
        for (int j = -middle; j <= middle; j++) {
            if ((i + j) >= 0 && (i + j) < POINT_BUFF_SIZE_LIDAR) {
                window[count++] = data_to_filter[i + j];
            } else {
                window[count++] = 0; // Zero-padding for edge cases
            }
        }

        // Sort the window to find the median
        for (int k = 0; k < count - 1; k++) {
            for (int l = k + 1; l < count; l++) {
                if (window[k] > window[l]) {
                    int temp = window[k];
                    window[k] = window[l];
                    window[l] = temp;
                }
            }
        }

        // Assign the median value to the filtered data
        hlidar->process_frame.filtered_buff[i] = window[count / 2];
    }
}


/**
 * @brief Calculate the average distance in a given range
 * @param distances Pointer to the array of distances
 * @param start_index Start index of the range
 * @param end_index End index of the range
 * @retval The average distance in the specified range
 */
int LIDAR_Calculate_Average_Distance(int *distances, int start_index, int end_index) {
    int sum = 0;
    int count = end_index - start_index + 1;

    // Avoid division by zero
    if (count <= 0) {
        return 0;
    }

    // Calculate average distance
    for (int i = start_index; i <= end_index; i++) {
        sum += distances[i];
    }
    return sum / count;
}
