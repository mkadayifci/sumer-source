################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Services/Flash/flash_service.c 

OBJS += \
./Services/Flash/flash_service.o 

C_DEPS += \
./Services/Flash/flash_service.d 


# Each subdirectory must supply rules for building sources it contributes
Services/Flash/%.o: ../Services/Flash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -Os -ffunction-sections -fdata-sections -g3 -std=c99 -DHS_SPEED_XTAL=HS_SPEED_XTAL_32MHZ -DSERVER=1 -DLS_SOURCE=LS_SOURCE_INTERNAL_RO -DSMPS_INDUCTOR=SMPS_INDUCTOR_NONE -DBLUENRG2_DEVICE -DUSER_BUTTON=BUTTON_1 -DBLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION -DST_USE_OTA_SERVICE_MANAGER_APPLICATION=1 -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/inc" -I"/Users/mkadayifci/source/sumer-source/sumer-firmware/Services/Include" -I../../sumer-library/hal/inc -I../../sumer-library/BlueNRG1_Periph_Driver/inc -I../../sumer-library/SDK_Eval_BlueNRG1/inc -I../../sumer-library/BLE_Application/OTA/inc -I../../sumer-library/CMSIS/Device/ST/BlueNRG1/Include -I../../sumer-library/BLE_Application/Utils/inc -I../../sumer-library/CMSIS/Include -I../../sumer-library/BLE_Application/layers_inc -I../../sumer-library/Bluetooth_LE/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


