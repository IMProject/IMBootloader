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

#include <string.h>
#include "board_info.h"
#include "gpio_adapter.h"
#include "flash_adapter.h"
#include "hash_adapter.h"
#include "binary_update.h"
#include "communication.h"
#include "signature.h"
#include "crc32.h"
#include "utils.h"
#include "software_info.h"
#include "usbd_cdc_if.h"

#define CHECK_SINGATURE_CMD         "check_signature"       //!< String command for bootloader to check firmware signature
#define ERASE_CMD                   "erase"                 //!< String command for bootloader to erase the flash
#define GET_BOARD_ID_CMD            "board_id"              //!< String command for bootloader to send board id
#define GET_BOARD_INFO_JSON_CMD     "board_info_json"       //!< String command for bootloader to send board info in JSON format
#define GET_VERSION_CMD             "version"               //!< String command for bootloader to send version
#define GET_SW_INFO_JSON_CMD        "software_info_json"    //!< String command for bootloader to send software info in JSON format
#define DISCONNECT_CMD              "disconnect"            //!< String command for bootloader to disconnect
#define VERIFY_FLASHER_CMD          "IMFlasher_Verify"      //!< String for bootloader to verify IMFlasher application
#define EXIT_BL_CMD                 "exit_bl"               //!< String command for exit bootloader
#define IS_FW_PROTECTED_CMD         "is_fw_protected"       //!< String command for bootloader to return if firmware is protected
#define ENABLE_FW_PROTECTION_CMD    "enable_fw_protection"  //!< String command to protect application flash
#define DISABLE_FW_PROTECTION_CMD   "disable_fw_protection" //!< String command to unprotect application flash
#define SW_TYPE_STR                 "software_type"         //!< String for bootloader to send if IMFlasher is connected to bootloader

#define BUFFER_SIZE     (2048U)         //!< Bootloader buffer size
#define CRC_SIZE        (4U)            //!< CRC size in bytes (CRC32)
#define CRC_INIT_VALUE  (0xFFFFFFFFU)   //!< CRC init value
#define XOR_CRC_VALUE   (0xFFFFFFFFU)   //!< XOR CRC value
#define TX_BUFFER_SIZE  (1000U)         //!< TX buffer maximum size

//! Enumeration for bootloader state machine
typedef enum communicationState_ENUM {
    communicationState_IDLE,
    communicationState_CMD_ACTION_SELECT,
    communicationState_ERASE,
    communicationState_RECEIVE_FIRMWARE_SIZE,
    communicationState_CHECK_SIGNATURE,
    communicationState_DOWNLOADING_AND_FLASHING,
    communicationState_CRC
} communicationState_E;

static bool Communication_sendStringWithCrc(uint8_t* string, size_t size);
inline static bool Communication_sendMessage(uint8_t* data, uint16_t length);

static bool s_exit_loop = false;        //!< Exit flash loop if needed
static bool s_rdp_enable_flag = false;  //!< Enable RDP flag
static bool s_rdp_disable_flag = false; //!< Disable RDP flag
static bool s_is_flashing = false;      //!< Flash for main loop indicating flashing state
static bool s_is_flashed = false;       //!< Flash for main loop indicating end of the flashing process

static uint8_t s_hashed_board_key[HASHED_BOARD_ID_SIZE];
static uint32_t s_crc_calculated = CRC_INIT_VALUE;

void
Communication_init(void) {
#ifndef FAKE_BOARD_ID
    HashAdapter_getHashedBoardId(s_hashed_board_key);
#endif
}

