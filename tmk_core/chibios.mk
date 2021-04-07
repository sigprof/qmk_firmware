# Hey Emacs, this is a -*- makefile -*-
##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x800
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Imported source files and paths
OPT_OS = chibios
CHIBIOS = $(TOP_DIR)/lib/chibios
CHIBIOS_CONTRIB = $(TOP_DIR)/lib/chibios-contrib

#
# Startup, Port and Platform support selection
##############################################################################

ifeq ($(strip $(MCU)), risc-v)
    # RISC-V Support
    # As of 7.4.2021 there is only one supported RISC-V platform in Chibios-Contrib,
    # therefore all required settings are hard-coded
    STARTUP_MK = $(CHIBIOS_CONTRIB)/os/common/startup/RISCV-ECLIC/compilers/GCC/mk/startup_$(MCU_STARTUP).mk
    PORT_V = $(CHIBIOS_CONTRIB)/os/common/ports/RISCV-ECLIC/compilers/GCC/mk/port.mk
    RULESPATH = $(CHIBIOS_CONTRIB)/os/common/startup/RISCV-ECLIC/compilers/GCC
else
    # ARM Support
    # Startup files. Try a few different locations, for compability with old versions and
    # for things hardware in the contrib repository
    STARTUP_MK = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/startup_$(MCU_STARTUP).mk
    ifeq ("$(wildcard $(STARTUP_MK))","")
        STARTUP_MK = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_$(MCU_STARTUP).mk
        ifeq ("$(wildcard $(STARTUP_MK))","")
            STARTUP_MK = $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_$(MCU_STARTUP).mk
        endif
    endif

    # Compability with old version
    PORT_V = $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/mk/port_v$(ARMV)m.mk
    ifeq ("$(wildcard $(PORT_V))","")
    PORT_V = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v$(ARMV)m.mk
    endif

    RULESPATH = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC
    ifeq ("$(wildcard $(RULESPATH)/rules.mk)","")
    RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
    endif
endif

ifeq ("$(PLATFORM_NAME)","")
    PLATFORM_NAME = platform
endif

PLATFORM_MK = $(CHIBIOS)/os/hal/ports/$(MCU_FAMILY)/$(MCU_SERIES)/$(PLATFORM_NAME).mk
ifeq ("$(wildcard $(PLATFORM_MK))","")
PLATFORM_MK = $(CHIBIOS_CONTRIB)/os/hal/ports/$(MCU_FAMILY)/$(MCU_SERIES)/$(PLATFORM_NAME).mk
endif

include $(STARTUP_MK)
include $(PORT_V)
include $(PLATFORM_MK)

#
# Board support selection.
##############################################################################

BOARD_MK :=

ifneq ("$(wildcard $(KEYBOARD_PATH_5)/boards/$(BOARD)/board.mk)","")
    BOARD_PATH = $(KEYBOARD_PATH_5)
    BOARD_MK += $(KEYBOARD_PATH_5)/boards/$(BOARD)/board.mk
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/boards/$(BOARD)/board.mk)","")
    BOARD_PATH = $(KEYBOARD_PATH_4)
    BOARD_MK += $(KEYBOARD_PATH_4)/boards/$(BOARD)/board.mk
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/boards/$(BOARD)/board.mk)","")
    BOARD_PATH = $(KEYBOARD_PATH_3)
    BOARD_MK += $(KEYBOARD_PATH_3)/boards/$(BOARD)/board.mk
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/boards/$(BOARD)/board.mk)","")
    BOARD_PATH = $(KEYBOARD_PATH_2)
    BOARD_MK += $(KEYBOARD_PATH_2)/boards/$(BOARD)/board.mk
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/boards/$(BOARD)/board.mk)","")
    BOARD_PATH = $(KEYBOARD_PATH_1)
    BOARD_MK += $(KEYBOARD_PATH_1)/boards/$(BOARD)/board.mk
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/board/board.mk)","")
    BOARD_PATH = $(TOP_DIR)/platforms/chibios/boards/$(BOARD)
    BOARD_MK += $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/board/board.mk
    KEYBOARD_PATHS += $(BOARD_PATH)/configs
    ifneq ("$(wildcard $(BOARD_PATH)/rules.mk)","")
        include $(BOARD_PATH)/rules.mk
    endif
