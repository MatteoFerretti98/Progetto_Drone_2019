################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include generate/r_glyph/src/glyph/drivers/subdir.mk
-include src/eMPL/subdir.mk
-include generate/r_glyph/src/glyph/subdir.mk
-include generate/r_glyph/src/glyph/fonts/subdir.mk
-include generate/r_riic_rx600/src/subdir.mk
-include generate/r_bsp/board/rdkrx63n/subdir.mk
-include generate/r_glyph/src/subdir.mk
-include generate/r_rspi_rx600/src/subdir.mk
-include src/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif 
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif 
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif 
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif 
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
AHRS.abs \
AHRS.x

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
AHRS.lib

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

AHRS.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	@echo LinkerAHRS.tmp=
	@sed -e "s/^/    /" "LinkerAHRS.tmp"
	rlink -subcommand="LinkerAHRS.tmp"
	@echo 'Finished building target:'
	@echo.


AHRS.lib: 
	@echo 'Invoking: Library Generator'
	@echo LibraryGeneratorAHRS.tmp=
	@sed -e "s/^/    /" "LibraryGeneratorAHRS.tmp"
	lbgrx -subcommand="LibraryGeneratorAHRS.tmp"
	@echo 'Finished building:'
	@echo.

AHRS.x: AHRS.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe AHRS.abs AHRS.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	-rm -rf generate/r_glyph/src/glyph/drivers/*.obj generate/r_glyph/src/glyph/drivers/*.d generate/r_glyph/src/glyph/drivers/*.asm generate/r_glyph/src/glyph/drivers/*.p generate/r_glyph/src/glyph/drivers/*.src generate/r_glyph/src/glyph/drivers/*.jmp generate/r_glyph/src/glyph/drivers/*.lst generate/r_glyph/src/glyph/drivers/*.prn 
	-rm -rf src/eMPL/*.obj src/eMPL/*.d src/eMPL/*.asm src/eMPL/*.p src/eMPL/*.src src/eMPL/*.jmp src/eMPL/*.lst src/eMPL/*.prn 
	-rm -rf generate/r_glyph/src/glyph/*.obj generate/r_glyph/src/glyph/*.d generate/r_glyph/src/glyph/*.asm generate/r_glyph/src/glyph/*.p generate/r_glyph/src/glyph/*.src generate/r_glyph/src/glyph/*.jmp generate/r_glyph/src/glyph/*.lst generate/r_glyph/src/glyph/*.prn 
	-rm -rf generate/r_glyph/src/glyph/fonts/*.obj generate/r_glyph/src/glyph/fonts/*.d generate/r_glyph/src/glyph/fonts/*.asm generate/r_glyph/src/glyph/fonts/*.p generate/r_glyph/src/glyph/fonts/*.src generate/r_glyph/src/glyph/fonts/*.jmp generate/r_glyph/src/glyph/fonts/*.lst generate/r_glyph/src/glyph/fonts/*.prn 
	-rm -rf generate/r_riic_rx600/src/*.obj generate/r_riic_rx600/src/*.d generate/r_riic_rx600/src/*.asm generate/r_riic_rx600/src/*.p generate/r_riic_rx600/src/*.src generate/r_riic_rx600/src/*.jmp generate/r_riic_rx600/src/*.lst generate/r_riic_rx600/src/*.prn 
	-rm -rf generate/r_bsp/board/rdkrx63n/*.obj generate/r_bsp/board/rdkrx63n/*.d generate/r_bsp/board/rdkrx63n/*.asm generate/r_bsp/board/rdkrx63n/*.p generate/r_bsp/board/rdkrx63n/*.src generate/r_bsp/board/rdkrx63n/*.jmp generate/r_bsp/board/rdkrx63n/*.lst generate/r_bsp/board/rdkrx63n/*.prn 
	-rm -rf generate/r_glyph/src/*.obj generate/r_glyph/src/*.d generate/r_glyph/src/*.asm generate/r_glyph/src/*.p generate/r_glyph/src/*.src generate/r_glyph/src/*.jmp generate/r_glyph/src/*.lst generate/r_glyph/src/*.prn 
	-rm -rf generate/r_rspi_rx600/src/*.obj generate/r_rspi_rx600/src/*.d generate/r_rspi_rx600/src/*.asm generate/r_rspi_rx600/src/*.p generate/r_rspi_rx600/src/*.src generate/r_rspi_rx600/src/*.jmp generate/r_rspi_rx600/src/*.lst generate/r_rspi_rx600/src/*.prn 
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.src src/*.jmp src/*.lst src/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets