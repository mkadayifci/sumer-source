################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Button.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Com.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Config.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Led.c 

OBJS += \
./Platform_Layers/SDK_EVAL_Button.o \
./Platform_Layers/SDK_EVAL_Com.o \
./Platform_Layers/SDK_EVAL_Config.o \
./Platform_Layers/SDK_EVAL_Led.o 

C_DEPS += \
./Platform_Layers/SDK_EVAL_Button.d \
./Platform_Layers/SDK_EVAL_Com.d \
./Platform_Layers/SDK_EVAL_Config.d \
./Platform_Layers/SDK_EVAL_Led.d 


# Each subdirectory must supply rules for building sources it contributes
Platform_Layers/SDK_EVAL_Button.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Button.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"Platform_Layers/SDK_EVAL_Button.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform_Layers/SDK_EVAL_Com.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Com.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"Platform_Layers/SDK_EVAL_Com.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform_Layers/SDK_EVAL_Config.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Config.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"Platform_Layers/SDK_EVAL_Config.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platform_Layers/SDK_EVAL_Led.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/SDK_Eval_BlueNRG1/src/SDK_EVAL_Led.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"Platform_Layers/SDK_EVAL_Led.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


