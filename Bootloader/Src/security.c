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

#include "base64.h"
#include "json.h"
#include "monocypher.h"
#include "security.h"
#include "security_adapter.h"


#define SIGNATURE_SIZE              64U //!< Size for signature binary
#define SIGNATURE_SIZE_BASE64_STR   89U //!< Size for signature string in base64 format including null-terminator
#define KEY_SIZE                    32U
#define KEY_SIZE_BASE64_STR         45U //!< Size for public key string in base64 format including null-terminator

static bool Security_getPresharedKey(uint8_t* preshared_key);
static bool Security_verifySignature(const uint8_t signature[SIGNATURE_SIZE], const uint8_t public_key[KEY_SIZE]);
static bool Security_calculateSignatureForPublicKey(const uint8_t public_key[KEY_SIZE], uint8_t signature[SIGNATURE_SIZE]);
static bool Security_generateNewSecretKey(uint8_t secret_key[KEY_SIZE]);
static void Security_generateNewPublicKey(const uint8_t secret_key[KEY_SIZE], uint8_t public_key[KEY_SIZE]);
static void Security_calculateSharedKey(const uint8_t your_secret_key[KEY_SIZE], const uint8_t their_public_key[KEY_SIZE], uint8_t shared_key[KEY_SIZE]);

static uint8_t s_client_public_key[KEY_SIZE];
static uint8_t s_server_public_key[KEY_SIZE];
static uint8_t s_client_secret_key[KEY_SIZE]; // (secret key == private key)
static uint8_t s_shared_key[KEY_SIZE]; // For file decryption


/* JSON String
 * {
*   "public_key":"mCwoSzGVUKZ9joFPj0EXZqoAh9WOI4QDv2LNqTq6LfTH6W6iEn8TCQATpDLXG9aC",
*   "public_key_signature":"bUN3b1N6R1ZVS1o5am9GUGowRVhacW9BaDlXT0k0UUR2MkxOcVRxNkxmVEg2VzZpRW44VENRQVRwRExYRzlhQw=="
*  }
*/

bool
Security_setServerSecurityDataJson(char* buffer, uint16_t buffer_size) {
    bool success = true;

    if ((strlen(buffer) != SERVER_SECURITY_DATA_SIZE) && (buffer_size != SERVER_SECURITY_DATA_SIZE)) {
        success = false;
    }

    char server_public_key_base64[KEY_SIZE_BASE64_STR];

    success = Json_findByKey(buffer, buffer_size, "public_key", server_public_key_base64, KEY_SIZE_BASE64_STR);

    if (success) {
        uint32_t ret_val = Base64_decode(server_public_key_base64, strlen(server_public_key_base64), s_server_public_key, KEY_SIZE);

        if (ret_val != 0U) {
            success = false;
        }
    }

    char server_signature_base64[SIGNATURE_SIZE_BASE64_STR];
    success = Json_findByKey(buffer, buffer_size, "public_key_signature", server_signature_base64, SIGNATURE_SIZE_BASE64_STR);

    uint8_t server_signature[SIGNATURE_SIZE];

    if (success) {
        uint32_t ret_val = Base64_decode(server_signature_base64, strlen(server_signature_base64), server_signature, SIGNATURE_SIZE);

        if (ret_val != 0U) {
            success = false;
        }
    }

    if (success) {
        success = Security_verifySignature(server_signature, s_server_public_key);
        Security_calculateSharedKey(s_client_secret_key, s_server_public_key, s_shared_key);
    }

    return success;
}

bool
Security_getClientSecurityDataJson(char* buffer, uint16_t buffer_size) {

    bool success = true;

    char public_key_base64[KEY_SIZE_BASE64_STR];
    char client_signature_base64[SIGNATURE_SIZE_BASE64_STR];

    success = Security_generateNewSecretKey(s_client_secret_key);

    if (success) {

        Security_generateNewPublicKey(s_client_secret_key, s_client_public_key);
        int ret_val = Base64_encode(s_client_public_key, KEY_SIZE, public_key_base64, KEY_SIZE_BASE64_STR);

        if (0 != ret_val) {
            success = false;
        }
    }

    if (success) {
        uint8_t signature[SIGNATURE_SIZE];

        success = Security_calculateSignatureForPublicKey(s_client_public_key, signature);
        if (success) {
            int ret_val = Base64_encode(signature, SIGNATURE_SIZE, client_signature_base64, SIGNATURE_SIZE_BASE64_STR);

            if (0 != ret_val) {
                success = false;
            }
        }

    }

    if (success) {
        success &= Json_startString(buffer, buffer_size);
        success &= Json_addData(buffer, buffer_size, "public_key", public_key_base64);
        success &= Json_addData(buffer, buffer_size, "public_key_signature", client_signature_base64);
        success &= Json_endString(buffer, buffer_size);
    }

    return success;
}

