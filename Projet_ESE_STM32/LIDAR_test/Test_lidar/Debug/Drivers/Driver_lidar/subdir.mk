################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Driver_lidar/lidar.c 

OBJS += \
./Drivers/Driver_lidar/lidar.o 

C_DEPS += \
./Drivers/Driver_lidar/lidar.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Driver_lidar/%.o Drivers/Driver_lidar/%.su Drivers/Driver_lidar/%.cyclo: ../Drivers/Driver_lidar/%.c Drivers/Driver_lidar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Driver_lidar

clean-Drivers-2f-Driver_lidar:
	-$(RM) ./Drivers/Driver_lidar/lidar.cyclo ./Drivers/Driver_lidar/lidar.d ./Drivers/Driver_lidar/lidar.o ./Drivers/Driver_lidar/lidar.su

.PHONY: clean-Drivers-2f-Driver_lidar

