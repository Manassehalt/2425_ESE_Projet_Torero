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
#include "ydlidarx4_header.h"
#include "stdio.h"
#include "ADXL.h"
#include "motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define ADXL343_CS_GPIO GPIOA
#define ADXL343_CS_PIN GPIO_PIN_5
#define PWM_MAX_DUTY_CYCLE 8499
#define FWD_GPIO_PIN GPIO_PIN_8  // PA8 -> FWD
#define REV_GPIO_PIN GPIO_PIN_9  // PA9 -> REV
#define FWD_GPIO_PORT GPIOA      // Port pour FWD
#define REV_GPIO_PORT GPIOA      // Port pour REV
#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64

#define STACK_SIZE 256

#define SHOCK_THRESHOLD 15
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
LIDAR_HandleTypeDef_t hlidar;

SemaphoreHandle_t SemHalfCallBack;
SemaphoreHandle_t SemClpCallBack;

TaskHandle_t xHandleLIDAR = NULL;
TaskHandle_t xHandleETAT = NULL;
TaskHandle_t xHandleACC = NULL;
TaskHandle_t xHandleMOTOR = NULL;
TaskHandle_t xHandleEDGE = NULL;
//TaskHandle_t xHandleMOTOR = NULL;
BaseType_t ret;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void TaskACC(void *pvParameters);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//flag choc
int shock_detected = 0;

//flag caoteurs bord
int capteur_G = 0;
int capteur_D = 0;
int capteur_virtuel = 0;

//Vitesse
int angle;		//Calcul de alpha
int vitesse;	//

//Etat Robot
int chat = 0;	//0 -> souris	1 -> chat
int alpha1,alpha2;
float coeff_Lidar, coeff_Capteur;
float delta;
SemaphoreHandle_t SemEtat;
SemaphoreHandle_t SemEdge;
SemaphoreHandle_t xShockSemaphore;
//SemaphoreHandle_t xNoSignalSemaphore;
SemaphoreHandle_t SemDMAHalfCallBack;
SemaphoreHandle_t SemDMAClpCallBack;

//Lidar
int distance_min=0;
int idx_min = 0;
int frame_start = 0;
int frame_end = 0;



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
		printf("hi\r\n");
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			if(chat == 1){
				chat = 0;
				printf("Squik\r\n");
			}
			else{
				// Le choc a été détecté
				chat = 1;
				printf("Miaou\r\n");
			}
		}

}

void TaskLIDAR(void * pvParameters){
	for(;;){
		printf("lidar\r\n");
		vTaskDelay(100);
	}
}

void TaskACC(void * pvParameters){

	for (;;) {

}
}

void TaskMOTOR (void * pvParameters){
	for(;;){

		Motor_Reverse_L(20);
		//Motor_Forward_R(100);

	}
}

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
		printf("Bonjour de Task2\r\n");
		//s'arreter, reculer, tourner et repartir

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
  /* USER CODE BEGIN 2 */
	Start_Motors();
	Motor_SetSpeed_L(30);
	//Motor_SetSpeed_R(-15);
	ADXL343_Init();
	LIDAR_Init(&hlidar);
	LIDAR_Start(&hlidar);

	//ret = xTaskCreate(TaskETAT,"TaskETAT",STACK_SIZE,(void *) NULL,5,&xHandleETAT);
	//if (ret != pdPASS)
	//{
	//	printf("Error creating TaskETAT\r\n");
	//	Error_Handler();
	//}
	//printf("Task ETAT created\r\n");

	/*
	ret = xTaskCreate(TaskLIDAR,"TaskLIDAR",STACK_SIZE,(void *) NULL,3,&xHandleLIDAR);
	if (ret != pdPASS)
	{
		printf("Error creating TaskLIDAR\r\n");
		Error_Handler();
	}
	printf("Task LIDAR created\r\n");
*/
	//ret = xTaskCreate(TaskACC,"TaskACC",STACK_SIZE,(void *) NULL,4,&xHandleACC);
	//if (ret != pdPASS)
	//{
	//	printf("Error creating TaskACC\r\n");
	//	Error_Handler();
	//}
	//printf("Task ACC created\r\n");

	//ret = xTaskCreate(TaskMOTOR,"TaskMOTOR",STACK_SIZE,(void *) NULL,1,&xHandleMOTOR);
	//if (ret != pdPASS)
	//{
	//	printf("Error creating TaskMOTOR\r\n");
	//	Error_Handler();
	//}
	//printf("Task MOTOR created\r\n");


	ret = xTaskCreate(TaskEDGE,"TaskEDGE",STACK_SIZE,(void *) NULL,4,&xHandleEDGE);
	if (ret != pdPASS)
	{
		printf("Error creating TaskEDGE\r\n");
		Error_Handler();
	}
	printf("Task EDGE created\r\n");

	SemDMAHalfCallBack = xSemaphoreCreateBinary();
	SemDMAClpCallBack = xSemaphoreCreateBinary();
	xShockSemaphore = xSemaphoreCreateBinary();
	SemEtat = xSemaphoreCreateBinary();
	//xNoSignalSemaphore = xSemaphoreCreateBinary();
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
		printf("yeah\r\n");
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		vTaskNotifyGiveFromISR(xHandleETAT, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	if (GPIO_Pin == CAPTEUR_D_Pin) {
			printf("void\r\n");
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			vTaskNotifyGiveFromISR(xHandleEDGE, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	if (GPIO_Pin == CAPTEUR_G_Pin) {
			printf("vide\r\n");
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			vTaskNotifyGiveFromISR(xHandleEDGE, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
