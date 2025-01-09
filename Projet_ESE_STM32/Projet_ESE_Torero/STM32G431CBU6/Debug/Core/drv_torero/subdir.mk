################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/drv_torero/motor.c 

OBJS += \
./Core/drv_torero/motor.o 

C_DEPS += \
./Core/drv_torero/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/drv_torero/%.o Core/drv_torero/%.su Core/drv_torero/%.cyclo: ../Core/drv_torero/%.c Core/drv_torero/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../drv_torero -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-drv_torero

clean-Core-2f-drv_torero:
	-$(RM) ./Core/drv_torero/motor.cyclo ./Core/drv_torero/motor.d ./Core/drv_torero/motor.o ./Core/drv_torero/motor.su

.PHONY: clean-Core-2f-drv_torero

