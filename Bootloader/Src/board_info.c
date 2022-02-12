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

#include "board_info.h"
#include "version.h"
#include "hash_adapter.h"
#include "json.h"
#include "base64.h"

/* JSON String
 *
 * {
 *   "bord_id":"Tk9UX1NFQ1VSRURfTUFHSUNfU1RSSU5HXzEyMzQ1Njc=",
 *   "manufacturer_id":"Qj9FKEgrTWJRZVRoV21acTR0N3cheiVDKkYpSkBOY1I=",
 *   "product_type":"Produt_type_name-board_version"
 * }
 */

bool
BoardInfo_getDataJson(uint8_t* buffer, size_t size) {
    bool success = true;

    uint8_t b64_hashed_board_id[BASE64_HASHED_BOARD_ID_SIZE + 1U] = {0U};
    HashAdapter_getBase64HashedBoardId(b64_hashed_board_id);

    success &= Json_startString((char*)buffer, size);
    success &= Json_addData((char*)buffer, size, "board_id", (const char*)b64_hashed_board_id);
    success &= Json_addData((char*)buffer, size, "manufacturer_id", BASE64_MANUFACTURER_ID);
    success &= Json_addData((char*)buffer, size, "product_type", PRODUCT_TYPE);
    success &= Json_endString((char*)buffer, size);

    return success;
}
