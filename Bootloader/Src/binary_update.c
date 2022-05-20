/****************************************************************************
 *
 *   Copyright (c) 2021 - 2022 IMProject Development Team. All rights reserved.
 *   Authors: Igor Misic <igy1000mb@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name IMProject nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "crc32.h"
#include "binary_update.h"
#include "flash_adapter.h"
#include "signature.h"

__attribute__ ((section(".restart_info")))
bootInfo_S boot_info;                       //!< Instruction on where to jump after the restart
static uint64_t s_address;                  //!< Address from where to erase flash and write binary
static detectedBinary_E s_detected_binary;  //!< Detected binary

bool
BinaryUpdate_handleDetectedBinary(detectedBinary_E detected_binary) {

    bool success = true;
    s_detected_binary = detected_binary;

    switch (detected_binary) {

        case detectedBinary_FIRMWARE_FLASH:
            s_address = FLASH_FIRMWARE_ADDRESS;
            break;

        case detectedBinary_BOOTLOADER_FLASH:
            s_address = FLASH_BOOTLOADER_ADDRESS;
            break;

        case detectedBinary_FIRMWARE_RAM:
        case detectedBinary_BOOTLOADER_RAM:
            s_address = RAM_FIRMWARE_ADDRESS;
            break;

        default:
            success = false;
            s_address = FLASH_FIRMWARE_ADDRESS;
            break;
    }

    return success;
}

void
BinaryUpdate_handleBootInfo(void) {

    switch (boot_info.jump_address) {

        case FLASH_FIRMWARE_ADDRESS:
        case FLASH_BOOTLOADER_ADDRESS:
        case RAM_FIRMWARE_ADDRESS:
            break;

        default:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            boot_info.previus_binary = detectedBinary_FIRMWARE_FLASH;
            break;
    }
}

uint32_t
BinaryUpdate_getJumpAddress(void) {
    return boot_info.jump_address;
}

void
BinaryUpdate_resetJumpAddress(void) {
    boot_info.jump_address = detectedBinary_FIRMWARE_FLASH;;
}

bool
BinaryUpdate_checkSkipLoopFlag(void) {
    return boot_info.skip_bl_loop;
}

void
BinaryUpdate_disableLoopFlag(void) {
    boot_info.skip_bl_loop = false;
}

bool
BinaryUpdate_erase(uint32_t firmware_size) {

    bool success = true;
    switch (s_detected_binary) {

        case detectedBinary_FIRMWARE_FLASH:
            success = FlashAdapter_erase(firmware_size, s_address);
            break;
        case detectedBinary_BOOTLOADER_FLASH:
            if (detectedBinary_BOOTLOADER_RAM != boot_info.previus_binary) {
                //Only allowed to erase if RAM version is running
                success = FlashAdapter_erase(firmware_size, s_address);
            }
            break;

        case detectedBinary_FIRMWARE_RAM:
        case detectedBinary_BOOTLOADER_RAM:
            break;

        default:
            success = FlashAdapter_erase(firmware_size, s_address);
            break;
    }

    return success;
}

bool
BinaryUpdate_write(uint8_t* write_buffer, const uint32_t flash_length, uint32_t* crc) {
    bool success = false;
    // cppcheck-suppress misra-c2012-18.8; PACKET_SIZE is defined as 256 or W25Q_PAGE_SIZE, depends on EXTERNAL_FLASH
    uint8_t readout_buffer[PACKET_SIZE];
    static uint32_t index = 0U;
    const uint32_t address_addition = index * PACKET_SIZE;
    uint64_t address = s_address + address_addition;

    success = FlashAdapter_program((uint32_t)address, write_buffer, flash_length);

    if (success) {
        success = FlashAdapter_readBytes((uint32_t)address, readout_buffer, flash_length);

        if (success) {
            *crc = CalculateCRC32(readout_buffer, flash_length, *crc, 0U, false, false, false);
            for (uint32_t i = 0U; (success) && (i < flash_length); ++i) {
                if (write_buffer[i] != readout_buffer[i]) {
                    success = false;
                }
            }
        }
    }

    ++index;

    return success;
}

bool
BinaryUpdate_finish(void) {

    bool success = true;

    switch (s_detected_binary) {

        case detectedBinary_FIRMWARE_FLASH:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            success = FlashAdapter_finish();
            break;

        case detectedBinary_FIRMWARE_RAM:
            boot_info.jump_address = RAM_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            break;

        case detectedBinary_BOOTLOADER_FLASH:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            break;

        case detectedBinary_BOOTLOADER_RAM:
            boot_info.jump_address = RAM_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = true;
            break;

        default:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            success = FlashAdapter_finish();
            break;
    }

    boot_info.previus_binary = s_detected_binary;
    return success;
}
