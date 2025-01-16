/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lidarX4.h"
#include "stdio.h"
#include "ADXL.h"
#include "motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern TIM_HandleTypeDef htim16;

LIDAR_HandleTypeDef_t hlidar;

SemaphoreHandle_t SemHalfCallBack;
SemaphoreHandle_t SemClpCallBack;

TaskHandle_t xHandleLIDAR = NULL;
TaskHandle_t xHandleETAT = NULL;
TaskHandle_t xHandleMOTOR = NULL;
TaskHandle_t xHandleEDGE = NULL;

BaseType_t ret;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void TaskMOTOR (void * pvParameters);
void TaskETAT(void * pvParameters);
void TaskEDGE(void * pvParameters);
void TaskLIDAR(void * pvParameters);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int motor_init = 0;

//flag capteurs bord
int capteur_G = 0;
int capteur_D = 0;

//Asservissement
int angle;		//Calcul de alpha
int vitesse;	//

//Etat Robot
int etat = 0;	//0 -> souris	1 -> chat

//Commande moteur
int alpha1 = MAX_SPEED_FORWARD; //moteur droit
int alpha2 = MAX_SPEED_FORWARD; //moteur gauche

//Flag pour bloquer l'actualisation de la vitesse des moteurs de TaskMOTOR
int EdgeProcess = 0;
int ShockProcess = 0;

//SemaphoreHandle_t xNoSignalSemaphore;
SemaphoreHandle_t SemDMAHalfCallBack;
SemaphoreHandle_t SemDMAClpCallBack;

//Lidar
int distance_min=0;
int idx_min = 0;
uint16_t frame_start = 0;
uint16_t frame_end = 0;
float coeff_Lidar, coeff_Capteur;
float delta;

//Accéléromètre
uint8_t rst_int;

int __io_putchar(int chr){
	HAL_UART_Transmit(&huart2, (uint8_t*)&chr, 1, HAL_MAX_DELAY);
	return chr;
}

void print_buffer(const char * Name, uint8_t *pData, uint16_t Size, int N_lines){
	printf("%s", Name);
	if(N_lines >= (int) Size){
		for(int i = 0; i<Size; i++){
			printf("%d : %d\r\n", i, pData[i]);
		}
	}
	else{
		for(int i = 0; i<N_lines; i++){
			printf("%d : %d\r\n", i, pData[i]);
		}
	}
}

void TaskETAT(void * pvParameters){
	for (;;) {
		// Attendre que la notification arrive
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		ShockProcess++;
		//lire INT_SOURCE met à 0 le bit d'interruption single tap
		//du registre pour générer une nouvelle interruption au prochain choc
		rst_int = SPI_Read(ADXL343_REG_INT_SOURCE);
		if(etat == 1){
			etat = 0;
			printf("Squik\r\n");
		}
		else{
			etat = 1;
			printf("Miaou\r\n");
		}
		alpha1 = MAX_SPEED_REVERSE;
		alpha2 = MAX_SPEED_REVERSE+40;
		vTaskDelay(pdMS_TO_TICKS(400));
		ShockProcess--;
	}
}

void TaskLIDAR(void * pvParameters){
	for(;;){
	}
}

void TaskMOTOR (void * pvParameters){
	for(;;){
		if((EdgeProcess||ShockProcess) == 0){
			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_FORWARD;
		}
	}
}

