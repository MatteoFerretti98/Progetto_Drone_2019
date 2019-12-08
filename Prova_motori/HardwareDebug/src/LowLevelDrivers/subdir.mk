################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/LowLevelDrivers/CMT.c \
../src/LowLevelDrivers/I2C.c \
../src/LowLevelDrivers/MTU_C3.c \
../src/LowLevelDrivers/MTU_C4.c \
../src/LowLevelDrivers/MTU_C5U.c \
../src/LowLevelDrivers/MTU_C5W.c 

COMPILER_OBJS += \
src/LowLevelDrivers/CMT.obj \
src/LowLevelDrivers/I2C.obj \
src/LowLevelDrivers/MTU_C3.obj \
src/LowLevelDrivers/MTU_C4.obj \
src/LowLevelDrivers/MTU_C5U.obj \
src/LowLevelDrivers/MTU_C5W.obj 

C_DEPS += \
src/LowLevelDrivers/CMT.d \
src/LowLevelDrivers/I2C.d \
src/LowLevelDrivers/MTU_C3.d \
src/LowLevelDrivers/MTU_C4.d \
src/LowLevelDrivers/MTU_C5U.d \
src/LowLevelDrivers/MTU_C5W.d 

# Each subdirectory must supply rules for building sources it contributes
src/LowLevelDrivers/%.obj: ../src/LowLevelDrivers/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\LowLevelDrivers\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\LowLevelDrivers\cDepSubCommand.tmp"
	ccrx -subcommand="src\LowLevelDrivers\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\LowLevelDrivers\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\LowLevelDrivers\cSubCommand.tmp"
	ccrx -subcommand="src\LowLevelDrivers\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

