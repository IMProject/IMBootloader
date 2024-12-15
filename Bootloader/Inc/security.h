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

#ifndef BOOTLOADER_INC_SECURITY_H_
#define BOOTLOADER_INC_SECURITY_H_

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef enum securityAlgorithm_ENUM {
    BLAKE2B = 0,
    SHA256
} securityAlgorithm_E;

#define MAC_SIZE                    16U
#define NONCE_SIZE                  24U
#define DATA_SIZE                   256U
#define SECURE_PACKET_SIZE          296U // PACKET_SIZE = MAC_SIZE + NONCE_SIZE + DATA_SIZE

#define SERVER_SECURITY_DATA_SIZE 175U

#define SIGNATURE_ALGORITHM               BLAKE2B //!< Selected algorithm for calculating public key signature

#define PRESHARED_KEY_SIZE                32U //!< Size for preshared key binary
#define PRESHARED_KEY_SIZE_BASE64_STR     45U //!< Size for preshared key string in base64 format including null-terminator
#define PRESHARED_KEY_BASE64_STR          (const char*)("cHJlc2hhcmVkX2tleV9pbl8zMl9ieXRlc19mb3JtYXQ=")

#ifndef UNIT_TEST
static_assert(strlen((const char*)PRESHARED_KEY_BASE64_STR) == (PRESHARED_KEY_SIZE_BASE64_STR - 1U), "PRESHARED_KEY_BASE64_STR is wrong size");
#endif

bool Security_setServerSecurityDataJson(char* buffer, uint16_t buffer_size);
bool Security_getClientSecurityDataJson(char* buffer, uint16_t buffer_size);
bool Security_isSecured(void);
bool Security_decrypt(
    const uint8_t mac[MAC_SIZE],
    const uint8_t nonce[NONCE_SIZE],
    const uint8_t* cipher_data,
    uint8_t* plain_data,
    const uint16_t data_length);
void Security_wipeKeys(void);

#endif /* BOOTLOADER_INC_SECURITY_H_ */
