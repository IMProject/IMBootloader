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

#include "hash_adapter.h"
#include "monocypher.h"
#include "base64.h"
#include "main.h"


void
HashAdapter_getHashedBoardId(uint8_t hashed_board_id[HASHED_BOARD_ID_SIZE]) {

    uint32_t uid[3];
    uid[0] = HAL_GetUIDw0();
    uid[1] = HAL_GetUIDw1();
    uid[2] = HAL_GetUIDw2();

    uint8_t manufacturer_id[MANUFACTURER_ID_SIZE];

    if (BoardInfo_decodeBase64ManufacturerId(manufacturer_id)) {

        switch (HASH_BOARD_ID_ALGORITHM) {

            case (BLAKE2B):
                crypto_blake2b_general(hashed_board_id, HASHED_BOARD_ID_SIZE, manufacturer_id, MANUFACTURER_ID_SIZE, (uint8_t*)uid, 3U * sizeof(uint32_t));
                break;

            case (SHA256):
                //Only SMT32H75x supports HW accelerator
                break;
            default:
                break;
        }
    }
}

bool
HashAdapter_getBase64HashedBoardId(uint8_t* b64_hashed_board_id_str, size_t b64_hashed_board_id_str_size) {

    bool success = false;

    uint8_t hashed_board_id[HASHED_BOARD_ID_SIZE];
    HashAdapter_getHashedBoardId(hashed_board_id);
    int32_t ret = Base64_encode(hashed_board_id, HASHED_BOARD_ID_SIZE, (char*)b64_hashed_board_id_str, b64_hashed_board_id_str_size);

    if (0 == ret) {
        success = true;
    }

    return success;
}

bool
BoardInfo_decodeBase64ManufacturerId(uint8_t* manufacturer_id) {
    bool success = false;

    // cppcheck-suppress misra-c2012-11.8
    int32_t ret = Base64_decode((char*)MANUFACTURER_ID_BASE64_STR, MANUFACTURER_ID_SIZE_BASE64_STR, manufacturer_id, MANUFACTURER_ID_SIZE);

    if (0 == ret) {
        success = true;
    }

    return success;
}
