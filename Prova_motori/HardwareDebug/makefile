################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs *.mi *.jmp *.fsy 1st/*.abs 1st/*.jmp 1st/*.fsy

# All of the sources participating in the build are defined here
-include sources.mk
-include generate/r_glyph/src/glyph/fonts/subdir.mk
-include src/subdir.mk
-include src/Ducted_Drivers/subdir.mk
-include generate/r_riic_rx600/src/subdir.mk
-include generate/r_glyph/src/glyph/subdir.mk
-include src/LowLevelDrivers/subdir.mk
-include generate/r_glyph/src/subdir.mk
-include generate/r_rspi_rx600/src/subdir.mk
-include generate/r_switches/src/subdir.mk
-include generate/r_bsp/board/rdkrx63n/subdir.mk
-include generate/r_glyph/src/glyph/drivers/subdir.mk

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
Prova_motori.abs

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
Prova_motori.lib

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(LINKER_OUTPUT_OUTPUTS) Prova_motori.x

	@echo 'Build complete.'

Prova_motori.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	@echo LinkerProva_motori.tmp=
	@sed -e "s/^/    /" "LinkerProva_motori.tmp"
	rlink -subcommand="LinkerProva_motori.tmp"
	@echo 'Finished building target:'
	@echo.


Prova_motori.lib: 
	@echo 'Invoking: Library Generator'
	@echo LibraryGeneratorProva_motori.tmp=
	@sed -e "s/^/    /" "LibraryGeneratorProva_motori.tmp"
	lbgrx -subcommand="LibraryGeneratorProva_motori.tmp"
	@echo 'Finished building:'
	@echo.

Prova_motori.x: Prova_motori.abs
		-C:\Renesas\e2_studio\Utilities\\ccrx\renesas_cc_converter.exe Prova_motori.abs Prova_motori.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	-rm -rf generate/r_glyph/src/glyph/fonts/*.obj generate/r_glyph/src/glyph/fonts/*.d generate/r_glyph/src/glyph/fonts/*.asm generate/r_glyph/src/glyph/fonts/*.p generate/r_glyph/src/glyph/fonts/*.pp generate/r_glyph/src/glyph/fonts/*.src generate/r_glyph/src/glyph/fonts/*.jmp generate/r_glyph/src/glyph/fonts/*.lst generate/r_glyph/src/glyph/fonts/*.prn generate/r_glyph/src/glyph/fonts/*.i 
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.pp src/*.src src/*.jmp src/*.lst src/*.prn src/*.i 
	-rm -rf src/Ducted_Drivers/*.obj src/Ducted_Drivers/*.d src/Ducted_Drivers/*.asm src/Ducted_Drivers/*.p src/Ducted_Drivers/*.pp src/Ducted_Drivers/*.src src/Ducted_Drivers/*.jmp src/Ducted_Drivers/*.lst src/Ducted_Drivers/*.prn src/Ducted_Drivers/*.i 
	-rm -rf generate/r_riic_rx600/src/*.obj generate/r_riic_rx600/src/*.d generate/r_riic_rx600/src/*.asm generate/r_riic_rx600/src/*.p generate/r_riic_rx600/src/*.pp generate/r_riic_rx600/src/*.src generate/r_riic_rx600/src/*.jmp generate/r_riic_rx600/src/*.lst generate/r_riic_rx600/src/*.prn generate/r_riic_rx600/src/*.i 
	-rm -rf generate/r_glyph/src/glyph/*.obj generate/r_glyph/src/glyph/*.d generate/r_glyph/src/glyph/*.asm generate/r_glyph/src/glyph/*.p generate/r_glyph/src/glyph/*.pp generate/r_glyph/src/glyph/*.src generate/r_glyph/src/glyph/*.jmp generate/r_glyph/src/glyph/*.lst generate/r_glyph/src/glyph/*.prn generate/r_glyph/src/glyph/*.i 
	-rm -rf src/LowLevelDrivers/*.obj src/LowLevelDrivers/*.d src/LowLevelDrivers/*.asm src/LowLevelDrivers/*.p src/LowLevelDrivers/*.pp src/LowLevelDrivers/*.src src/LowLevelDrivers/*.jmp src/LowLevelDrivers/*.lst src/LowLevelDrivers/*.prn src/LowLevelDrivers/*.i 
	-rm -rf generate/r_glyph/src/*.obj generate/r_glyph/src/*.d generate/r_glyph/src/*.asm generate/r_glyph/src/*.p generate/r_glyph/src/*.pp generate/r_glyph/src/*.src generate/r_glyph/src/*.jmp generate/r_glyph/src/*.lst generate/r_glyph/src/*.prn generate/r_glyph/src/*.i 
	-rm -rf generate/r_rspi_rx600/src/*.obj generate/r_rspi_rx600/src/*.d generate/r_rspi_rx600/src/*.asm generate/r_rspi_rx600/src/*.p generate/r_rspi_rx600/src/*.pp generate/r_rspi_rx600/src/*.src generate/r_rspi_rx600/src/*.jmp generate/r_rspi_rx600/src/*.lst generate/r_rspi_rx600/src/*.prn generate/r_rspi_rx600/src/*.i 
	-rm -rf generate/r_switches/src/*.obj generate/r_switches/src/*.d generate/r_switches/src/*.asm generate/r_switches/src/*.p generate/r_switches/src/*.pp generate/r_switches/src/*.src generate/r_switches/src/*.jmp generate/r_switches/src/*.lst generate/r_switches/src/*.prn generate/r_switches/src/*.i 
	-rm -rf generate/r_bsp/board/rdkrx63n/*.obj generate/r_bsp/board/rdkrx63n/*.d generate/r_bsp/board/rdkrx63n/*.asm generate/r_bsp/board/rdkrx63n/*.p generate/r_bsp/board/rdkrx63n/*.pp generate/r_bsp/board/rdkrx63n/*.src generate/r_bsp/board/rdkrx63n/*.jmp generate/r_bsp/board/rdkrx63n/*.lst generate/r_bsp/board/rdkrx63n/*.prn generate/r_bsp/board/rdkrx63n/*.i 
	-rm -rf generate/r_glyph/src/glyph/drivers/*.obj generate/r_glyph/src/glyph/drivers/*.d generate/r_glyph/src/glyph/drivers/*.asm generate/r_glyph/src/glyph/drivers/*.p generate/r_glyph/src/glyph/drivers/*.pp generate/r_glyph/src/glyph/drivers/*.src generate/r_glyph/src/glyph/drivers/*.jmp generate/r_glyph/src/glyph/drivers/*.lst generate/r_glyph/src/glyph/drivers/*.prn generate/r_glyph/src/glyph/drivers/*.i 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets