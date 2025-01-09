#include "usart.h"
#include "spi.h"
#include "ADXL.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


TaskHandle_t Bump_task = NULL;


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

HAL_StatusTypeDef Read_Acceleration(float *accel_data) {
	uint8_t buffer[6];
	int16_t raw_accel[3]; // Valeurs brutes signées (16 bits) pour X, Y, Z
	uint8_t reg = 0x32 | 0xC0;  // Commande de lecture multiple à partir de DATAX0

	HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &reg, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi2, buffer, 6, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL343_CS_GPIO, ADXL343_CS_PIN, GPIO_PIN_SET);

	// Combinaison des octets pour obtenir des valeurs 16 bits signées
	raw_accel[0] = (int16_t)((buffer[1] << 8) | buffer[0]);
	raw_accel[1] = (int16_t)((buffer[3] << 8) | buffer[2]);
	raw_accel[2] = (int16_t)((buffer[5] << 8) | buffer[4]);

	// Conversion en g (±2g, Full Resolution ou Fixed 10-bit)
	//float scale = 3.9 / 1000.0;  // Sensibilité pour ±2g en g/LSB

	// Convertir les données brutes en 'g'
	accel_data[0] = (float)raw_accel[0]; // Accélération X en g
	accel_data[1] = (float)raw_accel[1]; // Accélération Y en g
	accel_data[2] = (float)raw_accel[2]; // Accélération Z en g

	return HAL_OK;
}

void ADXL343_Init(void) {
	uint8_t devid = SPI_Read(ADXL343_REG_DEVID);  // Lire le registre DEVID (0x00)

	if (devid == 0xE5) {
		// Le composant est détecté, procéder à l'initialisation

		SPI_Write(ADXL343_REG_POWER_CTL, 0x08);  // Désactiver appareil
		SPI_Write(ADXL343_REG_DATA_FORMAT, 0x08);  // DATA_FORMAT : FULL_RES = 1, RANGE = ±2g
		SPI_Write(ADXL343_REG_INT_ENABLE, 0x00);  // Désactiver interruption
		SPI_Write(ADXL343_REG_THRESH_TAP, 0x20);	// Config seuil choc
		SPI_Write(ADXL343_REG_DUR, 0x10);	// Config durée choc
		SPI_Write(ADXL343_REG_LATENT, 0xC8);	// Config latence choc
		SPI_Write(ADXL343_REG_TAP_AXES, 0x06);	// Activer axe X Y
		SPI_Write(ADXL343_REG_BW_RATE, 0x0F);  // Configurer la bande passante
		SPI_Write(ADXL343_REG_INT_MAP, 0x40);	// Configurer pin interruption
		SPI_Write(ADXL343_REG_INT_ENABLE, 0x40);  // Activer interruption
		//Read_Acceleration(float *accel_data);    // Lire les données pour effacer l'interruption

		char *msg = "ADXL343 detecte et initialise !\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	} else {
		char *error_msg = "Erreur : ADXL343 non detecte !\r\n";
		HAL_UART_Transmit(&huart2, (uint8_t *)error_msg, strlen(error_msg), HAL_MAX_DELAY);
	}
}
