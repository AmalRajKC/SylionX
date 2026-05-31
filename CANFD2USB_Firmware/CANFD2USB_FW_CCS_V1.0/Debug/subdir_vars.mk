################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../CANFD2USB_FW_CCS_V1.0.syscfg 

C_SRCS += \
../CANFD2USB_FW_CCS_V1.0.c \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_2_10_00_04/source/ti/devices/msp/m0p/startup_system_files/gcc/startup_mspm0g350x_gcc.c 

GEN_CMDS += \
./device_linker.lds 

GEN_FILES += \
./device_linker.lds \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./CANFD2USB_FW_CCS_V1.0.d \
./ti_msp_dl_config.d \
./startup_mspm0g350x_gcc.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./CANFD2USB_FW_CCS_V1.0.o \
./ti_msp_dl_config.o \
./startup_mspm0g350x_gcc.o 

GEN_MISC_FILES += \
./device.lds.genlibs \
./ti_msp_dl_config.h 

OBJS__QUOTED += \
"CANFD2USB_FW_CCS_V1.0.o" \
"ti_msp_dl_config.o" \
"startup_mspm0g350x_gcc.o" 

GEN_MISC_FILES__QUOTED += \
"device.lds.genlibs" \
"ti_msp_dl_config.h" 

C_DEPS__QUOTED += \
"CANFD2USB_FW_CCS_V1.0.d" \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_gcc.d" 

GEN_FILES__QUOTED += \
"device_linker.lds" \
"device.opt" \
"ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../CANFD2USB_FW_CCS_V1.0.c" \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_2_10_00_04/source/ti/devices/msp/m0p/startup_system_files/gcc/startup_mspm0g350x_gcc.c" 

SYSCFG_SRCS__QUOTED += \
"../CANFD2USB_FW_CCS_V1.0.syscfg" 


