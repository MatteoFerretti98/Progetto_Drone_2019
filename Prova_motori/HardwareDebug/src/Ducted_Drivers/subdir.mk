################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/Ducted_Drivers/Motor.c \
../src/Ducted_Drivers/PID.c \
../src/Ducted_Drivers/map.c 

COMPILER_OBJS += \
src/Ducted_Drivers/Motor.obj \
src/Ducted_Drivers/PID.obj \
src/Ducted_Drivers/map.obj 

C_DEPS += \
src/Ducted_Drivers/Motor.d \
src/Ducted_Drivers/PID.d \
src/Ducted_Drivers/map.d 

# Each subdirectory must supply rules for building sources it contributes
src/Ducted_Drivers/%.obj: ../src/Ducted_Drivers/%.c src/Ducted_Drivers/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\Ducted_Drivers\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\Ducted_Drivers\cDepSubCommand.tmp"
	ccrx -subcommand="src\Ducted_Drivers\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\Ducted_Drivers\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\Ducted_Drivers\cSubCommand.tmp"
	ccrx -subcommand="src\Ducted_Drivers\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

