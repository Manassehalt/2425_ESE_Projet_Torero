################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g431cbux.s 

OBJS += \
./Core/Startup/startup_stm32g431cbux.o 

S_DEPS += \
./Core/Startup/startup_stm32g431cbux.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/paul_/OneDrive/Bureau/ESE/2425_ESE_Projet_Torero/Projet_ESE_STM32/Projet_ESE_Torero/STM32G431CBU6/MyDrivers/YDLIDARX4_Driver/Inc" -I"C:/Users/paul_/OneDrive/Bureau/ESE/2425_ESE_Projet_Torero/Projet_ESE_STM32/Projet_ESE_Torero/STM32G431CBU6/MyDrivers" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g431cbux.d ./Core/Startup/startup_stm32g431cbux.o

.PHONY: clean-Core-2f-Startup

