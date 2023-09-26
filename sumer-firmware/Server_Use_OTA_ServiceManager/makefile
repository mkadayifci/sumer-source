################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Stack/subdir.mk
-include Services/TagHeuer/subdir.mk
-include Services/Scribe/subdir.mk
-include Services/SPI/subdir.mk
-include Services/Flash/subdir.mk
-include Services/Debug/subdir.mk
-include Services/Accelerometer/subdir.mk
-include Platform_Layers/subdir.mk
-include OTA/subdir.mk
-include Library/subdir.mk
-include Initialize/subdir.mk
-include HAL/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
POST_BUILD_OBJCOPY += \
sumer-firmware_Server_Use_OTA_ServiceManager.hex \

POST_BUILD_PRINT_SIZE += \
elf-size \


# All Target
all: sumer-firmware_Server_Use_OTA_ServiceManager.elf secondary-outputs

# Tool invocations
sumer-firmware_Server_Use_OTA_ServiceManager.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -nostartfiles -nodefaultlibs -Wl,--gc-sections -Wl,-Map="sumer-firmware_Server_Use_OTA_ServiceManager.map" -T"../BlueNRG2.ld" -L../../sumer-library/Bluetooth_LE/library -L../../sumer-library/cryptolib -Wl,--cref --specs=nano.specs -o "sumer-firmware_Server_Use_OTA_ServiceManager.elf"  -Wl,--start-group @"objects.list" $(USER_OBJS) $(LIBS) -lm -lc  -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

sumer-firmware_Server_Use_OTA_ServiceManager.hex: sumer-firmware_Server_Use_OTA_ServiceManager.elf
	@echo 'Invoking: Generate binary'
	arm-none-eabi-objcopy -O ihex "sumer-firmware_Server_Use_OTA_ServiceManager.elf"  "sumer-firmware_Server_Use_OTA_ServiceManager.hex"
	@echo 'Finished building: $@'
	@echo ' '

elf-size: sumer-firmware_Server_Use_OTA_ServiceManager.elf
	@echo 'Invoking: Print Size'
	arm-none-eabi-size -B "sumer-firmware_Server_Use_OTA_ServiceManager.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(POST_BUILD_OBJCOPY)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(POST_BUILD_PRINT_SIZE)$(C_DEPS) sumer-firmware_Server_Use_OTA_ServiceManager.elf sumer-firmware_Server_Use_OTA_ServiceManager.bin *.map
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O binary sumer-firmware_Server_Use_OTA_ServiceManager.elf sumer-firmware_Server_Use_OTA_ServiceManager.bin
	-@echo ' '

secondary-outputs: $(POST_BUILD_OBJCOPY) $(POST_BUILD_PRINT_SIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
