################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ADC.c \
../Src/LED.c \
../Src/PWM.c \
../Src/Systick_timer.c \
../Src/button.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/ADC.o \
./Src/LED.o \
./Src/PWM.o \
./Src/Systick_timer.o \
./Src/button.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/ADC.d \
./Src/LED.d \
./Src/PWM.d \
./Src/Systick_timer.d \
./Src/button.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DSTM32L476RGTx -c -I../Inc -I"C:/Users/MES.Academic/STM32CubeIDE/workspace_1.19.0/Final_Project_PWM/CMSIS/Include" -I"C:/Users/MES.Academic/STM32CubeIDE/workspace_1.19.0/Final_Project_PWM/CMSIS/Device/ST/STM32L4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ADC.cyclo ./Src/ADC.d ./Src/ADC.o ./Src/ADC.su ./Src/LED.cyclo ./Src/LED.d ./Src/LED.o ./Src/LED.su ./Src/PWM.cyclo ./Src/PWM.d ./Src/PWM.o ./Src/PWM.su ./Src/Systick_timer.cyclo ./Src/Systick_timer.d ./Src/Systick_timer.o ./Src/Systick_timer.su ./Src/button.cyclo ./Src/button.d ./Src/button.o ./Src/button.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

