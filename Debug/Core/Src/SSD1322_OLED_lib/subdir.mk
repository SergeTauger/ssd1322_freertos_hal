################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SSD1322_OLED_lib/SSD1322_API.c \
../Core/Src/SSD1322_OLED_lib/SSD1322_GFX.c \
../Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.c 

OBJS += \
./Core/Src/SSD1322_OLED_lib/SSD1322_API.o \
./Core/Src/SSD1322_OLED_lib/SSD1322_GFX.o \
./Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.o 

C_DEPS += \
./Core/Src/SSD1322_OLED_lib/SSD1322_API.d \
./Core/Src/SSD1322_OLED_lib/SSD1322_GFX.d \
./Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SSD1322_OLED_lib/%.o Core/Src/SSD1322_OLED_lib/%.su: ../Core/Src/SSD1322_OLED_lib/%.c Core/Src/SSD1322_OLED_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/SSD1322_OLED_lib" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/SSD1322_OLED_lib/Fonts" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos/include" -I"C:/tauger/coding/stm32cube/nucleo_l476_alt_ssd1322/Core/Src/FreeRtos/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-SSD1322_OLED_lib

clean-Core-2f-Src-2f-SSD1322_OLED_lib:
	-$(RM) ./Core/Src/SSD1322_OLED_lib/SSD1322_API.d ./Core/Src/SSD1322_OLED_lib/SSD1322_API.o ./Core/Src/SSD1322_OLED_lib/SSD1322_API.su ./Core/Src/SSD1322_OLED_lib/SSD1322_GFX.d ./Core/Src/SSD1322_OLED_lib/SSD1322_GFX.o ./Core/Src/SSD1322_OLED_lib/SSD1322_GFX.su ./Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.d ./Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.o ./Core/Src/SSD1322_OLED_lib/SSD1322_HW_Driver.su

.PHONY: clean-Core-2f-Src-2f-SSD1322_OLED_lib

