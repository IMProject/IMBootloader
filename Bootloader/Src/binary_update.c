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

#include "utils.h"
#include "binary_update.h"
#include "flash_adapter.h"
#include "security.h"
#include "signature.h"

__attribute__ ((section(".restart_info")))
volatile bootInfo_S boot_info;              //!< Instruction on where to jump after the restart
static uint64_t s_address;                  //!< Address from where to erase flash and write binary
static signatureType_E s_detected_binary;   //!< Detected binary

static bool BinaryUpdate_writeToFlash(uint8_t* write_buffer, const uint32_t data_length);

bool
BinaryUpdate_handleDetectedBinary(signatureType_E detected_binary) {

    bool success = true;
    s_detected_binary = detected_binary;

    switch (detected_binary) {

        case signatureType_FIRMWARE_FLASH:
            s_address = FLASH_FIRMWARE_ADDRESS;
            break;

        case signatureType_BOOTLOADER_FLASH:
            s_address = FLASH_BOOTLOADER_ADDRESS;
            break;

        case signatureType_FIRMWARE_RAM:
        case signatureType_BOOTLOADER_RAM:
            s_address = RAM_FIRMWARE_ADDRESS;
            break;

        case signatureType_UNKNOWN:
            //we support unsigned binary but handle it as firmware for flash
            success = false;
            s_address = FLASH_FIRMWARE_ADDRESS;
            break;

        default:
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
            break;
    }
}

uint32_t
BinaryUpdate_getJumpAddress(void) {
    return boot_info.jump_address;
}

void
BinaryUpdate_resetJumpAddress(void) {
    boot_info.jump_address = signatureType_FIRMWARE_FLASH;;
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

        case signatureType_FIRMWARE_FLASH:
            success = FlashAdapter_erase(firmware_size, s_address);
            break;
        case signatureType_BOOTLOADER_FLASH:
            if (boot_info.jump_address == RAM_FIRMWARE_ADDRESS) {
                //Only allowed to erase if RAM version is running
                success = FlashAdapter_erase(firmware_size, s_address);
            }
            break;

        case signatureType_FIRMWARE_RAM:
        case signatureType_BOOTLOADER_RAM:
            break;

        case signatureType_UNKNOWN:
            success = FlashAdapter_erase(firmware_size, s_address);
            break;

        default:
            break;
    }

    return success;
}

bool
BinaryUpdate_write(uint8_t* write_buffer, const uint32_t packet_length) {

    bool success = false;

    bool is_secured = Security_isSecured();

    if (is_secured) {

        uint32_t data_length = packet_length - (MAC_SIZE + NONCE_SIZE);

        if (data_length > 0U) {

            uint8_t mac[MAC_SIZE];
            uint8_t nonce[NONCE_SIZE];
            uint8_t encrypted_data[DATA_SIZE];
            uint8_t decrypted_data[DATA_SIZE];

            Utils_DeserializeBlobLE(&(write_buffer[0]), mac, MAC_SIZE);
            Utils_DeserializeBlobLE(&(write_buffer[MAC_SIZE]), nonce, NONCE_SIZE);
            Utils_DeserializeBlobLE(&(write_buffer[MAC_SIZE + NONCE_SIZE]), encrypted_data, data_length);

            success = Security_decrypt(mac, nonce, encrypted_data, decrypted_data, data_length);

            if (success) {
                success = BinaryUpdate_writeToFlash(decrypted_data, data_length);
            }
        }

    } else {
        success = BinaryUpdate_writeToFlash(write_buffer, packet_length);
    }

    return success;
}

bool
BinaryUpdate_finish(void) {

    bool success = true;

    bool is_secured = Security_isSecured();

    if (is_secured) {
        Security_wipeKeys();
    }

    switch (s_detected_binary) {

        case signatureType_FIRMWARE_FLASH:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            success = FlashAdapter_finish();
            break;

        case signatureType_FIRMWARE_RAM:
            boot_info.jump_address = RAM_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            break;

        case signatureType_BOOTLOADER_FLASH:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            break;

        case signatureType_BOOTLOADER_RAM:
            boot_info.jump_address = RAM_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = true;
            break;

        case signatureType_UNKNOWN:
            boot_info.jump_address = FLASH_FIRMWARE_ADDRESS;
            boot_info.skip_bl_loop = false;
            success = FlashAdapter_finish();
            break;

        default:
            break;
    }

    boot_info.end = 0xFF;

    return success;
}

bool
BinaryUpdate_writeToFlash(uint8_t* write_buffer, const uint32_t data_length) {

    bool success = false;

    success = FlashAdapter_program((uint32_t)s_address, write_buffer, data_length);

    if (success) {
        // cppcheck-suppress misra-c2012-18.8;
        uint8_t readout_buffer[data_length];
        success = FlashAdapter_readBytes((uint32_t)s_address, readout_buffer, data_length);

        if (success) {
            for (uint32_t i = 0U; (success) && (i < data_length); ++i) {
                if (write_buffer[i] != readout_buffer[i]) {
                    success = false;
                }
            }
        }
    }

    s_address += data_length;

    return success;
}
