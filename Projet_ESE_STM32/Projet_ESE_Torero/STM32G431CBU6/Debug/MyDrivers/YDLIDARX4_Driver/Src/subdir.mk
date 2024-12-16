################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.c 

OBJS += \
./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.o 

C_DEPS += \
./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.d 


# Each subdirectory must supply rules for building sources it contributes
MyDrivers/YDLIDARX4_Driver/Src/%.o MyDrivers/YDLIDARX4_Driver/Src/%.su MyDrivers/YDLIDARX4_Driver/Src/%.cyclo: ../MyDrivers/YDLIDARX4_Driver/Src/%.c MyDrivers/YDLIDARX4_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../MyDrivers/YDLIDARX4_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyDrivers-2f-YDLIDARX4_Driver-2f-Src

clean-MyDrivers-2f-YDLIDARX4_Driver-2f-Src:
	-$(RM) ./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.cyclo ./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.d ./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.o ./MyDrivers/YDLIDARX4_Driver/Src/ydlidarx4_drv.su

.PHONY: clean-MyDrivers-2f-YDLIDARX4_Driver-2f-Src

