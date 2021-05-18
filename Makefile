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

#######################################
# paths
#######################################
# Build path
export BUILD_DIR = build

export COMMON_SRCS =  \
Bootloader/Src/main.c \
Bootloader/Src/crc32.c \
Bootloader/Src/firmware_update.c \
Bootloader/Src/firmware_update_adapter.c \
Bootloader/Utility/Src/usb_device.c \
Bootloader/Utility/Src/usbd_conf.c \
Bootloader/Utility/Src/usbd_desc.c \
Bootloader/Utility/Src/usbd_cdc_if.c \
Bootloader/Utility/Src/version.c \
Bootloader/Utility/Src/utils.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

export COMMON_INCS = \
-IBootloader/Inc \
-IBootloader/Utility/Inc \
-IBootloader/STM32/Inc \
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

#
# Bootloaders to build
#
TARGETS	= \
	stm32l4xx

all:	$(TARGETS)

clean:
	rm -f *.elf *.bin *.map # Remove any elf or bin files contained directly in the Bootloader directory
	rm -rf build # Remove build directories

#
# Specific bootloader targets.
#

stm32h7xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=Linker/STM32H7xx.ld FLASH=EXTERNAL_FLASH TARGET_FILE_NAME=$@
	
stm32l4xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32l4xx LDSCRIPT=Linker/STM32L4xx.ld FLASH=INTERNAL_FLASH TARGET_FILE_NAME=$@

