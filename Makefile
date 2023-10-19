####################################################################################################
# TTCONTROL                                                                                        #
# HY-TTC 500 Family                                                                                #
# Applications                                                                                     #
#                                                                                                  #
# Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.                    #
# Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com                               #
####################################################################################################

ROOT_DIR = .
BINARY   = app

ifeq ($(origin TARGET), undefined)
    TARGET_NOT_SPECIFIED = TRUE
    TARGET = TTC580
endif

# Includes
include $(ROOT_DIR)/env/settings.mk

# Check the TARGET specification
ifneq ($(MAKECMDGOALS), clean)
    ifdef TARGET_NOT_SPECIFIED
        $(info *****************************************************)
        $(info * Please specify the build target:)
        $(info *)
        $(info * TARGET=TTC580      for HY-TTC 580, 1st generation)
        $(info * TARGET=TTC540      for HY-TTC 540, 1st generation)
        $(info * TARGET=TTC520      for HY-TTC 520, 1st generation)
        $(info * TARGET=TTC510      for HY-TTC 510, 1st generation)
        $(info * TARGET=TTC580_GEN2 for HY-TTC 580, 2nd generation)
        $(info * TARGET=TTC540_GEN2 for HY-TTC 540, 2nd generation)
        $(info * TARGET=TTC520_GEN2 for HY-TTC 520, 2nd generation)
        $(info * TARGET=TTC510_GEN2 for HY-TTC 510, 2nd generation)
        $(info * TARGET=TTC590      for HY-TTC 590)
        $(info * TARGET=TTC590E     for HY-TTC 590E)
        $(info * TARGET=TTC508      for HY-TTC 508)
        $(info *)
        $(info * Example: make all TARGET=TTC580_GEN2)
        $(info *****************************************************)
        $(warning WARNING: Using default TARGET=$(TARGET))
    endif
    ifdef TARGET_UNKNOWN
        $(warning WARNING: Specified target unknown, using generic!)
    endif
endif

# Directories
# - build directory
BUILD_DIR     = $(ROOT_DIR)\build
BUILD_SDIR    = $(subst \,/,$(BUILD_DIR))
# - environment directory
ENV_DIR       = $(ROOT_DIR)\env
ENV_SDIR      = $(subst \,/,$(ENV_DIR))

# Application source and object files
APP_PATH      = $(subst \,/,$(ROOT_DIR)\src)
APP_SDIR      = $(subst \,/,$(APP_PATH))
APP_FILES     = $(basename $(wildcard $(APP_SDIR)/*.c))
APP_OBJ_FILES = $(addprefix $(BUILD_SDIR)/, $(notdir $(addsuffix .obj, $(APP_FILES))))

# Path settings
INCDIRS  = -I "$(ROOT_DIR)\inc"
INCDIRS += -I "$(C_COMP_PATH)\..\include"
INCDIRS += -I "$(APP_PATH)"

# Temporary linker command file
LINK_TMP_FILE = $(BUILD_SDIR)/link_tmp.cmd

all: prebuild postbuild

# Link
$(BUILD_SDIR)/$(BINARY).out: $(LIB_DIR)/$(LIB_NAME) $(LIB_DIR)/$(BSP_LIB_NAME) $(APP_OBJ_FILES)
	@echo Linking: $@
	@echo $(APP_OBJ_FILES)                                       > $(LINK_TMP_FILE)
	@echo -o $@                                                 >> $(LINK_TMP_FILE)
	@echo -l $(LIB_DIR)/$(LIB_NAME)                             >> $(LINK_TMP_FILE)
	@echo -l $(LIB_DIR)/$(BSP_LIB_NAME)                         >> $(LINK_TMP_FILE)
	@echo -l $(FPU_LIB)                                         >> $(LINK_TMP_FILE)
	@echo $(LINK_CMDFILE_BL)                                    >> $(LINK_TMP_FILE)
	@echo -m $(BUILD_SDIR)/$(BINARY).map                        >> $(LINK_TMP_FILE)
	@echo $(L_OPTS)                                             >> $(LINK_TMP_FILE)
	@"$(LINK)" --run_linker $(LINK_TMP_FILE)

# Compile
$(BUILD_SDIR)/%.obj: $(APP_PATH)/%.c
	@echo Compiling application files: $<
	@"$(CC)" $< $(INCDIRS) $(C_OPTS) --output_file $@ --asm_directory $(BUILD_SDIR) --obj_directory $(BUILD_SDIR)

clean:
	@echo Cleaning up application module files...
	@del /F /Q $(BUILD_DIR)\*.*
	@if exist AddAPDB.html del /F /Q AddAPDB.html
	@if exist nowECC.log del /F /Q nowECC.log
	@echo Done.

postbuild: $(BUILD_SDIR)/$(BINARY).out
	@echo  ----------------------------------------
	@echo   Creating .hex file with APDB...
	@echo  ----------------------------------------
	@if exist $(BUILD_DIR)\$(BINARY)_woAPDB.hex del /F /Q $(BUILD_DIR)\$(BINARY)_woAPDB.hex
	@"$(HEX)" --quiet --intel --romwidth=32 --fill=0xFFFFFFFF $(BUILD_SDIR)/$(BINARY).out -o $(BUILD_SDIR)/$(BINARY)_woAPDB.hex
	@if exist $(BUILD_DIR)\$(BINARY).hex del /F /Q $(BUILD_DIR)\$(BINARY).hex
	@"$(ENV_DIR)\AddAPDB.exe" -t $(DOWNLOADER_HW_TYPE) -v $(BOOTLOADER_VERSION) -w 32 -f I -k "$(ENV_DIR)\public.key" "$(ENV_DIR)\targets.xml" $(BUILD_DIR)\$(BINARY)_woAPDB.hex $(BUILD_DIR)\$(BINARY).hex
	@echo  ----------------------------------------
	@echo   Creating files for JTAG programming...
	@echo  ----------------------------------------
	@"$(SREC)" $(BUILD_SDIR)/$(BINARY).hex -Intel -fill 0xFF 0xA0000 0x300000 -O $(BUILD_SDIR)/$(BINARY)_jtag.hex -Intel
	@"$(ECC)" -i $(BUILD_SDIR)/$(BINARY)_jtag.hex -o $(BUILD_SDIR)/$(BINARY)_jtag_ecc.hex -F021 16M_ADD -r4 -d
	@echo  ------
	@echo   Done
	@echo  ------
	
prebuild:
	@echo  ----------------------------------------
	@echo   Checking CGT version...
	@echo  ----------------------------------------
	@"$(LINK)" --help > "$(BUILD_DIR)\cgt_version.tmp"
	@call "$(ENV_DIR)\cgt_version_check.bat" "$(BUILD_DIR)\cgt_version.tmp"
	@echo  ----------------------------------------
	@echo   Creating application...
	@echo  ----------------------------------------
	@echo Build target: $(TARGET_DESCRIPTION).
