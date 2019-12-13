################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_glyph/src/glyph/glyph.c 

COMPILER_OBJS += \
generate/r_glyph/src/glyph/glyph.obj 

C_DEPS += \
generate/r_glyph/src/glyph/glyph.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_glyph/src/glyph/%.obj: ../generate/r_glyph/src/glyph/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_glyph\src\glyph\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_glyph\src\glyph\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\cSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

