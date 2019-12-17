################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_glyph/src/glyph/fonts/bitmap_font.c \
../generate/r_glyph/src/glyph/fonts/font_8x16.c \
../generate/r_glyph/src/glyph/fonts/font_8x8.c \
../generate/r_glyph/src/glyph/fonts/font_helvr10.c \
../generate/r_glyph/src/glyph/fonts/font_winfreesystem14x16.c \
../generate/r_glyph/src/glyph/fonts/font_x5x7.c \
../generate/r_glyph/src/glyph/fonts/font_x6x13.c 

COMPILER_OBJS += \
generate/r_glyph/src/glyph/fonts/bitmap_font.obj \
generate/r_glyph/src/glyph/fonts/font_8x16.obj \
generate/r_glyph/src/glyph/fonts/font_8x8.obj \
generate/r_glyph/src/glyph/fonts/font_helvr10.obj \
generate/r_glyph/src/glyph/fonts/font_winfreesystem14x16.obj \
generate/r_glyph/src/glyph/fonts/font_x5x7.obj \
generate/r_glyph/src/glyph/fonts/font_x6x13.obj 

C_DEPS += \
generate/r_glyph/src/glyph/fonts/bitmap_font.d \
generate/r_glyph/src/glyph/fonts/font_8x16.d \
generate/r_glyph/src/glyph/fonts/font_8x8.d \
generate/r_glyph/src/glyph/fonts/font_helvr10.d \
generate/r_glyph/src/glyph/fonts/font_winfreesystem14x16.d \
generate/r_glyph/src/glyph/fonts/font_x5x7.d \
generate/r_glyph/src/glyph/fonts/font_x6x13.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_glyph/src/glyph/fonts/%.obj: ../generate/r_glyph/src/glyph/fonts/%.c generate/r_glyph/src/glyph/fonts/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_glyph\src\glyph\fonts\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\fonts\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\fonts\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_glyph\src\glyph\fonts\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_glyph\src\glyph\fonts\cSubCommand.tmp"
	ccrx -subcommand="generate\r_glyph\src\glyph\fonts\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