bool
Communication_handler(uint8_t* buf, uint32_t length) {
    static uint8_t im_bootloader[]  = {'I', 'M', 'B', 'o', 'o', 't', 'l', 'o', 'a', 'd', 'e', 'r', '\0'};
    static uint8_t ack_pack[]       = {'O', 'K', '\0'};
    static uint8_t no_ack_pack[]    = {'N', 'O', 'K', '\0'};
    static uint8_t true_str[]       = {'T', 'R', 'U', 'E', '\0'};
    static uint8_t false_str[]      = {'F', 'A', 'L', 'S', 'E', '\0'};

    static communicationState_E s_update_state = communicationState_IDLE;
    uint8_t fw_buffer[BUFFER_SIZE];
    static uint32_t firmware_size = 0U;
    uint32_t crc_received;
    static uint32_t firmware_size_counter = 0U;
    uint32_t flash_length;
    bool success = true;
    uint32_t package_index;
    uint8_t tx_buffer[TX_BUFFER_SIZE];

    if (!s_is_flashing) {

        if (0 == strcmp((char*)buf, SW_TYPE_STR)) {
            success = Communication_sendMessage(im_bootloader, sizeof(im_bootloader));
            if (success) {
                s_update_state = communicationState_CMD_ACTION_SELECT;
            } else {
                s_update_state = communicationState_IDLE;
            }

        } else if (0 == strcmp((char*)buf, DISCONNECT_CMD)) {
            s_update_state = communicationState_IDLE;
            success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

        } else {
            // Do nothing
        }
    }

    switch (s_update_state) {

        case communicationState_CMD_ACTION_SELECT:
            /* Different bootloader actions. Not necessary to flash firmware from here. */

            if (0 == strcmp((char*)buf, VERIFY_FLASHER_CMD)) {
                s_update_state = communicationState_RECEIVE_FIRMWARE_SIZE;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, CHECK_SINGATURE_CMD)) {
                s_update_state = communicationState_CHECK_SIGNATURE;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, GET_VERSION_CMD)) {
                success = SwInfo_getVersion(tx_buffer, sizeof(tx_buffer));
                if (success) {
                    success = Communication_sendMessage(tx_buffer, strlen((char*)tx_buffer));
                }

            } else if (0 == strcmp((char*)buf, GET_BOARD_ID_CMD)) {

#ifdef FAKE_BOARD_ID
                (void*)memcpy(tx_buffer, FAKE_BOARD_ID, HASHED_BOARD_ID_SIZE);

#else
                (void*)memcpy(tx_buffer, s_hashed_board_key, HASHED_BOARD_ID_SIZE);
#endif
                uint32_t crc = CalculateCRC32(tx_buffer, HASHED_BOARD_ID_SIZE, s_crc_calculated, XOR_CRC_VALUE, false, false, true);
                Utils_Serialize32BE(&tx_buffer[HASHED_BOARD_ID_SIZE], crc);
                success = Communication_sendMessage(tx_buffer, HASHED_BOARD_ID_SIZE + CRC_SIZE);

            } else if (0 == strcmp((char*)buf, GET_BOARD_INFO_JSON_CMD)) {
                if (BoardInfo_getDataJson(tx_buffer, sizeof(tx_buffer))) {
                    // MISRA
                }
                success = Communication_sendStringWithCrc(tx_buffer, sizeof(tx_buffer));

            } else if (0 == strcmp((char*)buf, GET_SW_INFO_JSON_CMD)) {
                success = SwInfo_getDataJson(tx_buffer, sizeof(tx_buffer));
                if (success) {
                    success = Communication_sendStringWithCrc(tx_buffer, sizeof(tx_buffer));
                }

            } else if (0 == strcmp((char*)buf, EXIT_BL_CMD)) {
                s_update_state = communicationState_IDLE;
                // cppcheck-suppress misra-c2012-11.6; conversion is needed to set value stored at MAGIC_KEY_ADDRESS_RAM to 0
                (void*)memset((void*)MAGIC_KEY_ADDRESS_RAM, 0, sizeof(uint64_t));
                s_exit_loop = true;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, IS_FW_PROTECTED_CMD)) {
                bool is_flash_protected = FlashAdapter_isFlashRDPProtected();

                if (is_flash_protected) {
                    success = Communication_sendMessage(true_str, sizeof(true_str));
                } else {
                    success = Communication_sendMessage(false_str, sizeof(false_str));
                }

            } else if (0 == strcmp((char*)buf, ENABLE_FW_PROTECTION_CMD)) {
                s_rdp_enable_flag = true;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, DISABLE_FW_PROTECTION_CMD)) {
                s_rdp_disable_flag = true;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));

            } else {
                // NO ACK
            }

            break;

        case communicationState_CHECK_SIGNATURE: {
            signature_S signature;
            (void*)memcpy((void*)&signature, (void*)buf, SIGNATURE_SIZE);
            detectedBinary_E binary_detected = Signature_verification(&signature);
            success = BinaryUpdate_handleDetectedBinary(binary_detected);
            s_update_state = communicationState_CMD_ACTION_SELECT;

            if (success) {
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;
        }
        case communicationState_RECEIVE_FIRMWARE_SIZE:

            firmware_size = Utils_StringToInt(buf, length);

            if (firmware_size <= FIRMWARE_FLASH_SIZE_LIMIT) {
                s_is_flashing = true;
                s_update_state = communicationState_ERASE;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                s_is_flashing = false;
                s_update_state = communicationState_CMD_ACTION_SELECT;
                success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case communicationState_ERASE:

            success = BinaryUpdate_erase(firmware_size);

            if (success) {
                s_update_state = communicationState_DOWNLOADING_AND_FLASHING;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                s_is_flashing = false;
                s_update_state = communicationState_CMD_ACTION_SELECT;
                success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case communicationState_DOWNLOADING_AND_FLASHING:

            package_index = firmware_size_counter % PACKET_SIZE;
            (void*)memcpy(&(fw_buffer[package_index]), buf, length);
            firmware_size_counter += length;

            package_index = firmware_size_counter % PACKET_SIZE;

            if ((0u == package_index) && (firmware_size != firmware_size_counter)) {
                success = BinaryUpdate_write(&(fw_buffer[0]), PACKET_SIZE, &s_crc_calculated);
                if (success) {
                    success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
                } else {
                    s_is_flashing = false;
                    s_update_state = communicationState_CMD_ACTION_SELECT;
                    success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
                }

            } else if (firmware_size == firmware_size_counter) {
                flash_length = (firmware_size_counter % PACKET_SIZE);
                if (flash_length == 0u) {
                    flash_length = PACKET_SIZE;
                }
                success = BinaryUpdate_write(&(fw_buffer[0]), flash_length, &s_crc_calculated);
                if (success) {
                    s_update_state = communicationState_CRC;
                    success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
                } else {
                    s_is_flashing = false;
                    s_update_state = communicationState_CMD_ACTION_SELECT;
                    success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
                }
            } else {
                // Keep collecting
            }

            break;

        case communicationState_CRC:

            crc_received = Utils_StringToInt(buf, length);
            s_crc_calculated ^= XOR_CRC_VALUE;
            if (crc_received == s_crc_calculated) {
                s_is_flashed  = true;
                success = Communication_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                success = Communication_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }
            s_is_flashing = false;
            s_update_state = communicationState_IDLE;

            break;

        case communicationState_IDLE:
            // Do nothing
            break;

        default:
            // Do nothing
            break;
    }

    return success;
}

bool
Communication_mainLoop(const uint32_t timeout) {
    // timeout = 0 -> no timeout
    bool stay_in_loop = true;
    if ((timeout != 0U) && (HAL_GetTick() > timeout) && (!s_is_flashing)) {
        stay_in_loop = false;
    }

    if (s_is_flashed) {
        HAL_Delay(100); // wait for last ACK to be send (main loop shall wait)
        bool success = BinaryUpdate_finish();
        if (success) {
            stay_in_loop = false;
        }
    }

    if (s_is_flashing) {
        HAL_Delay(100);
        GpioAdapter_led1Toggle();
    }

    if (s_exit_loop) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        BinaryUpdate_resetJumpAddress();
        stay_in_loop = false;
    }

    if (s_rdp_enable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        stay_in_loop = FlashAdapter_setReadProtection(true);
    }

    if (s_rdp_disable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        // BL is deleted after this, it needs to be flashed again
        stay_in_loop = FlashAdapter_setReadProtection(false);
    }

    return stay_in_loop;
}

static bool
Communication_sendStringWithCrc(uint8_t* string, size_t size) {

    bool success = false;
    bool null_terminator_exist = false;

    for (size_t i = 0U; i < size; ++i) {
        if (string[i] == (uint8_t)('\0')) {
            null_terminator_exist = true;
            break;
        }
    }

    if (null_terminator_exist) {

        size_t last_char = strlen((char*)string);

        if (size >= last_char) {
            uint32_t crc = CalculateCRC32(&string[0], last_char, CRC_INIT_VALUE, XOR_CRC_VALUE, false, false, true);
            Utils_Serialize32BE(&string[last_char], crc);
            success = Communication_sendMessage(string, last_char + sizeof(crc));
        }
    }

    return success;
}

inline static bool
Communication_sendMessage(uint8_t* data, uint16_t length) {
    return (CDC_Transmit_FS(data, length) == (uint8_t)USBD_OK);
}
