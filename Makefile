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
Bootloader/Src/binary_update.c \
Bootloader/Src/communication.c \
Bootloader/Src/security.c \
Bootloader/Src/signature.c \
Bootloader/Src/board_info.c \
Bootloader/Adapters/Src/gpio_adapter.c \
Bootloader/Adapters/Src/flash_adapter.c \
Bootloader/Adapters/Src/hash_adapter.c \
Bootloader/Adapters/Src/security_adapter.c \
Bootloader/Adapters/Src/system_adapter.c \
Bootloader/Adapters/Src/system_clock_adapter.c \
Bootloader/Library/Src/software_info.c \
Bootloader/STM32/Src/syscalls.c \
Bootloader/STM32/Src/sysmem.c \
Bootloader/Utility/Src/base64.c \
Bootloader/Utility/Src/crc/crc32_base.c \
Bootloader/Utility/Src/crc/crc32_variants/crc32_bzip2.c \
Bootloader/Utility/Src/json.c \
Bootloader/Utility/Src/utils.c \
Monocypher/src/monocypher.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

export COMMON_INCS = \
-IBootloader/Inc \
-IBootloader/Adapters/Inc \
-IBootloader/Library/Inc \
-IBootloader/STM32/Inc \
-IBootloader/Utility/Inc \
-IBootloader/Utility/Inc/crc/ \
-IBootloader/Utility/Inc/crc/crc32_variants/ \
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

#######################################
# Cppcheck
#######################################
.PHONY: cppcheck misra
cppcheck:
	$(call colorecho,'Checking code with cppcheck')
	@cppcheck --error-exitcode=1 Bootloader -DSECURED

misra:
	$(call colorecho,'Checking MISRA C:2012 with cppcheck')
	@cppcheck cppcheck $(COMMON_INCS) Bootloader --force --addon=misra.py --inline-suppr --error-exitcode=1 \
	-i Bootloader/Utility -i Bootloader/STM32
#
# Bootloaders to build
#
TARGETS	= \
	stm32l4xx \
	stm32h7xx \
	stm32h7xx_ram \
	stm32f7xx \
	stm32f7xx_ram \
	stm32h735g_dk \
	stm32h735g_dk_ram \
	matek_H7_slim \
	matek_H7_slim_ram \
	nucleo_h755zi \
	nucleo_h755zi_ram \
	pixhawk4 \
	pixhawk4_ram

all:	$(TARGETS)

clean:
	rm -f *.elf *.bin *.map # Remove any elf or bin files contained directly in the Bootloader directory
	rm -rf build # Remove build directories

#
# Board specific targets.
#
export USB = FS
BOARD =

matek_H7_slim:
	${MAKE} stm32h7xx BOARD=MATEK_H743_SLIM BOARD_FILE_NAME=$@

matek_H7_slim_ram:
	${MAKE} stm32h7xx_ram BOARD=MATEK_H743_SLIM BOARD_FILE_NAME=$@
	
nucleo_h755zi:
	${MAKE} stm32h7xx BOARD=NUCLEO_H755ZI BOARD_FILE_NAME=$@

nucleo_h755zi_ram:
	${MAKE} stm32h7xx_ram BOARD=NUCLEO_H755ZI BOARD_FILE_NAME=$@

pixhawk4:
	${MAKE} stm32f7xx BOARD=PIXHAWK4 BOARD_FILE_NAME=$@

pixhawk4_ram:
	${MAKE} stm32f7xx_ram BOARD=PIXHAWK4 BOARD_FILE_NAME=$@
	
stm32h735g_dk:
	${MAKE} stm32h7xx BOARD=STM32H735G_DK BOARD_FILE_NAME=$@
	
stm32h735g_dk_ram:
	${MAKE} stm32h7xx_ram BOARD=STM32H735G_DK BOARD_FILE_NAME=$@
	
test_cmock test_all test_clean:
	${MAKE} -C Tests $@

#
# Microcontroller (MCU) specific targets.
#
stm32l4xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32l4xx LDSCRIPT=STM32L4xx.ld FLASH=INTERNAL_FLASH MCU_FILE_NAME=$@

stm32h7xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=STM32H7xx.ld FLASH=INTERNAL_FLASH MCU_FILE_NAME=$@

stm32h7xx_ram: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=STM32H7xx_RAM.ld FLASH=INTERNAL_FLASH MCU_FILE_NAME=$@

stm32h7xx_ext: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32h7xx LDSCRIPT=STM32H7xx.ld FLASH=EXTERNAL_FLASH MCU_FILE_NAME=$@

stm32f7xx: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32f7xx LDSCRIPT=STM32F7xx.ld FLASH=INTERNAL_FLASH MCU_FILE_NAME=$@

stm32f7xx_ram: $(MAKEFILE_LIST)
	${MAKE} -f Makefile.stm32f7xx LDSCRIPT=STM32F7xx_RAM.ld FLASH=INTERNAL_FLASH MCU_FILE_NAME=$@
	
#######################################
# Code coverage
#######################################
GCOV_EXTENSIONS=*.gc*
GCOVR_PATH=Tests/gcovr-report
LCOV_PATH=Tests/lcov-report

lcov-report: test_all
	mkdir -p $(LCOV_PATH)
	lcov --capture --directory . --output-file $(LCOV_PATH)/coverage.info
	lcov --remove $(LCOV_PATH)/coverage.info '*/Tests/*' --output-file $(LCOV_PATH)/coverage.info
	genhtml $(LCOV_PATH)/coverage.info --output-directory $(LCOV_PATH)

gcovr-report: test_all
	mkdir -p $(GCOVR_PATH)
	gcovr --exclude 'Tests' --root . --html --html-details --output $(GCOVR_PATH)/coverage.html

#######################################
# Dependancies
#######################################
deps:
	sudo apt-get install lcov
	pip3 install gcovr
	