endif

ifeq ("$(wildcard $(BOARD_MK))","")
    BOARD_MK = $(CHIBIOS)/os/hal/boards/$(BOARD)/board.mk
    ifeq ("$(wildcard $(BOARD_MK))","")
        BOARD_MK = $(CHIBIOS_CONTRIB)/os/hal/boards/$(BOARD)/board.mk
    endif
endif

include $(BOARD_MK)

#
# Bootloader selection.
##############################################################################

# Set bootloader address if supplied.
ifdef STM32_BOOTLOADER_ADDRESS
    OPT_DEFS += -DSTM32_BOOTLOADER_ADDRESS=$(STM32_BOOTLOADER_ADDRESS)
endif

# Work out if we need to set up the include for the bootloader definitions
ifneq ("$(wildcard $(KEYBOARD_PATH_5)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_5)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_5)/boards/$(BOARD)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_5)/boards/$(BOARD)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_4)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/boards/$(BOARD)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_4)/boards/$(BOARD)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_3)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/boards/$(BOARD)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_3)/boards/$(BOARD)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_2)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/boards/$(BOARD)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_2)/boards/$(BOARD)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_1)/bootloader_defs.h
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/boards/$(BOARD)/bootloader_defs.h)","")
    OPT_DEFS += -include $(KEYBOARD_PATH_1)/boards/$(BOARD)/bootloader_defs.h
else ifneq ("$(wildcard $(BOARD_PATH)/configs/bootloader_defs.h)","")
    OPT_DEFS += -include $(BOARD_PATH)/configs/bootloader_defs.h
endif

#
# ChibiOS config selection.
##############################################################################

# Work out the config file directories
ifneq ("$(wildcard $(KEYBOARD_PATH_5)/chconf.h)","")
    CHCONFDIR = $(KEYBOARD_PATH_5)
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/chconf.h)","")
    CHCONFDIR = $(KEYBOARD_PATH_4)
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/chconf.h)","")
    CHCONFDIR = $(KEYBOARD_PATH_3)
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/chconf.h)","")
    CHCONFDIR = $(KEYBOARD_PATH_2)
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/chconf.h)","")
    CHCONFDIR = $(KEYBOARD_PATH_1)
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs/chconf.h)","")
    CHCONFDIR = $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs
else ifneq ("$(wildcard $(TOP_DIR)/platforms/boards/chibios/common/configs/chconf.h)","")
    CHCONFDIR = $(TOP_DIR)/platforms/chibios/boards/common/configs
endif

#
# HAL config selection.
##############################################################################

ifneq ("$(wildcard $(KEYBOARD_PATH_5)/halconf.h)","")
    HALCONFDIR = $(KEYBOARD_PATH_5)
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/halconf.h)","")
    HALCONFDIR = $(KEYBOARD_PATH_4)
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/halconf.h)","")
    HALCONFDIR = $(KEYBOARD_PATH_3)
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/halconf.h)","")
    HALCONFDIR = $(KEYBOARD_PATH_2)
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/halconf.h)","")
    HALCONFDIR = $(KEYBOARD_PATH_1)
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs/halconf.h)","")
    HALCONFDIR = $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/common/configs/halconf.h)","")
    HALCONFDIR = $(TOP_DIR)/platforms/chibios/boards/common/configs
endif

#
# Linker script selection.
##############################################################################

ifneq ("$(wildcard $(KEYBOARD_PATH_5)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_5)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_4)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_3)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_2)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(KEYBOARD_PATH_1)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/ld/$(MCU_LDSCRIPT).ld)","")
    LDFLAGS += -L$(TOP_DIR)/platforms/chibios/boards/$(BOARD)/ld
    LDSCRIPT = $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/common/ld/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(TOP_DIR)/platforms/chibios/boards/common/ld/$(MCU_LDSCRIPT).ld
