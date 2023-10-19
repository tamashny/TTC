SHELL=cmd.exe

ifeq ($(ROOT_DIR), )
    ROOT_DIR = .
endif

# List of supported build targets:
# DOWNLOADER_HW_TYPE defines the TTC-Downloader Hardware ID that will be written to the APDB.
ifeq ($(TARGET), TTC580)
    DOWNLOADER_HW_TYPE = 0x00400807
    TARGET_DESCRIPTION = HY-TTC 580, 1st generation
endif
ifeq ($(TARGET), TTC540)
    DOWNLOADER_HW_TYPE = 0x00600807
    TARGET_DESCRIPTION = HY-TTC 540, 1st generation
endif
ifeq ($(TARGET), TTC520)
    DOWNLOADER_HW_TYPE = 0x00A00807
    TARGET_DESCRIPTION = HY-TTC 520, 1st generation
endif
ifeq ($(TARGET), TTC510)
    DOWNLOADER_HW_TYPE = 0x00C00807
    TARGET_DESCRIPTION = HY-TTC 510, 1st generation
endif
ifeq ($(TARGET), TTC580_GEN2)
    DOWNLOADER_HW_TYPE = 0x00201007
    TARGET_DESCRIPTION = HY-TTC 580, 2nd generation
endif
ifeq ($(TARGET), TTC540_GEN2)
    DOWNLOADER_HW_TYPE = 0x00402007
    TARGET_DESCRIPTION = HY-TTC 540, 2nd generation
endif
ifeq ($(TARGET), TTC520_GEN2)
    DOWNLOADER_HW_TYPE = 0x00602007
    TARGET_DESCRIPTION = HY-TTC 520, 2nd generation
endif
ifeq ($(TARGET), TTC510_GEN2)
    DOWNLOADER_HW_TYPE = 0x00802007
    TARGET_DESCRIPTION = HY-TTC 510, 2nd generation
endif
ifeq ($(TARGET), TTC590)
    DOWNLOADER_HW_TYPE = 0x00201C07
    TARGET_DESCRIPTION = HY-TTC 590
endif
ifeq ($(TARGET), TTC590E)
    DOWNLOADER_HW_TYPE = 0x00401C07
    TARGET_DESCRIPTION = HY-TTC 590E
endif
ifeq ($(TARGET), TTC508)
    DOWNLOADER_HW_TYPE = 0x00201807
    TARGET_DESCRIPTION = HY-TTC 508
endif
ifndef DOWNLOADER_HW_TYPE
    TARGET_UNKNOWN     = TRUE
    DOWNLOADER_HW_TYPE = 0x00200807
    TARGET_DESCRIPTION = HY-TTC 5xx Generic (all variant-specific memory regions disabled)
endif

# Bootloader version
BOOTLOADER_VERSION = 3.0

# Board Support Package
BSP_DIR = $(subst \,/,$(ROOT_DIR)\bsp)

# output files
OUTPUT_DIR  = $(subst \,/,$(CURDIR)\build)
OUTPUT_FILE = $(OUTPUT_DIR)/app.out

# library directories
LIB_DIR      = $(subst \,/,$(ROOT_DIR)\lib)
LIB_NAME     = ttc500.lib
BSP_LIB_NAME = bsp.lib

# linker command file
LINK_CMDFILE_BL = $(BSP_DIR)/mem_ttc500_bl.lsf

# path with C compiler
ifndef C_COMP_PATH
    C_COMP_PATH = C:\ti\ccs1240\ccs\tools\compiler\arm_5.1.6\bin
endif

# path for the nowECC tool
ECC_PATH = $(ROOT_DIR)/env

#path for the srecord tool
SREC_PATH = $(ROOT_DIR)/env

# path with Run-Time Support Libraries
C_RTS_LIB_PATH = $(C_COMP_PATH)/../lib

#-----------------------------------------------------------------------------
# optimization / debugging features
#-----------------------------------------------------------------------------
# compile code for profiling or debugging
#   0 ... profiling
#   1 ... debugging
#   2 ... release
OPT_TYPE = 1

ifeq ($(OPT_TYPE), 0)
    # optimize for profiling
    OPT_STR := PROFILING
endif
ifeq ($(OPT_TYPE), 1)
    # optimize for debugging
    OPT_STR := DEBUGGING
endif
ifeq ($(OPT_TYPE), 2)
    # optimize for release
    OPT_STR := RELEASE
endif

USE_FPU = 1

#-----------------------------------------------------------------------------
# compiler options:
#-----------------------------------------------------------------------------
ifeq ($(OPT_STR), PROFILING)
    # optimize for profiling
    OPT_SYMDEBUG   := skeletal
    OPT_LEVEL      := 2
    OPT_ADDITIONAL := --optimize_with_debug --symdebug:dwarf_version=2
