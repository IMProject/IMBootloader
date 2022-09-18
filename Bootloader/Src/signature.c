/****************************************************************************
 *
 *   Copyright (c) 2022 IMProject Development Team. All rights reserved.
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
#include <string.h>
#include "signature.h"

static const uint64_t signature_magic_key = 0xDEC0DE5528101987;    //!< First 8 signature bytes of firmware for flash

__attribute__ ((section(".bl_flash_signature"))) signature_S bl_flash_signature = {
    .magic_key = signature_magic_key,
    .type = signatureType_BOOTLOADER_FLASH
};

__attribute__ ((section(".bl_ram_signature"))) signature_S bl_ram_signature = {
    .magic_key = signature_magic_key,
    .type = signatureType_BOOTLOADER_RAM
};

signatureType_E
Signature_verification(const signature_S* signature) {

    signatureType_E detected_binary;

    if (0 == memcmp(&(signature->magic_key), &signature_magic_key, sizeof(signature_magic_key))) {

        switch (signature->type) {
            case signatureType_FIRMWARE_FLASH:
                detected_binary = signatureType_FIRMWARE_FLASH;
                break;
            case signatureType_FIRMWARE_RAM:
                detected_binary = signatureType_FIRMWARE_RAM;
                break;
            case signatureType_BOOTLOADER_FLASH:
                detected_binary = signatureType_BOOTLOADER_FLASH;
                break;
            case signatureType_BOOTLOADER_RAM:
                detected_binary = signatureType_BOOTLOADER_RAM;
                break;
            default:
                detected_binary = signatureType_UNKNOWN;
                break;
        }

    } else {
        detected_binary = signatureType_UNKNOWN;
    }

    return detected_binary;
}

