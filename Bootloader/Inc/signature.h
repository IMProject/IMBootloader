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

#ifndef BOOTLOADER_INC_SIGNATURE_H_
#define BOOTLOADER_INC_SIGNATURE_H_

#include <stdint.h>

#define SIGNATURE_SIZE 64 //!< Signature size is 64 bytes

typedef struct signature {
    uint64_t magic_key;    //!< First 8 bytes for the magic key
    uint64_t unused[7];    //!< The rest of the 56 bytes are reserved for future use
} signature_S;

//! Enumeration for different signatures
typedef enum detectedBinary_ENUM {
    detectedBinary_FIRMWARE_FLASH,   //!< New firmware for FLASH
    detectedBinary_FIRMWARE_RAM,     //!< Firmware for RAM
    detectedBinary_BOOTLOADER_FLASH, //!< New bootloader for FLASH
    detectedBinary_BOOTLOADER_RAM,   //!< Bootloader for RAM
} detectedBinary_E;

detectedBinary_E Signature_verification(const signature_S* signature);

#endif /* BOOTLOADER_INC_SIGNATURE_H_ */
