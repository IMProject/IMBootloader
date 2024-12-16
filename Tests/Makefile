# Detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

######################################
# Git branch and hash
######################################
export BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
export HASH := $(shell git rev-parse HEAD)
export TAG := $(shell git tag --sort=committerdate | tail -1)

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

UNITY_ROOT=Unity

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wno-array-parameter
CFLAGS += -ftest-coverage
CFLAGS += -fprofile-arcs

C_DEFS += \
-DGIT_BRANCH=\"$(BRANCH)\" \
-DGIT_HASH=\"$(HASH)\" \
-DGIT_TAG=\"$(TAG)\"

TARGET_DIR=build
TARGET_BASE=all_tests
TARGET = $(TARGET_DIR)/$(TARGET_BASE)$(TARGET_EXTENSION)

IMBOOTLOADER_INC=\
  -I../Bootloader/Adapters/Inc \
  -I../Bootloader/Inc \
  -I../Bootloader/Library/Inc \
  -I../Bootloader/Utility/Inc \

CMOCK_INC=\
  ../Bootloader/Adapters/Inc/* \
  ../Bootloader/Inc/* \
  ../Bootloader/Library/Inc/* \
  ../Bootloader/Utility/Inc/utils.h \
  ../Bootloader/Utility/Inc/json.h

SRC_FILES+= \
  $(UNITY_ROOT)/src/unity.c \
  $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
  CMock/src/cmock.c \
  test_main.c \
  
SRC_BINARY_UPDATE= \
  ../Bootloader/Src/binary_update.c \
  test_binary_update.c \
  mocks/mock_flash_adapter.c \
  mocks/mock_security.c \
  mocks/mock_utils.c
  
SRC_BOARD_INFO= \
  ../Bootloader/Src/board_info.c \
  test_board_info.c \
  mocks/mock_hash_adapter.c \
  mocks/mock_json.c

INC_DIRS=$(IMBOOTLOADER_INC) -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixture/src -I CMock/src -I mocks
SYMBOLS = -DUNIT_TEST
SYMBOLS += -DUNITY_FIXTURE_NO_EXTRAS
SYMBOLS += -DUNITY_INCLUDE_DOUBLE
SYMBOLS += -DUNITY_DOUBLE_PRECISION=1e-12

#######################################
# Unit test
#######################################

all: test_clean test_cmock test_all test_binary_update

test_cmock:
	ruby CMock/lib/cmock.rb -oCMockConfig.yml $(CMOCK_INC)

test_binary_update:
	mkdir -p $(TARGET_DIR)
	$(C_COMPILER) $(CFLAGS) $(C_DEFS) $(INC_DIRS) $(SYMBOLS) -DBINARY_UPDATE -g $(SRC_FILES) $(SRC_BINARY_UPDATE) -o $(TARGET) -lm
	- ./$(TARGET) -v 
	
test_board_info:
	mkdir -p $(TARGET_DIR)
	$(C_COMPILER) $(CFLAGS) $(C_DEFS) $(INC_DIRS) $(SYMBOLS) -DBOARD_INFO -g $(SRC_FILES) $(SRC_BOARD_INFO) -o $(TARGET) -lm
	- ./$(TARGET) -v 

test_all: \
	test_binary_update \
	test_board_info

test_clean:
	$(CLEANUP) $(TARGET) $(GCOV_EXTENSIONS) \
	rmdir -rf $(TARGET_DIR) $(GCOVR_FOLDER) $(LCOV_FOLDER)

ci: CFLAGS += -Werror
ci: default
