################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_riic_rx600/src/r_riic_rx600.c \
../generate/r_riic_rx600/src/r_riic_rx600_master.c 

COMPILER_OBJS += \
generate/r_riic_rx600/src/r_riic_rx600.obj \
generate/r_riic_rx600/src/r_riic_rx600_master.obj 

C_DEPS += \
generate/r_riic_rx600/src/r_riic_rx600.d \
generate/r_riic_rx600/src/r_riic_rx600_master.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_riic_rx600/src/%.obj: ../generate/r_riic_rx600/src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_riic_rx600\src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_riic_rx600\src\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_riic_rx600\src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_riic_rx600\src\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_riic_rx600\src\cSubCommand.tmp"
	ccrx -subcommand="generate\r_riic_rx600\src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

