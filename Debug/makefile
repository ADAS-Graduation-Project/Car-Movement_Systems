################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/stm32f4-hal/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cmsis/subdir.mk
-include src/COTS/MyRTOS/subdir.mk
-include src/COTS/MCAL/UART/subdir.mk
-include src/COTS/MCAL/TIM1/subdir.mk
-include src/COTS/MCAL/SysTick/subdir.mk
-include src/COTS/MCAL/RCC/subdir.mk
-include src/COTS/MCAL/NVIC/subdir.mk
-include src/COTS/MCAL/GPIO/subdir.mk
-include src/COTS/MCAL/EXTI/subdir.mk
-include src/COTS/HAL/UltraSonic/subdir.mk
-include src/COTS/HAL/LED/subdir.mk
-include src/COTS/HAL/DCMOTOR/subdir.mk
-include src/COTS/HAL/Car_Movement/subdir.mk
-include src/COTS/HAL/Bluetooth/subdir.mk
-include src/COTS/HAL/BUZZER/subdir.mk
-include src/COTS/APP/Traditional_Mode/subdir.mk
-include src/COTS/APP/NCC/subdir.mk
-include src/COTS/APP/Mob_APP/subdir.mk
-include src/COTS/APP/FCW/subdir.mk
-include src/COTS/APP/Exit_State/subdir.mk
-include src/COTS/APP/ACC/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CarMovementModes_Project.hex \

SECONDARY_SIZE += \
CarMovementModes_Project.siz \


# All Target
all: CarMovementModes_Project.elf secondary-outputs

# Tool invocations
CarMovementModes_Project.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"CarMovementModes_Project.map" --specs=nano.specs -o "CarMovementModes_Project.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

CarMovementModes_Project.hex: CarMovementModes_Project.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "CarMovementModes_Project.elf"  "CarMovementModes_Project.hex"
	@echo 'Finished building: $@'
	@echo ' '

CarMovementModes_Project.siz: CarMovementModes_Project.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "CarMovementModes_Project.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) CarMovementModes_Project.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
