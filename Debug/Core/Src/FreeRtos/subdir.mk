################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/FreeRtos/croutine.c \
../Core/Src/FreeRtos/event_groups.c \
../Core/Src/FreeRtos/freertos.c \
../Core/Src/FreeRtos/list.c \
../Core/Src/FreeRtos/queue.c \
../Core/Src/FreeRtos/stream_buffer.c \
../Core/Src/FreeRtos/tasks.c \
../Core/Src/FreeRtos/timers.c 

OBJS += \
./Core/Src/FreeRtos/croutine.o \
./Core/Src/FreeRtos/event_groups.o \
./Core/Src/FreeRtos/freertos.o \
./Core/Src/FreeRtos/list.o \
./Core/Src/FreeRtos/queue.o \
./Core/Src/FreeRtos/stream_buffer.o \
./Core/Src/FreeRtos/tasks.o \
./Core/Src/FreeRtos/timers.o 

C_DEPS += \
./Core/Src/FreeRtos/croutine.d \
./Core/Src/FreeRtos/event_groups.d \
./Core/Src/FreeRtos/freertos.d \
./Core/Src/FreeRtos/list.d \
./Core/Src/FreeRtos/queue.d \
./Core/Src/FreeRtos/stream_buffer.d \
./Core/Src/FreeRtos/tasks.d \
./Core/Src/FreeRtos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/FreeRtos/%.o Core/Src/FreeRtos/%.su: ../Core/Src/FreeRtos/%.c Core/Src/FreeRtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/SSD1322_OLED_lib" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/SSD1322_OLED_lib/Fonts" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos/include" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-FreeRtos

clean-Core-2f-Src-2f-FreeRtos:
	-$(RM) ./Core/Src/FreeRtos/croutine.d ./Core/Src/FreeRtos/croutine.o ./Core/Src/FreeRtos/croutine.su ./Core/Src/FreeRtos/event_groups.d ./Core/Src/FreeRtos/event_groups.o ./Core/Src/FreeRtos/event_groups.su ./Core/Src/FreeRtos/freertos.d ./Core/Src/FreeRtos/freertos.o ./Core/Src/FreeRtos/freertos.su ./Core/Src/FreeRtos/list.d ./Core/Src/FreeRtos/list.o ./Core/Src/FreeRtos/list.su ./Core/Src/FreeRtos/queue.d ./Core/Src/FreeRtos/queue.o ./Core/Src/FreeRtos/queue.su ./Core/Src/FreeRtos/stream_buffer.d ./Core/Src/FreeRtos/stream_buffer.o ./Core/Src/FreeRtos/stream_buffer.su ./Core/Src/FreeRtos/tasks.d ./Core/Src/FreeRtos/tasks.o ./Core/Src/FreeRtos/tasks.su ./Core/Src/FreeRtos/timers.d ./Core/Src/FreeRtos/timers.o ./Core/Src/FreeRtos/timers.su

.PHONY: clean-Core-2f-Src-2f-FreeRtos

