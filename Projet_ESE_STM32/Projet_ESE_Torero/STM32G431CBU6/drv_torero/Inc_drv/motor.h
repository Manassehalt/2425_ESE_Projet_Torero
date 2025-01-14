

#ifndef DRV_TORERO_MOTOR_H_
#define DRV_TORERO_MOTOR_H_

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

extern int motor_init;

void Start_Motors(void);
void Stop_Motors(void);
void Motor_Forward(int percentage);
void Motor_Reverse(int percentage);
void Motor_SetSpeed(int percentage);
void Motor_SetSpeed_R(int percentage);
void Motor_SetSpeed_L(int percentage);
void Motor_Reverse_R(int percentage);
void Motor_Reverse_L(int percentage);
void Motor_Forward_R(int percentage);
void Motor_Forward_L(int percentage);

#endif /* DRV_TORERO_MOTOR_H_ */
