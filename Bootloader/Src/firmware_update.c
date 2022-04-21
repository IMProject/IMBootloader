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

#include "firmware_update.h"

#include <string.h>

#include "utils.h"
#include "usbd_cdc_if.h"
#include "crc32.h"
#include "version.h"


#define CHECK_SINGATURE_CMD         "check_signature"       //!< String command for bootloader to check firmware signature
#define ERASE_CMD                   "erase"                 //!< String command for bootloader to erase the flash
#define GET_BOARD_ID_CMD            "board_id"              //!< String command for bootloader to send board id
#define GET_BOARD_INFO_JSON_CMD     "board_info_json"       //!< String command for bootloader to send board info in JSON format
#define GET_VERSION_CMD             "version"               //!< String command for bootloader to send version
#define GET_VERSION_JSON_CMD        "version_json"          //!< String command for bootloader to send version in JSON format
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
typedef enum fwUpdateState_ENUM {
    fwUpdateState_IDLE,
    fwUpdateState_CMD_ACTION_SELECT,
    fwUpdateState_ERASE,
    fwUpdateState_RECEIVE_FIRMWARE_SIZE,
    fwUpdateState_CHECK_SIGNATURE,
    fwUpdateState_DOWNLOADING_AND_FLASHING,
    fwUpdateState_CRC
} fwUpdateState_E;

static bool FirmwareUpdate_sendStringWithCrc(uint8_t* string, size_t size);
inline static bool FirmwareUpdate_sendMessage(uint8_t* data, uint16_t length);
inline static bool FirmwareUpdate_hasSignature(const uint64_t* buffer);

static bool s_exit_loop = false;        //!< Exit flash loop if needed
static bool s_rdp_enable_flag = false;  //!< Enable RDP flag
static bool s_rdp_disable_flag = false; //!< Disable RDP flag
static bool s_is_flashing = false;      //!< Flash for main loop indicating flashing state
static bool s_is_flashed = false;       //!< Flash for main loop indicating end of the flashing process

static uint8_t s_hashed_board_key[HASHED_BOARD_ID_SIZE];
static uint32_t s_crc_calculated = CRC_INIT_VALUE;

void
FirmwareUpdate_init(void) {
#ifndef FAKE_BOARD_ID
    HashAdapter_getHashedBoardId(s_hashed_board_key);
#endif
}

