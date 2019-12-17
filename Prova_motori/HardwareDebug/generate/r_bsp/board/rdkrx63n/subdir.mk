################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/r_bsp/board/rdkrx63n/dbsct.c \
../generate/r_bsp/board/rdkrx63n/flash_options.c \
../generate/r_bsp/board/rdkrx63n/hwsetup.c \
../generate/r_bsp/board/rdkrx63n/lcd.c \
../generate/r_bsp/board/rdkrx63n/lowsrc.c \
../generate/r_bsp/board/rdkrx63n/resetprg.c \
../generate/r_bsp/board/rdkrx63n/sbrk.c \
../generate/r_bsp/board/rdkrx63n/vecttbl.c 

SRC_SRCS += \
../generate/r_bsp/board/rdkrx63n/lowlvl.src 

SRC_DEPS += \
generate/r_bsp/board/rdkrx63n/lowlvl.d 

ASSEMBLER_OBJS += \
generate/r_bsp/board/rdkrx63n/lowlvl.obj 

COMPILER_OBJS += \
generate/r_bsp/board/rdkrx63n/dbsct.obj \
generate/r_bsp/board/rdkrx63n/flash_options.obj \
generate/r_bsp/board/rdkrx63n/hwsetup.obj \
generate/r_bsp/board/rdkrx63n/lcd.obj \
generate/r_bsp/board/rdkrx63n/lowsrc.obj \
generate/r_bsp/board/rdkrx63n/resetprg.obj \
generate/r_bsp/board/rdkrx63n/sbrk.obj \
generate/r_bsp/board/rdkrx63n/vecttbl.obj 

C_DEPS += \
generate/r_bsp/board/rdkrx63n/dbsct.d \
generate/r_bsp/board/rdkrx63n/flash_options.d \
generate/r_bsp/board/rdkrx63n/hwsetup.d \
generate/r_bsp/board/rdkrx63n/lcd.d \
generate/r_bsp/board/rdkrx63n/lowsrc.d \
generate/r_bsp/board/rdkrx63n/resetprg.d \
generate/r_bsp/board/rdkrx63n/sbrk.d \
generate/r_bsp/board/rdkrx63n/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
generate/r_bsp/board/rdkrx63n/%.obj: ../generate/r_bsp/board/rdkrx63n/%.c generate/r_bsp/board/rdkrx63n/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo generate\r_bsp\board\rdkrx63n\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_bsp\board\rdkrx63n\cDepSubCommand.tmp"
	ccrx -subcommand="generate\r_bsp\board\rdkrx63n\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_bsp\board\rdkrx63n\cSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_bsp\board\rdkrx63n\cSubCommand.tmp"
	ccrx -subcommand="generate\r_bsp\board\rdkrx63n\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

generate/r_bsp/board/rdkrx63n/%.obj: ../generate/r_bsp/board/rdkrx63n/%.src 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Assembler'
	@echo generate\r_bsp\board\rdkrx63n\srcDepSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_bsp\board\rdkrx63n\srcDepSubCommand.tmp"
	asrx -subcommand="generate\r_bsp\board\rdkrx63n\srcDepSubCommand.tmp" -MF="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	@echo generate\r_bsp\board\rdkrx63n\srcSubCommand.tmp=
	@sed -e "s/^/ /" "generate\r_bsp\board\rdkrx63n\srcSubCommand.tmp"
	asrx -subcommand="generate\r_bsp\board\rdkrx63n\srcSubCommand.tmp" -output="$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

