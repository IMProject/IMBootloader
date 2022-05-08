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
#include "firmware_update.h"
#include "flash_adapter.h"

bool
FirmwareUpdate_flash(uint8_t* write_buffer, const uint32_t flash_length, uint32_t* crc) {
    bool success = false;
    // cppcheck-suppress misra-c2012-18.8; PACKET_SIZE is defined as 256 or W25Q_PAGE_SIZE, depends on EXTERNAL_FLASH
    uint8_t readout_buffer[PACKET_SIZE];
    static uint32_t index = 0U;
    const uint32_t address_addition = index * PACKET_SIZE;
    uint64_t address = FLASH_FIRMWARE_ADDRESS + address_addition;

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
FirmwareUpdate_finish(void) {
    return FlashAdapter_finish();
}
