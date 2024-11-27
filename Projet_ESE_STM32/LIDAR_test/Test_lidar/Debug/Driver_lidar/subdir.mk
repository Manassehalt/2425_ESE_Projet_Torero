################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver_lidar/lidar.c 

OBJS += \
./Driver_lidar/lidar.o 

C_DEPS += \
./Driver_lidar/lidar.d 


# Each subdirectory must supply rules for building sources it contributes
Driver_lidar/%.o Driver_lidar/%.su Driver_lidar/%.cyclo: ../Driver_lidar/%.c Driver_lidar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Driver_lidar

clean-Driver_lidar:
	-$(RM) ./Driver_lidar/lidar.cyclo ./Driver_lidar/lidar.d ./Driver_lidar/lidar.o ./Driver_lidar/lidar.su

.PHONY: clean-Driver_lidar

