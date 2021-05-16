/*
 * Copyright (C) 2021  Igor Misic, igy1000mb@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *
 *  If not, see <http://www.gnu.org/licenses/>.
 */

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




