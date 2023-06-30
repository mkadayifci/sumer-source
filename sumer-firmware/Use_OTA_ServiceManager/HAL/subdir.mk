################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/context_switch.s 

C_SRCS += \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/clock.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/miscutil.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/osal.c \
/Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/sleep.c 

OBJS += \
./HAL/clock.o \
./HAL/context_switch.o \
./HAL/miscutil.o \
./HAL/osal.o \
./HAL/sleep.o 

S_DEPS += \
./HAL/context_switch.d 

C_DEPS += \
./HAL/clock.d \
./HAL/miscutil.d \
./HAL/osal.d \
./HAL/sleep.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/clock.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/clock.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"HAL/clock.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/context_switch.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/context_switch.s
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -x assembler-with-cpp -MMD -MP -MF"HAL/context_switch.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/miscutil.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/miscutil.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"HAL/miscutil.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/osal.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/osal.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"HAL/osal.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/sleep.o: /Users/mkadayifci/Documents/BlueNRG-1_2\ DK\ 3.2.3/Library/hal/src/sleep.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DLS_SOURCE=LS_SOURCE_EXTERNAL_32KHZ -DSMPS_INDUCTOR=SMPS_INDUCTOR_10uH -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -DBLUENRG2_DEVICE -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DUSER_BUTTON=BUTTON_1 -I../../../inc -I../../../../../../Library/hal/inc -I../../../../../../Library/BlueNRG1_Periph_Driver/inc -I../../../../../../Library/CMSIS/Device/ST/BlueNRG1/Include -I../../../../../../Library/CMSIS/Include -I../../../../../../Library/Bluetooth_LE/inc -I../../../../../../Library/BLE_Application/layers_inc -I../../../../../../Library/BLE_Application/Utils/inc -I../../../../../../Library/BLE_Application/OTA/inc -I../../../../../../Library/SDK_Eval_BlueNRG1/inc -MMD -MP -MF"HAL/sleep.d" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


