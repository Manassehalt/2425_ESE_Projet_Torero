#include "tim.h"
#include "motor.h"

#define PWM_MAX_DUTY_CYCLE 40
#define FWD_GPIO_PIN_R GPIO_PIN_8  // PA8 -> FWD
#define REV_GPIO_PIN_R GPIO_PIN_9  // PA9 -> REV
#define FWD_GPIO_PIN_L GPIO_PIN_0  // PA0 -> FWD
#define REV_GPIO_PIN_L GPIO_PIN_1  // PA1 -> REV
#define FWD_GPIO_PORT_R GPIOA      // Port pour FWD_R
#define REV_GPIO_PORT_R GPIOA      // Port pour REV_R
#define FWD_GPIO_PORT_L GPIOA      // Port pour FWD_L
#define REV_GPIO_PORT_L GPIOA      // Port pour REV_L
#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64

void Motor_Forward_R(int percentage) {
    // Limiter le pourcentage entre 0 et 100
    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;

    // Mettre à jour les GPIO et PWM pour avancer (moteur droit)
    HAL_GPIO_WritePin(REV_GPIO_PORT_R, REV_GPIO_PIN_R, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FWD_GPIO_PORT_R, FWD_GPIO_PIN_R, GPIO_PIN_SET);
    TIM1->CCR1 = (percentage * PWM_MAX_DUTY_CYCLE) / 100;
    TIM1->CCR2 = 0;
}