bool
FirmwareUpdate_communicationHandler(uint8_t* buf, uint32_t length) {
    static uint8_t im_bootloader[]  = {'I', 'M', 'B', 'o', 'o', 't', 'l', 'o', 'a', 'd', 'e', 'r', '\0'};
    static uint8_t ack_pack[]       = {'O', 'K', '\0'};
    static uint8_t no_ack_pack[]    = {'N', 'O', 'K', '\0'};
    static uint8_t true_str[]       = {'T', 'R', 'U', 'E', '\0'};
    static uint8_t false_str[]      = {'F', 'A', 'L', 'S', 'E', '\0'};

    static fwUpdateState_E s_update_state = fwUpdateState_IDLE;
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
            success = FirmwareUpdate_sendMessage(im_bootloader, sizeof(im_bootloader));
            if (success) {
                s_update_state = fwUpdateState_CMD_ACTION_SELECT;
            } else {
                s_update_state = fwUpdateState_IDLE;
            }

        } else if (0 == strcmp((char*)buf, DISCONNECT_CMD)) {
            s_update_state = fwUpdateState_IDLE;
            success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

        } else {
            // Do nothing
        }
    }

    switch (s_update_state) {

        case fwUpdateState_CMD_ACTION_SELECT:
            /* Different bootloader actions. Not necessary to flash firmware from here. */

            firmware_size = Utils_StringToInt(buf, length);

            if (0 == strcmp((char*)buf, VERIFY_FLASHER_CMD)) {
                s_update_state = fwUpdateState_RECEIVE_FIRMWARE_SIZE;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, CHECK_SINGATURE_CMD)) {
                s_update_state = fwUpdateState_CHECK_SIGNATURE;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, GET_VERSION_CMD)) {
                success = Version_getData(tx_buffer, sizeof(tx_buffer));
                if (success) {
                    success = FirmwareUpdate_sendMessage(tx_buffer, strlen((char*)tx_buffer));
                }

            } else if (0 == strcmp((char*)buf, GET_BOARD_ID_CMD)) {

#ifdef FAKE_BOARD_ID
                (void*)memcpy(tx_buffer, FAKE_BOARD_ID, HASHED_BOARD_ID_SIZE);

#else
                (void*)memcpy(tx_buffer, s_hashed_board_key, HASHED_BOARD_ID_SIZE);
#endif
                uint32_t crc = CalculateCRC32(tx_buffer, HASHED_BOARD_ID_SIZE, s_crc_calculated, XOR_CRC_VALUE, false, false, true);
                Utils_Serialize32BE(&tx_buffer[HASHED_BOARD_ID_SIZE], crc);
                success = FirmwareUpdate_sendMessage(tx_buffer, HASHED_BOARD_ID_SIZE + CRC_SIZE);

            } else if (0 == strcmp((char*)buf, GET_BOARD_INFO_JSON_CMD)) {
                if (BoardInfo_getDataJson(tx_buffer, sizeof(tx_buffer))) {
                    // MISRA
                }
                success = FirmwareUpdate_sendStringWithCrc(tx_buffer, sizeof(tx_buffer));

            } else if (0 == strcmp((char*)buf, GET_VERSION_JSON_CMD)) {
                success = Version_getDataJson(tx_buffer, sizeof(tx_buffer));
                if (success) {
                    success = FirmwareUpdate_sendStringWithCrc(tx_buffer, sizeof(tx_buffer));
                }

            } else if (0 == strcmp((char*)buf, EXIT_BL_CMD)) {
                s_update_state = fwUpdateState_IDLE;
                // cppcheck-suppress misra-c2012-11.6; conversion is needed to set value stored at MAGIC_KEY_ADDRESS_RAM to 0
                (void*)memset((void*)MAGIC_KEY_ADDRESS_RAM, 0, sizeof(uint64_t));
                s_exit_loop = true;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, IS_FW_PROTECTED_CMD)) {
                bool is_flash_protected = FlashAdapter_isFlashRDPProtected();

                if (is_flash_protected) {
                    success = FirmwareUpdate_sendMessage(true_str, sizeof(true_str));
                } else {
                    success = FirmwareUpdate_sendMessage(false_str, sizeof(false_str));
                }

            } else if (0 == strcmp((char*)buf, ENABLE_FW_PROTECTION_CMD)) {
                s_rdp_enable_flag = true;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, DISABLE_FW_PROTECTION_CMD)) {
                s_rdp_disable_flag = true;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));

            } else {
                // NO ACK
            }

            break;

        case fwUpdateState_CHECK_SIGNATURE: {
            uint64_t signature;
            (void*)memcpy((void*)&signature, (void*)buf, sizeof(uint64_t));
            success = FirmwareUpdate_hasSignature(&signature);
            s_update_state = fwUpdateState_CMD_ACTION_SELECT;

            if (success) {
                s_is_flashing = true;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                s_is_flashing = false;
                success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;
        }
        case fwUpdateState_RECEIVE_FIRMWARE_SIZE:

            firmware_size = Utils_StringToInt(buf, length);

            if (firmware_size <= FIRMWARE_FLASH_SIZE_LIMIT) {
                s_update_state = fwUpdateState_ERASE;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                s_is_flashing = false;
                s_update_state = fwUpdateState_CMD_ACTION_SELECT;
                success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_ERASE:

            success = FlashAdapter_erase(firmware_size, FLASH_FIRMWARE_ADDRESS);

            if (success) {
                s_update_state = fwUpdateState_DOWNLOADING_AND_FLASHING;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                s_is_flashing = false;
                s_update_state = fwUpdateState_CMD_ACTION_SELECT;
                success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_DOWNLOADING_AND_FLASHING:

            package_index = firmware_size_counter % PACKET_SIZE;
            (void*)memcpy(&(fw_buffer[package_index]), buf, length);
            firmware_size_counter += length;

            package_index = firmware_size_counter % PACKET_SIZE;

            if ((0u == package_index) && (firmware_size != firmware_size_counter)) {
                success = FirmwareUpdate_flash(&(fw_buffer[0]), PACKET_SIZE);
                if (success) {
                    success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
                } else {
                    s_is_flashing = false;
                    s_update_state = fwUpdateState_CMD_ACTION_SELECT;
                    success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
                }

            } else if (firmware_size == firmware_size_counter) {
                flash_length = (firmware_size_counter % PACKET_SIZE);
                if (flash_length == 0u) {
                    flash_length = PACKET_SIZE;
                }
                success = FirmwareUpdate_flash(&(fw_buffer[0]), flash_length);
                if (success) {
                    s_update_state = fwUpdateState_CRC;
                    success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
                } else {
                    s_is_flashing = false;
                    s_update_state = fwUpdateState_CMD_ACTION_SELECT;
                    success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
                }
            } else {
                // Keep collecting
            }

            break;

        case fwUpdateState_CRC:

            crc_received = Utils_StringToInt(buf, length);
            s_crc_calculated ^= XOR_CRC_VALUE;
            if (crc_received == s_crc_calculated) {
                s_is_flashed  = true;
                success = FirmwareUpdate_sendMessage(ack_pack, sizeof(ack_pack));
            } else {
                success = FirmwareUpdate_sendMessage(no_ack_pack, sizeof(no_ack_pack));
            }
            s_is_flashing = false;
            s_update_state = fwUpdateState_IDLE;

            break;

        case fwUpdateState_IDLE:
            // Do nothing
            break;

        default:
            // Do nothing
            break;
    }

    return success;
}

bool
FirmwareUpdate_flash(uint8_t* write_buffer, const uint32_t flash_length) {
    bool success = false;
    // cppcheck-suppress misra-c2012-18.8; PACKET_SIZE is defined as 256 or W25Q_PAGE_SIZE, depends on EXTERNAL_FLASH
    uint8_t readout_buffer[PACKET_SIZE];
    static uint32_t index = 0U;
    const uint32_t address_addition = index * PACKET_SIZE;
    uint64_t address = FLASH_FIRMWARE_ADDRESS + address_addition;

    success = FlashAdapter_program((uint32_t)address, write_buffer, flash_length);

    if (success) {
        success = FlashAdapter_readBytes((uint32_t)address, readout_buffer, flash_length);

        if (success) {
            s_crc_calculated = CalculateCRC32(readout_buffer, flash_length, s_crc_calculated, 0U, false, false, false);
            for (uint32_t i = 0U; (success) && (i < flash_length); ++i) {
                if (write_buffer[i] != readout_buffer[i]) {
                    success = false;
                }
            }
        }
    }

    ++index;

    return success;
}

bool
FirmwareUpdate_bootloaderLoop(const uint32_t timeout) {
    // timeout = 0 -> no timeout
    bool ret_val = true;
    if ((timeout != 0U) && (HAL_GetTick() > timeout) && (!s_is_flashing)) {
        ret_val = false;
    }

    if (s_is_flashed) {
        HAL_Delay(100); // wait for last ACK to be send (main loop shall wait)
        FlashAdapter_finish();
    }

    if (s_is_flashing) {
        HAL_Delay(100);
        GpioAdapter_led1Toggle();
    }

    if (s_exit_loop) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        ret_val = false;
    }

    if (s_rdp_enable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        ret_val = FlashAdapter_setReadProtection(true);
    }

    if (s_rdp_disable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        // BL is deleted after this, it needs to be flashed again
        ret_val = FlashAdapter_setReadProtection(false);
    }

    return ret_val;
}

static bool
FirmwareUpdate_sendStringWithCrc(uint8_t* string, size_t size) {

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
            success = FirmwareUpdate_sendMessage(string, last_char + sizeof(crc));
        }
    }

    return success;
}

inline static bool
FirmwareUpdate_sendMessage(uint8_t* data, uint16_t length) {
    return (CDC_Transmit_FS(data, length) == (uint8_t)USBD_OK);
}

inline static bool
FirmwareUpdate_hasSignature(const uint64_t* buffer) {
    const uint64_t signature_magic_key = SIGNATURE_MAGIC_KEY;
    return (0 == memcmp(buffer, &signature_magic_key, sizeof(SIGNATURE_MAGIC_KEY)));
}
