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

#ifndef BOOTLOADER_INC_BINARYUPDATE_H_
#define BOOTLOADER_INC_BINARYUPDATE_H_

#include <stdint.h>
#include <stdbool.h>
#include "signature.h"

#pragma pack(push, 1)
typedef struct bootInfo {
    uint32_t jump_address;                  //!< Address for BL to jump
    bool skip_bl_loop;                      //!< Flag to skip BL loop
    detectedBinary_E previus_binary;        //!< Previous detected binary
} bootInfo_S;
#pragma pack(pop)

bool BinaryUpdate_handleDetectedBinary(detectedBinary_E detected_binary);
void BinaryUpdate_handleBootInfo(void);
uint32_t BinaryUpdate_getJumpAddress(void);
void BinaryUpdate_resetJumpAddress(void);
bool BinaryUpdate_checkSkipLoopFlag(void);
void BinaryUpdate_disableLoopFlag(void);
bool BinaryUpdate_erase(uint32_t firmware_size);
bool BinaryUpdate_write(uint8_t* write_buffer, const uint32_t data_length, uint32_t* crc);
bool BinaryUpdate_finish(void);

#endif /* BOOTLOADER_INC_BINARYUPDATE_H_ */