endif
ifeq ($(OPT_STR), DEBUGGING)
    # optimize for debugging
    OPT_SYMDEBUG   := dwarf
    OPT_LEVEL      := 0
    OPT_ADDITIONAL := -dDEBUG -dCSM_DEBUG --symdebug:dwarf_version=2
endif
ifeq ($(OPT_STR), RELEASE)
    # optimize for release
    OPT_SYMDEBUG   := none
    OPT_LEVEL      := 2
    OPT_ADDITIONAL := -dRELEASE
endif

ifeq ($(USE_FPU),1)
    FPUSWITCH = --float_support=VFPv3D16
    FPU_LIB   = "$(C_RTS_LIB_PATH)/rtsv7R4_T_be_v3D16_eabi.lib"
else
    FPUSWITCH = -dosdNoFPU
    FPU_LIB   = "$(C_RTS_LIB_PATH)/rtsv7R4_T_be_eabi.lib"
endif

#-----------------------------------------------------------------------------
# Defines for tools
#-----------------------------------------------------------------------------
CC   = $(C_COMP_PATH)\armcl.exe
AS   = $(C_COMP_PATH)\armcl.exe
AR   = $(C_COMP_PATH)\armar.exe
LINK = $(C_COMP_PATH)\armcl.exe
HEX  = $(C_COMP_PATH)\armhex.exe
ECC  = $(ECC_PATH)\nowECC.exe
SREC = $(SREC_PATH)\srec_cat.exe

#-----------------------------------------------------------------------------
# compiler/linker options
#-----------------------------------------------------------------------------

# explicit compiler options
C_OPTS      :=  -mv7R4                         \
                --abi=eabi                     \
                --auto_inline=0                \
                --endian=big                   \
                $(FPUSWITCH)                   \
                --plain_char=unsigned          \
                --small_enum                   \
                --fp_mode=strict               \
                --opt_level=$(OPT_LEVEL)       \
                --opt_for_speed=3              \
                --code_state=32                \
                --no_inlining                  \
                --gen_func_subsections         \
                --check_misra=none             \
                --issue_remarks                \
                --display_error_number         \
                --quiet

# compiler options for debugging
C_OPTS +=       --symdebug:$(OPT_SYMDEBUG)     \
                $(OPT_ADDITIONAL)

# recommended compiler modifications
C_OPTS +=       --aliased_variables            \
                --no_stm                       \
                --unaligned_access=off
#               --no_intrinsics (deactivated)
#               --strict_ansi (deactivated)

# compiler symbol definitions via command line
C_OPTS +=       -dHET_v2                      \
                -d_AEABI_PORTABILITY_LEVEL=0

# insert the build date
BUILDDATE_F    = $(ROOT_DIR)\env\builddate\getbuilddate.exe
BUILDDATE_FUNC = $(subst \,\\,$(BUILDDATE_F))
DATE_S         = $(shell $(BUILDDATE_FUNC))

BUILD_DAY   = $(word 1, $(DATE_S))
BUILD_MONTH = $(word 2, $(DATE_S))
BUILD_YEAR  = $(word 3, $(DATE_S))
BUILD_HOUR  = $(word 4, $(DATE_S))
BUILD_MIN   = $(word 5, $(DATE_S))

C_OPTS += -DRTS_TTC_FLASH_DATE_YEAR=$(BUILD_YEAR)
C_OPTS += -DRTS_TTC_FLASH_DATE_MONTH=$(BUILD_MONTH)
C_OPTS += -DRTS_TTC_FLASH_DATE_DAY=$(BUILD_DAY)
C_OPTS += -DRTS_TTC_FLASH_DATE_HOUR=$(BUILD_HOUR)
C_OPTS += -DRTS_TTC_FLASH_DATE_MINUTE=$(BUILD_MIN)

# explicit assembler options
A_OPTS      :=  -quiet                        \
                --abi=eabi                    \
                -mv7r4                        \
                $(FPUSWITCH)                  \
                --endian=big                  \
                --symdebug:$(OPT_SYMDEBUG)

# explicit linker options
L_OPTS      :=  --reread_libs                 \
                --rom_model                   \
                --warn_sections               \
                --be32                        \
                --mapfile_contents=all        \
                --issue_remarks               \
                --display_error_number

# recommended linker modifications
L_OPTS      +=  --disable_auto_rts            \
                --fill_value=0                \
                --minimize_trampolines        \
                --diag_suppress=10068
#               --scan_libraries
