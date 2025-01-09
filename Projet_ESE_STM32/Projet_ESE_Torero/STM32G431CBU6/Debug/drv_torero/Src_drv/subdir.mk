################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drv_torero/Src_drv/ADXL.c \
../drv_torero/Src_drv/motor.c \
../drv_torero/Src_drv/ydlidarx4_drv.c 

OBJS += \
./drv_torero/Src_drv/ADXL.o \
./drv_torero/Src_drv/motor.o \
./drv_torero/Src_drv/ydlidarx4_drv.o 

C_DEPS += \
./drv_torero/Src_drv/ADXL.d \
./drv_torero/Src_drv/motor.d \
./drv_torero/Src_drv/ydlidarx4_drv.d 


# Each subdirectory must supply rules for building sources it contributes
drv_torero/Src_drv/%.o drv_torero/Src_drv/%.su drv_torero/Src_drv/%.cyclo: ../drv_torero/Src_drv/%.c drv_torero/Src_drv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/paul_/OneDrive/Bureau/ESE/2425_ESE_Projet_Torero/Projet_ESE_STM32/Projet_ESE_Torero/STM32G431CBU6/drv_torero/Inc_drv" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drv_torero-2f-Src_drv

clean-drv_torero-2f-Src_drv:
	-$(RM) ./drv_torero/Src_drv/ADXL.cyclo ./drv_torero/Src_drv/ADXL.d ./drv_torero/Src_drv/ADXL.o ./drv_torero/Src_drv/ADXL.su ./drv_torero/Src_drv/motor.cyclo ./drv_torero/Src_drv/motor.d ./drv_torero/Src_drv/motor.o ./drv_torero/Src_drv/motor.su ./drv_torero/Src_drv/ydlidarx4_drv.cyclo ./drv_torero/Src_drv/ydlidarx4_drv.d ./drv_torero/Src_drv/ydlidarx4_drv.o ./drv_torero/Src_drv/ydlidarx4_drv.su

.PHONY: clean-drv_torero-2f-Src_drv