/* 
//TaskMOTOR si le lidar fournissait bien l'angle de l'objet le plus proche et la distance
void TaskMOTOR (void * pvParameters){
	for(;;){
 		if((EdgeProcess||ShockProcess) == 0){
			if (Etat_Robot == 0){
				// Cas Robot loin 
				if (distance>==TRIG_DIST) {
					alpha1 = MAX_SPEED_FORWARD;
					alpha2 = MAX_SPEED_FORWARD; 
				}
				// Cas Robot proche 
				if (distance<=TRIG_DIST) {
					if (ANGLE_R <=45 || ANGLE_R >=315) {
						if (ANGLE_R <=45) {
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD - ANGLE_R;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD -(ANGLE_MAX-ANGLE_R);
							alpha2 = MAX_SPEED_FORWARD;
						}
					}
					else if (ANGLE_R <=90 || ANGLE_R >=270) {
						if (ANGLE_R <=90) {
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD;
						}
	
					}
					else if (ANGLE_R <=180 || ANGLE_R >=180) {
						if (ANGLE_R <=180) {
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD - 10;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD - 10;
							alpha2 = MAX_SPEED_FORWARD;
						}
					} 
				}   
			}
			if(Etat_Robot == 1){
							// Cas Robot loin 
				if (distance>=TRIG_DIST) {
					if (ANGLE_R <=45 || ANGLE_R >=315) {
						if (ANGLE_R <=45) {
							alpha1 = MAX_SPEED_FORWARD - 5;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD - 5;
						}
					}
					else if (ANGLE_R <=90 || ANGLE_R >=270) {
						if (ANGLE_R <=90) {
							alpha1 = 50 - ANGLE_R;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = 50 - (360-ANGLE_R);
						}
	
					}
				}
					else if (ANGLE_R <=180 || ANGLE_R >=180) {
						if (ANGLE_R <=180) {
							alpha1 = - MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = - MAX_SPEED_FORWARD;
						} 
				}
				// Cas Robot proche 
				if (distance<==TRIG_DIST) {
					if (ANGLE_R <=45 || ANGLE_R >=315) {
						if (ANGLE_R <=45) {
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = MAX_SPEED_FORWARD;
						}
					}
					else if (ANGLE_R <=90 || ANGLE_R >=270) {
						if (ANGLE_R <=90) {
							alpha1 = 70 - ANGLE_R;
							alpha2 = MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = MAX_SPEED_FORWARD;
							alpha2 = 70 - ANGLE_R;
						}
	
					}
					else if (ANGLE_R <=180 || ANGLE_R >=180) {
						if (ANGLE_R <=180) {
							alpha1 = - (MAX_SPEED_FORWARD - 10);
							alpha2 = - MAX_SPEED_FORWARD;
						}
						else{
							alpha1 = - MAX_SPEED_FORWARD;
							alpha2 = - (MAX_SPEED_FORWARD - 10);
						}
					}    
				}
			}
		}
	}
}
*/

/*		Début de comportement à implementer pour asserv dynamique
 *
 * 		capteur_virtuel = capteur_G&&capteur_D;
		int erreur_capteur = -45*capteur_G+45*capteur_D+90*capteur_virtuel;
		int erreurLidar = 10/180;
		int alphaD = coeff_Lidar*erreurLidar-coeff_Capteur*erreur_capteur;
		int alphaG = -coeff_Lidar*erreurLidar+coeff_Capteur*erreur_capteur;
 */

void TaskEDGE(void * pvParameters){
	for (;;) {

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		EdgeProcess++;

		/* Cas Robot bord frontal */
		if((capteur_D&&capteur_G)==1){
			while((capteur_D&&capteur_G)==1){
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));


			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}

		/* Cas Robot bord droit tourne a gauche */
		if(capteur_D){
			while(capteur_D){
				// reculer, tourner et repartir
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));


			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}

		/* Cas Robot bord gauche tourne a droite */
		if(capteur_G){
			while(capteur_G){
				// reculer, tourner et repartir
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));

			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}
		//lire INT_SOURCE met à 0 le bit d'interruption single tap
		//du registre pour générer une nouvelle interruption au prochain choc
		rst_int = SPI_Read(ADXL343_REG_INT_SOURCE);
		EdgeProcess--;
	}
}

