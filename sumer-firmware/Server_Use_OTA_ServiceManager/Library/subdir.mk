################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_adc.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_dma.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_flash.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_gpio.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_i2c.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_mft.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_pka.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_radio.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_rng.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_rtc.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_spi.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_sysCtrl.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_timer.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_uart.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_wdg.c \
/Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/misc.c 

OBJS += \
./Library/BlueNRG1_adc.o \
./Library/BlueNRG1_dma.o \
./Library/BlueNRG1_flash.o \
./Library/BlueNRG1_gpio.o \
./Library/BlueNRG1_i2c.o \
./Library/BlueNRG1_mft.o \
./Library/BlueNRG1_pka.o \
./Library/BlueNRG1_radio.o \
./Library/BlueNRG1_rng.o \
./Library/BlueNRG1_rtc.o \
./Library/BlueNRG1_spi.o \
./Library/BlueNRG1_sysCtrl.o \
./Library/BlueNRG1_timer.o \
./Library/BlueNRG1_uart.o \
./Library/BlueNRG1_wdg.o \
./Library/misc.o 

C_DEPS += \
./Library/BlueNRG1_adc.d \
./Library/BlueNRG1_dma.d \
./Library/BlueNRG1_flash.d \
./Library/BlueNRG1_gpio.d \
./Library/BlueNRG1_i2c.d \
./Library/BlueNRG1_mft.d \
./Library/BlueNRG1_pka.d \
./Library/BlueNRG1_radio.d \
./Library/BlueNRG1_rng.d \
./Library/BlueNRG1_rtc.d \
./Library/BlueNRG1_spi.d \
./Library/BlueNRG1_sysCtrl.d \
./Library/BlueNRG1_timer.d \
./Library/BlueNRG1_uart.d \
./Library/BlueNRG1_wdg.d \
./Library/misc.d 


# Each subdirectory must supply rules for building sources it contributes
Library/BlueNRG1_adc.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_dma.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_flash.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_gpio.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_i2c.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_mft.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_mft.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_pka.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_pka.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_radio.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_radio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_rng.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_rng.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_rtc.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_spi.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_sysCtrl.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_sysCtrl.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_timer.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_uart.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/BlueNRG1_wdg.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/BlueNRG1_wdg.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/misc.o: /Users/mkadayifci/source/sumer-source/sumer-library/BlueNRG1_Periph_Driver/src/misc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


