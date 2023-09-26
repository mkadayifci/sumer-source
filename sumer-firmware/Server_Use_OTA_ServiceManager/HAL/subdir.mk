################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/context_switch.s 

C_SRCS += \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/clock.c \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/gp_timer.c \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/miscutil.c \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/osal.c \
/Users/mkadayifci/source/sumer-source/sumer-library/hal/src/sleep.c 

OBJS += \
./HAL/clock.o \
./HAL/context_switch.o \
./HAL/gp_timer.o \
./HAL/miscutil.o \
./HAL/osal.o \
./HAL/sleep.o 

S_DEPS += \
./HAL/context_switch.d 

C_DEPS += \
./HAL/clock.d \
./HAL/gp_timer.d \
./HAL/miscutil.d \
./HAL/osal.d \
./HAL/sleep.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/clock.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/clock.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/context_switch.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/context_switch.s
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/gp_timer.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/gp_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/miscutil.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/miscutil.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/osal.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/osal.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/sleep.o: /Users/mkadayifci/source/sumer-source/sumer-library/hal/src/sleep.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