else ifneq ("$(wildcard $(STARTUPLD_CONTRIB)/$(MCU_LDSCRIPT).ld)","")
    LDSCRIPT = $(STARTUPLD_CONTRIB)/$(MCU_LDSCRIPT).ld
    USE_CHIBIOS_CONTRIB = yes
else
    LDSCRIPT = $(STARTUPLD)/$(MCU_LDSCRIPT).ld
endif

#
# Include ChibiOS makefiles.
##############################################################################

# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
-include $(CHIBIOS)/os/hal/osal/rt/osal.mk         # ChibiOS <= 19.x
-include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk     # ChibiOS >= 20.x
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
# Other files (optional).
include $(CHIBIOS)/os/hal/lib/streams/streams.mk

CHIBISRC = $(STARTUPSRC) \
        $(KERNSRC) \
        $(PORTSRC) \
        $(OSALSRC) \
        $(HALSRC) \
        $(PLATFORMSRC) \
        $(BOARDSRC) \
        $(STREAMSSRC) \
        $(CHIBIOS)/os/various/syscalls.c \
        $(PLATFORM_COMMON_DIR)/syscall-fallbacks.c \
        $(PLATFORM_COMMON_DIR)/wait.c

# Ensure the ASM files are not subjected to LTO -- it'll strip out interrupt handlers otherwise.
QUANTUM_LIB_SRC += $(STARTUPASM) $(PORTASM) $(OSALASM) $(PLATFORMASM)

CHIBISRC := $(patsubst $(TOP_DIR)/%,%,$(CHIBISRC))

EXTRAINCDIRS += $(CHIBIOS)/os/license $(CHIBIOS)/os/oslib/include \
         $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs \
         $(TOP_DIR)/platforms/chibios/boards/common/configs \
         $(HALCONFDIR) $(CHCONFDIR) \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) $(TESTINC) \
         $(STREAMSINC) $(CHIBIOS)/os/various $(COMMON_VPATH)

#
# ChibiOS-Contrib
##############################################################################

