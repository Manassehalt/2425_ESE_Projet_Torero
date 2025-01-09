

#ifndef DRV_TORERO_MOTOR_H_
#define DRV_TORERO_MOTOR_H_

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
