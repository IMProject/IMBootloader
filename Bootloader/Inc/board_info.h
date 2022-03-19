/****************************************************************************
 *
 *   Copyright (c) 2021 IMProject Development Team. All rights reserved.
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

#ifndef BOOTLOADER_INC_BOARD_INFO_H_
#define BOOTLOADER_INC_BOARD_INFO_H_

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <main.h>

/* 32 bytes fake board id. If enabled, board and manufacturer id communication will be skipped. */
//#define FAKE_BOARD_ID  "NOT_SECURED_MAGIC_STRING_1234567"

typedef enum secureHash_ENUM {
    BLAKE2B = 0,
    SHA256,
    MD5
} secureHash_E;

#define HASH_BOARD_ID_ALGORITHM         BLAKE2B

#define HASHED_BOARD_ID_SIZE            32U
#define BASE64_HASHED_BOARD_ID_SIZE     44U

#define MANUFACTURER_ID_SIZE            32U
#define BASE64_MANUFACTURER_ID_SIZE     44U

#define PRODUCT_TYPE_MAX_SIZE           100U

#define BASE64_MANUFACTURER_ID (const char*)("Qj9FKEgrTWJRZVRoV21acTR0N3cheiVDKkYpSkBOY1I=") // Size must be equal to 44 bytes (Base64 format)

#ifdef MATEK_H743_SLIM
#define PRODUCT_TYPE    (const char*)("IMProject_demo-matek_H7_slim")   // Maximum allowed size is 100 bytes
#elif defined(PIXHAWK4)
#define PRODUCT_TYPE    (const char*)("IMProject_demo-pixhawk4")        // Maximum allowed size is 100 bytes
#else
#define PRODUCT_TYPE    (const char*)("Product_type_name-board_name")   // Maximum allowed size is 100 bytes
#endif

static_assert(strlen((const char*)BASE64_MANUFACTURER_ID) == BASE64_MANUFACTURER_ID_SIZE, "MANUFACTURER_ID is wrong size");
static_assert(strlen((const char*)PRODUCT_TYPE) <= PRODUCT_TYPE_MAX_SIZE, "PRODUCT_TYPE is wrong size");

bool BoardInfo_getDataJson(uint8_t* buffer, size_t size);
bool BoardInfo_getBase64ManufacturerId(uint8_t* b64_manufacturer_id);


#endif /* BOOTLOADER_INC_BOARD_INFO_H_ */
