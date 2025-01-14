
/* Includes ----------------------------------------------------------------- */
#include "lidarX4.h"
#include "gpio.h"
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
		HAL_UART_Receive_DMA(hlidar->huart, hlidar->data_buff, DATA_BUFF_SIZE_LIDAR);
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
 * @brief
 * @param
 */
void LIDAR_process_frame(LIDAR_HandleTypeDef_t * hlidar) {
    uint16_t Si;
    float Di;
    float Ai;
    float AngleFSA = (hlidar->process_frame.FSA >> 1) / 64.0; // Angle initial (en degrés)
    float AngleLSA = (hlidar->process_frame.LSA >> 1) / 64.0; // Angle final (en degrés)
    float diffAngle = AngleLSA - AngleFSA; // Différence d'angle (en degrés)
    int LSN = hlidar->process_frame.LSN;     // Nombre d'échantillons dans le paquet
    int index;

    for (int i = 0; i < hlidar->process_frame.index / 2; i++) {
        // Extraction des données de distance
        Si = hlidar->process_frame.frame_buff[2 * i] | (hlidar->process_frame.frame_buff[2 * i + 1] << 8);
        Di = Si / 4.0; // Distance en mm

        // Calcul de l'angle sans correction
        Ai = (diffAngle / (LSN - 1)) * i + AngleFSA;

        // Correction de l'angle
        float AngCorrect = 0.0;
        if (Di != 0) {
            AngCorrect = atan(21.8 * (155.3 - Di) / (155.3 * Di)) * (180.0 / 3.141592653589793); // Conversion en degrés
        }
        Ai += AngCorrect;

        // Index du point dans le buffer circulaire
        index = (int)round(Ai);

        // Filtrage des points trop proches ou trop loin
        if (Di > 1500 || Di < 40) {
            hlidar->process_frame.point_buff[index] = 0;
        } else {
            hlidar->process_frame.point_buff[index] = (int)Di;
        }
    }
}


/*
 * @brief
 * @param
 */
void LIDAR_get_point(LIDAR_HandleTypeDef_t *hlidar) {


    for (int i = 0; i < DATA_BUFF_SIZE_LIDAR; i++) {
        // Réponse à la commande SCAN pour détecter le début des trames
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
            hlidar->process_frame.PH = hlidar->process_frame.PH|(hlidar->data_buff[i] << 8);
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
            hlidar->process_frame.FSA = hlidar->process_frame.FSA|(hlidar->data_buff[i] << 8);
        }
        else if (i == frame_start + 6) {
            hlidar->process_frame.LSA = hlidar->data_buff[i];
        }
        else if (i == frame_start + 7) {
            hlidar->process_frame.LSA = hlidar->process_frame.LSA|(hlidar->data_buff[i] << 8);
        }
        else if (i == frame_start + 8) {
            hlidar->process_frame.CS = hlidar->data_buff[i];
        }
        else if (i == frame_start + 9) {
            hlidar->process_frame.CS = hlidar->process_frame.CS|(hlidar->data_buff[i] << 8);
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

    // Gérer les indices circulaires pour les buffers
    frame_start = frame_start - DATA_BUFF_SIZE_LIDAR;
    frame_end = frame_end - DATA_BUFF_SIZE_LIDAR;
}


/*
 * @brief Filter point buff using a median filter to eliminate the aberrant values due
 * to the noise
 * @param hlidar : pointer to the lidar handle structure
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
 * @brief Detect clusters in LIDAR data
 * @param hlidar Pointer to the LIDAR_HandleTypeDef_t structure
 */
void LIDAR_Find_Clusters(LIDAR_HandleTypeDef_t *hlidar) {
    int *distances = hlidar->process_frame.filtered_buff;
    int cluster_count = 0;

    // Initialize the first cluster
    int cluster_start = 0;

    // Traverse 360 degrees to group points into clusters
    for (int i = 1; i < POINT_BUFF_SIZE_LIDAR; i++) {
        if (fabs(distances[i] - distances[i - 1]) > CLUSTER_THRESHOLD) {
            // Compute the mean distance for the cluster
            int mean_distance = 0;
            for (int j = cluster_start; j < i; j++) {
                mean_distance += distances[j];
            }
            mean_distance /= (i - cluster_start);

            // Compute the mean angle for the cluster
            int mean_angle = (cluster_start + i - 1) / 2;

            // Store cluster data
            hlidar->clusters[cluster_count].mean_angle = mean_angle;
            hlidar->clusters[cluster_count].mean_distance = mean_distance;
            hlidar->clusters[cluster_count].point_count = i - cluster_start;

            // Update for the next cluster
            cluster_start = i;
            cluster_count++;

            // Prevent exceeding the array limit
            if (cluster_count >= MAX_CLUSTERS) {
                cluster_count = MAX_CLUSTERS - 1;
                break;
            }
        }
    }

    // Handle the last cluster
    int mean_distance = 0;
    for (int j = cluster_start; j < POINT_BUFF_SIZE_LIDAR; j++) {
        mean_distance += distances[j];
    }
    mean_distance /= (POINT_BUFF_SIZE_LIDAR - cluster_start);
    int mean_angle = (cluster_start + POINT_BUFF_SIZE_LIDAR - 1) / 2;

    hlidar->clusters[cluster_count].mean_angle = mean_angle;
    hlidar->clusters[cluster_count].mean_distance = mean_distance;
    hlidar->clusters[cluster_count].point_count = POINT_BUFF_SIZE_LIDAR - cluster_start;
    cluster_count++;

    // Update the total number of detected clusters
    hlidar->cluster_count = cluster_count;
}

/**
 * @brief Calculate the average distance in a given range
 * @param distances Pointer to the array of distances
 * @param start_index Start index of the range
 * @param end_index End index of the range
 * @return The average distance in the specified range
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
