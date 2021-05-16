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

#include "version.h"
#include <string.h>

#define VERSION_INFO_STR    "Bootloader info"
#define BRANCH_NAME_STR     "Branch name: "
#define COMMIT_HASH_STR     "Commit hash: "
#define LAST_TAG_STR        "Last tag: "
#define NEW_ROW_STR         "\r\n"

const char git_branch[20] = GIT_BRANCH;
const char git_hash[42] = GIT_HASH;
const char git_tag[20] = GIT_TAG;

void
Version_copyToBuffer(uint8_t* buffer, uint16_t size) {

    uint16_t str_size = strlen(BRANCH_NAME_STR) + strlen(COMMIT_HASH_STR) + strlen(LAST_TAG_STR);
    str_size += (3 * strlen(NEW_ROW_STR)) + strlen(git_branch) + strlen(git_hash) + strlen(git_tag);

    if (size >= str_size) {

        uint16_t buffer_index = 0;

        memcpy(&buffer[buffer_index], VERSION_INFO_STR, strlen(VERSION_INFO_STR));
        buffer_index += strlen(VERSION_INFO_STR);
        memcpy(&buffer[buffer_index], NEW_ROW_STR, strlen(NEW_ROW_STR));
        buffer_index += strlen(NEW_ROW_STR);

        //BRANCH
        memcpy(&buffer[buffer_index], BRANCH_NAME_STR, strlen(BRANCH_NAME_STR));
        buffer_index += strlen(BRANCH_NAME_STR);
        memcpy(&buffer[buffer_index], git_branch, strlen(git_branch));
        buffer_index += strlen(git_branch);
        memcpy(&buffer[buffer_index], NEW_ROW_STR, strlen(NEW_ROW_STR));
        buffer_index += strlen(NEW_ROW_STR);

        //HASH
        memcpy(&buffer[buffer_index], COMMIT_HASH_STR, strlen(COMMIT_HASH_STR));
        buffer_index += strlen(COMMIT_HASH_STR);
        memcpy(&buffer[buffer_index], git_hash, strlen(git_hash));
        buffer_index += strlen(git_hash);
        memcpy(&buffer[buffer_index], NEW_ROW_STR, strlen(NEW_ROW_STR));
        buffer_index += strlen(NEW_ROW_STR);

        //TAG
        if (strlen(git_tag) != 0) { // in case tag doesn't exist
            memcpy(&buffer[buffer_index], LAST_TAG_STR, strlen(LAST_TAG_STR));
            buffer_index += strlen(LAST_TAG_STR);
            memcpy(&buffer[buffer_index], git_tag, strlen(git_tag));
            buffer_index += strlen(git_tag);
            memcpy(&buffer[buffer_index], NEW_ROW_STR, strlen(NEW_ROW_STR));
            buffer_index += strlen(NEW_ROW_STR);
        }

        buffer[buffer_index] = '\0';
    }
}