void Motor_Reverse_R(int percentage) {
    // Limiter le pourcentage entre 0 et 100
    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;

    // Mettre à jour les GPIO et PWM pour reculer (moteur droit)
    HAL_GPIO_WritePin(FWD_GPIO_PORT_R, FWD_GPIO_PIN_R, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(REV_GPIO_PORT_R, REV_GPIO_PIN_R, GPIO_PIN_SET);
    TIM1->CCR2 = (percentage * PWM_MAX_DUTY_CYCLE) / 100;
    TIM1->CCR1 = 0;
}

void Motor_Forward_L(int percentage) {
    // Limiter le pourcentage entre 0 et 100
    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;

    // Mettre à jour les GPIO et PWM pour avancer (moteur gauche)
    HAL_GPIO_WritePin(REV_GPIO_PORT_L, REV_GPIO_PIN_L, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(FWD_GPIO_PORT_L, FWD_GPIO_PIN_L, GPIO_PIN_SET);
    TIM2->CCR2 = (percentage * PWM_MAX_DUTY_CYCLE) / 100;
    TIM2->CCR1 = 0;
}

void Motor_Reverse_L(int percentage) {
    // Limiter le pourcentage entre 0 et 100
    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;

    // Mettre à jour les GPIO et PWM pour reculer (moteur gauche)
    HAL_GPIO_WritePin(FWD_GPIO_PORT_L, FWD_GPIO_PIN_L, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(REV_GPIO_PORT_L, REV_GPIO_PIN_L, GPIO_PIN_SET);
    TIM2->CCR1 = (percentage * PWM_MAX_DUTY_CYCLE) / 100;
    TIM2->CCR2 = 0;
}

void Stop_Motors(void) {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
}

void Start_Motors(void) {
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM2->CCR1 = 0;
    TIM2->CCR2 = 0;
}
int32_t Read_Encoder(TIM_HandleTypeDef *htim) {
    return __HAL_TIM_GET_COUNTER(htim); // Lire la valeur actuelle du compteur
}

void Reset_Encoder(TIM_HandleTypeDef *htim) {
    __HAL_TIM_SET_COUNTER(htim, 0);    // Réinitialiser le compteur
}

void Motor_SetSpeed_R(int percentage) {
    static int currentSpeed_R = 0;  // Maintenir la vitesse actuelle (moteur droit)
    int step = (percentage > currentSpeed_R) ? 1 : -1; // Déterminer le pas (incrément ou décrément)

    // Limiter la cible au pourcentage maximal autorisé
    if (percentage > 100) percentage = 100;
    if (percentage < -100) percentage = -100;

    // Effectuer un ramp-up ou ramp-down jusqu'à la vitesse cible
    while (currentSpeed_R != percentage) {
        currentSpeed_R += step;

        if (currentSpeed_R > 0) {
            // Motor Forward
            HAL_GPIO_WritePin(REV_GPIO_PORT_R, REV_GPIO_PIN_R, GPIO_PIN_RESET); // Stop Reverse
            HAL_GPIO_WritePin(FWD_GPIO_PORT_R, FWD_GPIO_PIN_R, GPIO_PIN_SET);   // Start Forward
            TIM1->CCR1 = (PWM_MAX_DUTY_CYCLE * currentSpeed_R) / 100;           // Ajuster le rapport cyclique sur CCR1
        } else if (currentSpeed_R < 0) {
            // Motor Reverse
            HAL_GPIO_WritePin(FWD_GPIO_PORT_R, FWD_GPIO_PIN_R, GPIO_PIN_RESET); // Stop Forward
            HAL_GPIO_WritePin(REV_GPIO_PORT_R, REV_GPIO_PIN_R, GPIO_PIN_SET);   // Start Reverse
            TIM1->CCR2 = (PWM_MAX_DUTY_CYCLE * -currentSpeed_R) / 100;          // Ajuster le rapport cyclique sur CCR2
        } else {
            // Stop both directions
            HAL_GPIO_WritePin(FWD_GPIO_PORT_R, FWD_GPIO_PIN_R, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(REV_GPIO_PORT_R, REV_GPIO_PIN_R, GPIO_PIN_RESET);
            TIM1->CCR1 = 0;  // Désactiver Forward
            TIM1->CCR2 = 0;  // Désactiver Reverse
        }

        // Délai pour un changement progressif
        HAL_Delay(10);
    }
}

void Motor_SetSpeed_L(int percentage) {
    static int currentSpeed_L = 0;  // Maintenir la vitesse actuelle (moteur gauche)
    int step = (percentage > currentSpeed_L) ? 1 : -1; // Déterminer le pas (incrément ou décrément)

    // Limiter la cible au pourcentage maximal autorisé
    if (percentage > 100) percentage = 100;
    if (percentage < -100) percentage = -100;

    // Effectuer un ramp-up ou ramp-down jusqu'à la vitesse cible
    while (currentSpeed_L != percentage) {
        currentSpeed_L += step;

        if (currentSpeed_L > 0) {
            // Motor Forward
            HAL_GPIO_WritePin(REV_GPIO_PORT_L, REV_GPIO_PIN_L, GPIO_PIN_RESET); // Stop Reverse
            HAL_GPIO_WritePin(FWD_GPIO_PORT_L, FWD_GPIO_PIN_L, GPIO_PIN_SET);   // Start Forward
            TIM2->CCR1 = (PWM_MAX_DUTY_CYCLE * currentSpeed_L) / 100;           // Ajuster le rapport cyclique sur CCR1
        } else if (currentSpeed_L < 0) {
            // Motor Reverse
            HAL_GPIO_WritePin(FWD_GPIO_PORT_L, FWD_GPIO_PIN_L, GPIO_PIN_RESET); // Stop Forward
            HAL_GPIO_WritePin(REV_GPIO_PORT_L, REV_GPIO_PIN_L, GPIO_PIN_SET);   // Start Reverse
            TIM2->CCR2 = (PWM_MAX_DUTY_CYCLE * -currentSpeed_L) / 100;          // Ajuster le rapport cyclique sur CCR2
        } else {
            // Stop both directions
            HAL_GPIO_WritePin(FWD_GPIO_PORT_L, FWD_GPIO_PIN_L, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(REV_GPIO_PORT_L, REV_GPIO_PIN_L, GPIO_PIN_RESET);
            TIM2->CCR1 = 0;  // Désactiver Forward
            TIM2->CCR2 = 0;  // Désactiver Reverse
        }

        // Délai pour un changement progressif
        HAL_Delay(10);
    }
}
