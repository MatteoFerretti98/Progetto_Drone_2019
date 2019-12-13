################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/AHRS.c \
../src/CMT.c \
../src/PrintOnScreen.c \
../src/SetupAHRS.c \
../src/i2c.c \
../src/imu.c \
../src/mag.c \
../src/main.c 

COMPILER_OBJS += \
src/AHRS.obj \
src/CMT.obj \
src/PrintOnScreen.obj \
src/SetupAHRS.obj \
src/i2c.obj \
src/imu.obj \
src/mag.obj \
src/main.obj 

C_DEPS += \
src/AHRS.d \
src/CMT.d \
src/PrintOnScreen.d \
src/SetupAHRS.d \
src/i2c.d \
src/imu.d \
src/mag.d \
src/main.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

