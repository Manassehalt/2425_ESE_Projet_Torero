################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyDrivers/YDLIDARX4_Driver/ydlidarx4.c 

OBJS += \
./MyDrivers/YDLIDARX4_Driver/ydlidarx4.o 

C_DEPS += \
./MyDrivers/YDLIDARX4_Driver/ydlidarx4.d 


# Each subdirectory must supply rules for building sources it contributes
MyDrivers/YDLIDARX4_Driver/%.o MyDrivers/YDLIDARX4_Driver/%.su MyDrivers/YDLIDARX4_Driver/%.cyclo: ../MyDrivers/YDLIDARX4_Driver/%.c MyDrivers/YDLIDARX4_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../MyDrivers/YDLIDARX4_Driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyDrivers-2f-YDLIDARX4_Driver

clean-MyDrivers-2f-YDLIDARX4_Driver:
	-$(RM) ./MyDrivers/YDLIDARX4_Driver/ydlidarx4.cyclo ./MyDrivers/YDLIDARX4_Driver/ydlidarx4.d ./MyDrivers/YDLIDARX4_Driver/ydlidarx4.o ./MyDrivers/YDLIDARX4_Driver/ydlidarx4.su

.PHONY: clean-MyDrivers-2f-YDLIDARX4_Driver