# Work out if we're using ChibiOS-Contrib by checking if halconf_community.h exists
ifneq ("$(wildcard $(KEYBOARD_PATH_5)/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
else ifneq ("$(wildcard $(KEYBOARD_PATH_4)/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
else ifneq ("$(wildcard $(KEYBOARD_PATH_3)/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
else ifneq ("$(wildcard $(KEYBOARD_PATH_2)/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
else ifneq ("$(wildcard $(KEYBOARD_PATH_1)/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
else ifneq ("$(wildcard $(TOP_DIR)/platforms/chibios/boards/$(BOARD)/configs/halconf_community.h)","")
    USE_CHIBIOS_CONTRIB = yes
endif

ifeq ($(strip $(USE_CHIBIOS_CONTRIB)),yes)
    include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
    CHIBISRC += $(PLATFORMSRC_CONTRIB) $(HALSRC_CONTRIB)
    EXTRAINCDIRS += $(PLATFORMINC_CONTRIB) $(HALINC_CONTRIB) $(CHIBIOS_CONTRIB)/os/various
endif

#
# Project, sources and paths
##############################################################################

##############################################################################
# Injected configs
#
ifneq ("$(wildcard $(BOARD_PATH)/configs/config.h)","")
    CONFIG_H += $(BOARD_PATH)/configs/config.h
endif
ifneq ("$(wildcard $(BOARD_PATH)/configs/post_config.h)","")
    POST_CONFIG_H += $(BOARD_PATH)/configs/post_config.h
endif

##############################################################################
# Compiler and Linker configuration
#

# Use defined stack sizes of the main thread in linker scripts
LDSYMBOLS =,--defsym=__process_stack_size__=$(USE_PROCESS_STACKSIZE),--defsym=__main_stack_size__=$(USE_EXCEPTIONS_STACKSIZE)

# Shared Compiler flags for all toolchains
SHARED_CFLAGS = -fomit-frame-pointer \
                -ffunction-sections \
                -fdata-sections \
                -fno-common \
                -fshort-wchar

# Shared Linker flags for all toolchains
SHARED_LDFLAGS = -Wl,--script=$(LDSCRIPT)$(LDSYMBOLS) \
                 -Wl,--gc-sections \
                 -nostartfiles

ifeq ($(strip $(MCU)), risc-v)
    # RISC-V toolchain specific configuration
    # Find suitable GCC compiler
    ifeq ($(strip $(TOOLCHAIN)),)
        ifneq ($(shell which riscv32-unknown-elf-gcc 2>/dev/null),)
            TOOLCHAIN = riscv32-unknown-elf-
        else
            ifneq ($(shell which riscv64-unknown-elf-gcc 2>/dev/null),)
                TOOLCHAIN = riscv64-unknown-elf-
            else
                $(error "No RISC-V toolchain found. Can't find riscv32-unknown-elf-gcc or riscv64-unknown-elf-gcc found in your systems PATH variable. Please install a valid toolchain and make it accessible!")
            endif
        endif
    endif

    # MCU architecture flags
    MCUFLAGS = -march=$(MCU_ARCH) \
               -mabi=$(MCU_ABI) \
               -mcmodel=$(MCU_CMODEL) \
               -mstrict-align
else
    # ARM toolchain specific configuration
    TOOLCHAIN ?= arm-none-eabi-

    # Toolchain specific Linker flags
    TOOLCHAIN_LDFLAGS = -Wl,--no-wchar-size-warning \
                        --specs=nano.specs

    # MCU architecture flags
    MCUFLAGS = -mcpu=$(MCU) \
               -mthumb -DTHUMB_PRESENT \
               -mno-thumb-interwork -DTHUMB_NO_INTERWORKING \
               -mno-unaligned-access

    # Some ARM cores like the M4 and M7 have floating point units which can be enabled
    USE_FPU ?= no

    ifneq ($(USE_FPU),no)
        OPT_DEFS += -DCORTEX_USE_FPU=TRUE

        # Default is single precision floats
        USE_FPU_OPT ?= -mfloat-abi=hard \
                       -mfpu=fpv4-sp-d16 \
                       -fsingle-precision-constant

        MCUFLAGS += $(USE_FPU_OPT)
    else
        OPT_DEFS += -DCORTEX_USE_FPU=FALSE
    endif
endif

# Assembler flags
ASFLAGS  += $(SHARED_ASFLAGS) $(TOOLCHAIN_ASFLAGS)

# C Compiler flags
CFLAGS   += $(SHARED_CFLAGS) $(TOOLCHAIN_CFLAGS)

# C++ Compiler flags
CXXFLAGS += $(CFLAGS) $(SHARED_CXXFLAGS) $(TOOLCHAIN_CXXFLAGS) -fno-rtti

# Linker flags
LDFLAGS  += $(SHARED_LDFLAGS) $(TOOLCHAIN_LDFLAGS) $(MCUFLAGS)

# Tell QMK that we are hosting it on ChibiOS.
OPT_DEFS += -DPROTOCOL_CHIBIOS

# Speed up recompilations by opt-in usage of ccache
USE_CCACHE ?= no
ifneq ($(USE_CCACHE),no)
    CC_PREFIX ?= ccache
endif

# Construct GCC toolchain
CC      = $(CC_PREFIX) $(TOOLCHAIN)gcc
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE    = $(TOOLCHAIN)size
AR      = $(TOOLCHAIN)ar
NM      = $(TOOLCHAIN)nm
HEX     = $(OBJCOPY) -O $(FORMAT)
EEP     =
BIN     = $(OBJCOPY) -O binary

##############################################################################
# Make targets
#

DEBUG = gdb

# List any extra directories to look for libraries here.
EXTRALIBDIRS = $(RULESPATH)/ld

bin: $(BUILD_DIR)/$(TARGET).bin sizeafter
	$(COPY) $(BUILD_DIR)/$(TARGET).bin $(TARGET).bin;