void TaskEDGE(void * pvParameters){
	for (;;) {

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		EdgeProcess++;

		/* Cas Robot bord frontal */
		if((capteur_D&&capteur_G)==1){
			while((capteur_D&&capteur_G)==1){
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));


			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}

		/* Cas Robot bord droit tourne a gauche */
		if(capteur_D){
			while(capteur_D){
				// reculer, tourner et repartir
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));


			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}

		/* Cas Robot bord gauche tourne a droite */
		if(capteur_G){
			while(capteur_G){
				// reculer, tourner et repartir
				alpha1 = MAX_SPEED_REVERSE;
				alpha2 = MAX_SPEED_REVERSE;
			}
			vTaskDelay(pdMS_TO_TICKS(100));

			alpha1 = MAX_SPEED_FORWARD;
			alpha2 = MAX_SPEED_REVERSE;
			vTaskDelay(pdMS_TO_TICKS(200));
		}
		//lire INT_SOURCE met à 0 le bit d'interruption single tap
		//du registre pour générer une nouvelle interruption au prochain choc
		rst_int = SPI_Read(ADXL343_REG_INT_SOURCE);
		EdgeProcess--;
	}
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_TIM16_Init();
	/* USER CODE BEGIN 2 */
	Start_Motors();
	ADXL343_Init();
	HAL_TIM_Base_Start_IT(&htim16);
	LIDAR_Init(&hlidar);
	LIDAR_Start(&hlidar);

	ret = xTaskCreate(TaskETAT,"TaskETAT",STACK_SIZE,(void *) NULL,PRIORITY_ETAT,&xHandleETAT);
	if (ret != pdPASS)
	{
		printf("Error creating TaskETAT\r\n");
		Error_Handler();
	}
	printf("Task ETAT created\r\n");

	/*
	ret = xTaskCreate(TaskLIDAR,"TaskLIDAR",STACK_SIZE,(void *) NULL,PRIORITY_LIDAR,&xHandleLIDAR);
	if (ret != pdPASS)
	{
		printf("Error creating TaskLIDAR\r\n");
		Error_Handler();
	}
	printf("Task LIDAR created\r\n");
	 */

	ret = xTaskCreate(TaskMOTOR,"TaskMOTOR",STACK_SIZE,(void *) NULL,PRIORITY_MOTOR,&xHandleMOTOR);
	if (ret != pdPASS)
	{
		printf("Error creating TaskMOTOR\r\n");
		Error_Handler();
	}
	printf("Task MOTOR created\r\n");


	ret = xTaskCreate(TaskEDGE,"TaskEDGE",STACK_SIZE,(void *) NULL,PRIORITY_EDGE,&xHandleEDGE);
	if (ret != pdPASS)
	{
		printf("Error creating TaskEDGE\r\n");
		Error_Handler();
	}
	printf("Task EDGE created\r\n");

	SemDMAHalfCallBack = xSemaphoreCreateBinary();
	SemDMAClpCallBack = xSemaphoreCreateBinary();

	/* USER CODE END 2 */

	/* Call init function for freertos objects (in cmsis_os2.c) */
	MX_FREERTOS_Init();

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == INT1_ACC_Pin) {
		if(EdgeProcess==0){
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			vTaskNotifyGiveFromISR(xHandleETAT, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
			HAL_GPIO_TogglePin(GPIOC, Status_LED_Pin);
		}
	}
	if (GPIO_Pin == CAPTEUR_D_Pin) {
		if(capteur_D==1){
			capteur_D=0;
		}
		else{
			capteur_D=1;
		}
		//printf("capteurD\r\n");
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(xHandleEDGE, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	if (GPIO_Pin == CAPTEUR_G_Pin) {
		if(capteur_G==1){
			capteur_G=0;
		}
		else{
			capteur_G=1;
		}
		//printf("capteurG\r\n");
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(xHandleEDGE, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

//Timer qui génère une interruption toutes les 1ms pour faire une rampe d'accélération des moteurs
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{	
	//Timer qui génère une interruption toutes les 1ms pour faire une rampe d'accélération des moteurs
	if (htim->Instance == TIM16){
		if(motor_init){		//Attendre de générer les PWM avant de mettre à jour leur rapport cyclique
			Motor_SetSpeed_R(alpha1);
			Motor_SetSpeed_L(alpha2);
		}
	}
}

/*
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	xSemaphoreGiveFromISR(SemHalfCallBack,&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	xSemaphoreGiveFromISR(SemClpCallBack,&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}
 */
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
