################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_glyph/src/r_glyph.c \
../generate/r_glyph/src/r_glyph_register.c 

COMPILER_OBJS += \
generate/r_glyph/src/r_glyph.obj \
generate/r_glyph/src/r_glyph_register.obj 

C_DEPS += \
generate/r_glyph/src/r_glyph.d \
generate/r_glyph/src/r_glyph_register.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_glyph/src/%.obj: ../generate/r_glyph/src/%.c generate/r_glyph/src/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_glyph\src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_glyph\src\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\cSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