bool
Security_isSecured(void) {

#ifdef  SECURED
    return true;
#else
    return false;
#endif
}

bool
Security_decrypt(
    const uint8_t mac[MAC_SIZE],
    const uint8_t nonce[NONCE_SIZE],
    const uint8_t* cipher_data,
    uint8_t* plain_data,
    const uint16_t data_length) {

    bool success = false;

    int ret_val = crypto_unlock(plain_data, s_shared_key, nonce, mac, cipher_data, data_length);

    if (ret_val == 0) {
        success = true;
    }

    return success;
}

void
Security_wipeKeys(void) {
    crypto_wipe(s_shared_key, KEY_SIZE);
    crypto_wipe(s_client_secret_key, KEY_SIZE);
}

static bool
Security_verifySignature(const uint8_t signature[SIGNATURE_SIZE], const uint8_t public_key[KEY_SIZE]) {

    bool success = false;

    uint8_t preshared_key[PRESHARED_KEY_SIZE];
    success = Security_getPresharedKey(preshared_key);

    if (success) {

        uint8_t calc_signature[SIGNATURE_SIZE];

        switch (SIGNATURE_ALGORITHM) {

            case (BLAKE2B):
                crypto_blake2b_general((uint8_t*)calc_signature, SIGNATURE_SIZE, (const uint8_t*)preshared_key, PRESHARED_KEY_SIZE, (const uint8_t*)public_key, KEY_SIZE);
                break;

            case (SHA256):
                //Only SMT32H75x supports HW accelerator
                break;
            default:
                break;
        }

        crypto_wipe(preshared_key, PRESHARED_KEY_SIZE);

        if (0 != memcmp((const void*)calc_signature, (const void*)signature, SIGNATURE_SIZE)) {
            success = false;
        }
    }


    return success;
}

static bool
Security_calculateSignatureForPublicKey(const uint8_t public_key[KEY_SIZE], uint8_t signature[SIGNATURE_SIZE]) {

    bool success = false;

    uint8_t preshared_key[PRESHARED_KEY_SIZE];
    success = Security_getPresharedKey(preshared_key);

    switch (SIGNATURE_ALGORITHM) {

        case (BLAKE2B):
            crypto_blake2b_general((uint8_t*)signature, SIGNATURE_SIZE, (const uint8_t*)preshared_key, PRESHARED_KEY_SIZE, (const uint8_t*)public_key, KEY_SIZE);
            break;

        case (SHA256):
            //Only SMT32H75x supports HW accelerator
            break;
        default:
            break;
    }

    crypto_wipe(preshared_key, PRESHARED_KEY_SIZE);

    return success;
}

static bool
Security_generateNewSecretKey(uint8_t secret_key[KEY_SIZE]) {

    bool success = false;

    success = SecurityAdapter_getRandomData(secret_key, KEY_SIZE);

    return success;
}

static void
Security_generateNewPublicKey(const uint8_t secret_key[KEY_SIZE], uint8_t public_key[KEY_SIZE]) {
    crypto_key_exchange_public_key(public_key, secret_key);
}

static void
Security_calculateSharedKey(const uint8_t your_secret_key[KEY_SIZE], const uint8_t their_public_key[KEY_SIZE], uint8_t shared_key[KEY_SIZE]) {
    crypto_key_exchange(shared_key, your_secret_key, their_public_key);
}

static bool
Security_getPresharedKey(uint8_t* preshared_key) {
    bool success = false;

    int32_t ret = Base64_decode(PRESHARED_KEY_BASE64_STR, strlen(PRESHARED_KEY_BASE64_STR), preshared_key, PRESHARED_KEY_SIZE);

    if (0 == ret) {
        success = true;
    }

    return success;
}
