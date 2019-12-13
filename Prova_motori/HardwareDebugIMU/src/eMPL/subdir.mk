################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/eMPL/inv_mpu.c 

COMPILER_OBJS += \
src/eMPL/inv_mpu.obj 

C_DEPS += \
src/eMPL/inv_mpu.d 

# Each subdirectory must supply rules for building sources it contributes
src/eMPL/%.obj: ../src/eMPL/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\eMPL\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\eMPL\cDepSubCommand.tmp"
	ccrx -subcommand="src\eMPL\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\eMPL\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\eMPL\cSubCommand.tmp"
	ccrx -subcommand="src\eMPL\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

