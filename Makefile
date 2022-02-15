######################################
# target
######################################
export TARGET = IMBootloader

######################################
# building variables
######################################
# debug build?
export DEBUG = 1
# optimization
export OPT = -Og

######################################
# Git branch and hash
######################################
export BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
export HASH := $(shell git rev-parse HEAD)
export TAG := $(shell git tag --sort=committerdate | tail -1)

#######################################
# paths
#######################################

export COMMON_SRCS =  \
Bootloader/Src/main.c \
Bootloader/Src/crc32.c \
Bootloader/Src/firmware_update.c \
Bootloader/Src/board_info.c \
Bootloader/Adapters/Src/gpio_adapter.c \
Bootloader/Adapters/Src/flash_adapter.c \
Bootloader/Adapters/Src/hash_adapter.c \
Bootloader/Adapters/Src/system_clock_adapter.c \
Bootloader/STM32/Src/usb_device.c \
Bootloader/STM32/Src/usbd_conf.c \
Bootloader/STM32/Src/usbd_desc.c \
Bootloader/STM32/Src/usbd_cdc_if.c \
Bootloader/Utility/Src/version.c \
Bootloader/Utility/Src/utils.c \
Bootloader/Utility/Src/base64.c \
Bootloader/Utility/Src/json.c \
Monocypher/src/monocypher.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

export COMMON_INCS = \
-IBootloader/Inc \
-IBootloader/Adapters/Inc \
-IBootloader/Utility/Inc \
-IBootloader/STM32/Inc \
-IMonocypher/src \
-IDrivers/CMSIS/Include \
-IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \
-IExternalDevices/Winbond/Inc/

#######################################
# binaries
#######################################
export PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
export CC = $(GCC_PATH)/$(PREFIX)gcc
export AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
export CP = $(GCC_PATH)/$(PREFIX)objcopy
export SZ = $(GCC_PATH)/$(PREFIX)size
else
export CC = $(PREFIX)gcc
export AS = $(PREFIX)gcc -x assembler-with-cpp
export CP = $(PREFIX)objcopy
export SZ = $(PREFIX)size
endif
export HEX = $(CP) -O ihex
export BIN = $(CP) -O binary -S

#######################################
# Astyle
#######################################
.PHONY: check_format format

check_format:
	$(call colorecho,'Checking formatting with astyle')
	@Tools/astyle/check_code_style_all.sh
	@git diff --check

format:
	$(call colorecho,'Formatting with astyle')
	@Tools/astyle/check_code_style_all.sh --fix

#
# Bootloaders to build
#
TARGETS	= \
	stm32l4xx \
	stm32h7xx \
	stm32f7xx

all:	$(TARGETS)

clean:
	rm -f *.elf *.bin *.map # Remove any elf or bin files contained directly in the Bootloader directory
	rm -rf build # Remove build directories

#
# Board specific targets.
#
BOARD =

matek_H7_slim:
	${MAKE} stm32h7xx BOARD=MATEK_H743_SLIM

pixhawk4:
	${MAKE} stm32f7xx BOARD=PIXHAWK4

#
# Microcontroller specific targets.
#

stm32l4xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32l4xx LDSCRIPT=STM32L4xx.ld FLASH=INTERNAL_FLASH TARGET_FILE_NAME=$@

stm32h7xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=STM32H7xx.ld FLASH=INTERNAL_FLASH TARGET_FILE_NAME=$@

stm32h7xx_ext: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=STM32H7xx.ld FLASH=EXTERNAL_FLASH TARGET_FILE_NAME=$@

stm32f7xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32f7xx LDSCRIPT=STM32F7xx.ld FLASH=INTERNAL_FLASH TARGET_FILE_NAME=$@
