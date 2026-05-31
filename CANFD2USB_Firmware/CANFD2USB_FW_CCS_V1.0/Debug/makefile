################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/gcc_arm_none_eabi_9_2_1

GEN_OPTS__FLAG := @"./device.opt" 
GEN_CMDS__FLAG := -Wl,-T"./device_linker.lds" 

ORDERED_OBJS += \
"./CANFD2USB_FW_CCS_V1.0.o" \
"./ti_msp_dl_config.o" \
"./startup_mspm0g350x_gcc.o" \
$(GEN_CMDS__FLAG) \
-Wl,-Tdevice.lds.genlibs \
-l:driverlib.a \
-lgcc \
-lc \
-lm \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
CANFD2USB_FW_CCS_V1.0.out 

EXE_OUTPUTS__QUOTED += \
"CANFD2USB_FW_CCS_V1.0.out" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "CANFD2USB_FW_CCS_V1.0.out"

# Tool invocations
CANFD2USB_FW_CCS_V1.0.out: $(OBJS) $(GEN_CMDS)
	@echo 'GNU Linker - building target: "$@"'
	"C:/ti/gcc_arm_none_eabi_9_2_1/bin/arm-none-eabi-gcc-9.2.1.exe" @"device.opt"  -O2 -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -mthumb -mfloat-abi=soft -Wl,-Map,"CANFD2USB_FW_CCS_V1.0.map" -nostartfiles -static -Wl,--gc-sections -L"C:/ti/mspm0_sdk_2_10_00_04/source/ti/driverlib/lib/gcc/m0p/mspm0g1x0x_g3x0x" -L"C:/ti/mspm0_sdk_2_10_00_04/source" -L"C:/Users/ARKC/workspace_ccstheia/CANFD2USB_FW_CCS_V1.0" -L"C:/Users/ARKC/workspace_ccstheia/CANFD2USB_FW_CCS_V1.0/Debug/syscfg" -L"C:/ti/gcc_arm_none_eabi_9_2_1/arm-none-eabi/lib/thumb/v6-m/nofp" -march=armv6-m -mthumb --specs=nosys.specs -o"CANFD2USB_FW_CCS_V1.0.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CANFD2USB_FW_CCS_V1.0.o" "ti_msp_dl_config.o" "startup_mspm0g350x_gcc.o" 
	-$(RM) "CANFD2USB_FW_CCS_V1.0.d" "ti_msp_dl_config.d" "startup_mspm0g350x_gcc.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

