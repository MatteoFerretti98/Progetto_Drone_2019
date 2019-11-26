################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_glyph/src/glyph/drivers/st7579_lcd.c 

COMPILER_OBJS += \
generate/r_glyph/src/glyph/drivers/st7579_lcd.obj 

C_DEPS += \
generate/r_glyph/src/glyph/drivers/st7579_lcd.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_glyph/src/glyph/drivers/%.obj: ../generate/r_glyph/src/glyph/drivers/%.c generate/r_glyph/src/glyph/drivers/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_glyph\src\glyph\drivers\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\drivers\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\drivers\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_glyph\src\glyph\drivers\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\drivers\cSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\drivers\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

