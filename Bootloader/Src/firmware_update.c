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

#include <firmware_update.h>
#include <string.h>

#include "utils.h"
#include "usbd_cdc_if.h"
#include "crc32.h"
#include "version.h"

#ifdef SECURITY_ENABLED
#include "authentication.h"
#endif

#define CHECK_SINGATURE_CMD         "check_signature"       //!< String command for bootloader to check firmware signature
#define ERASE_CMD                   "erase"                 //!< String command for bootloader to erase the flash
#define GET_BOARD_ID_CMD            "board_id"              //!< String command for bootloader to send board id to IMFlasher application
#define GET_VERSION_CMD             "version"               //!< String command for bootloader to return version
#define DISCONNECT_CMD              "disconnect"            //!< String command for bootloader to disconnect
#define VERIFY_FLASHER_CMD          "IMFlasher_Verify"      //!< String for bootloader to verify IMFlasher application
#define EXIT_BL_CMD                 "exit_bl"               //!< String command for exit bootloader
#define IS_FW_PROTECTED_CMD         "is_fw_protected"       //!< String command for bootloader to return if firmware is protected
#define ENABLE_FW_PROTECTION_CMD    "enable_fw_protection"  //!< String command to protect application flash
#define DISABLE_FW_PROTECTION_CMD   "disable_fw_protection" //!< String command to unprotect application flash
#define SW_TYPE_STR                 "software_type"         //!< String for bootloader to send if IMFlasher is connected to bootloader
#define IM_BOOTLOADER_STR           "IMBootloader"          //!< String for IMFlasher application to verify bootloader

#define BUFFER_SIZE     2048u   //!< Bootloader buffer size
#define HASH_SIZE       32u     //!< Hashed board id value size in bytes
#define CRC_SIZE        4u      //!< CRC size in bytes (CRC32)

/* Enumeration for bootloader state machine*/
typedef enum fwUpdateState_ENUM {
    fwUpdateState_INIT,
    fwUpdateState_CMD_ACTION_SELECT,
    fwUpdateState_ERASE,
    fwUpdateState_RECEIVE_FIRMWARE_SIZE,
    fwUpdateState_CHECK_SIGNATURE,
    fwUpdateState_DOWNLOADING,
    fwUpdateState_FLASHING,
    fwUpdateState_CRC,
    fwUpdateState_END
} fwUpdateState_E;

/* Enumeration for tracking firmware download process */
typedef enum fwFlashingState_ENUM {
    fwFlashingState_SKIP,
    fwFlashingState_FLASH,
    fwFlashingState_LAST,
} fwFlashingState_E;

static void FirmwareUpdate_ack();
static void FirmwareUpdate_noAck();
static bool FirmwareUpdate_checkIfHasSignature(uint8_t* buffer);

static uint8_t im_bootloader[] = IM_BOOTLOADER_STR;
static uint8_t ack_pack[]      = "OK";     //! ACK OK packet for IMFlasher
static uint8_t no_ack_pack[]   = "NOK";    //! ACK NOK packet for IMFlasher
static uint8_t true_str[]      = "TRUE";
static uint8_t false_str[]     = "FALSE";

static uint32_t s_flash_address   = 0u;

static bool s_exit_loop = false;        //! Exit flash loop if needed
static bool s_rdp_enable_flag = false;  //! Enable RDP flag
static bool s_rdp_disable_flag = false; //! Disable RDP flag
static bool s_is_flashing = false;      //! Flash for main loop indicating flashing state
static bool s_is_flashed = false;       //! Flash for main loop indicating end of the flashing process

static fwUpdateState_E s_update_state = fwUpdateState_INIT;
static fwFlashingState_E s_flashing_state = fwFlashingState_SKIP;

static uint8_t s_fw_buffer[BUFFER_SIZE] = {0};
static uint8_t s_hash_buffer[HASH_SIZE + CRC_SIZE];


static uint32_t s_crc_calculated = 0xFFFFFFFF;  //!< First (init) CRC value
static uint32_t s_crc_xor_value = 0xFFFFFFFF;   //!< XOR CRC value

void
FirmwareUpdate_init(void) {
#ifdef SECURITY_ENABLED
    Auth_calcHashBoardId();
    uint8_t boardKey[BOARD_KEY_SIZE];
#endif
}

bool
FirmwareUpdate_communicationHandler(uint8_t* buf, uint32_t length) {

    static uint32_t firmware_size = 0;
    static uint32_t crc_received;
    static uint32_t firmware_size_counter = 0; // we are collecting batches of 64 bytes
    static uint32_t flash_lenght;
    bool success = true;
    uint32_t package_index;

    if (!s_is_flashing) {

        if (0 == strcmp((char*)buf, SW_TYPE_STR)) {
            CDC_Transmit_FS(im_bootloader, sizeof(im_bootloader));
            s_update_state = fwUpdateState_CMD_ACTION_SELECT;
            s_flash_address = FLASH_FIRMWARE_ADDRESS;

        } else if (0 == strcmp((char*)buf, DISCONNECT_CMD)) {
            s_update_state = fwUpdateState_INIT;
            s_flashing_state = fwFlashingState_SKIP;
            FirmwareUpdate_ack();

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
                FirmwareUpdate_ack();

            } else if (0 == strcmp((char*)buf, CHECK_SINGATURE_CMD)) {

                s_update_state = fwUpdateState_CHECK_SIGNATURE;
                FirmwareUpdate_ack();

            } else if (0 == strcmp((char*)buf, GET_VERSION_CMD)) {

                uint8_t buffer[300] = {0};
                Version_copyToBuffer(buffer, sizeof(buffer));
                CDC_Transmit_FS((uint8_t*)buffer, strlen((char*)buffer));

            } else if (0 == strcmp((char*)buf, GET_BOARD_ID_CMD)) {

#ifdef SECURITY_ENABLED
                Auth_getHashedBoardId(s_hash_buffer);
                s_update_state = fwUpdateState_CMD_ACTION_SELECT;
#else
                memcpy(s_hash_buffer, NOT_SECURED_MAGIC_STRING, HASH_SIZE);
#endif
                uint32_t crc = CalculateCRC32(s_hash_buffer, HASH_SIZE, s_crc_calculated, false, false);
                crc ^= s_crc_xor_value;
                Utils_Serialize32BE(&s_hash_buffer[HASH_SIZE], crc);
                CDC_Transmit_FS(s_hash_buffer, sizeof(s_hash_buffer));

            } else if (0 == strcmp((char*)buf, EXIT_BL_CMD)) {
                s_update_state = fwUpdateState_INIT;
                s_flashing_state = fwFlashingState_SKIP;
                FirmwareUpdate_ack();
                memset((void*)MAGIC_KEY_ADDRESS_RAM, 0x0, sizeof(uint64_t));
                s_exit_loop = true;

            } else if (0 == strcmp((char*)buf, IS_FW_PROTECTED_CMD)) {

                bool is_flash_protected = FlashAdapter_isFlashRDPProtected();

                if (is_flash_protected) {
                    CDC_Transmit_FS(true_str, sizeof(true_str));

                } else {
                    CDC_Transmit_FS(false_str, sizeof(false_str));
                }

            } else if (0 == strcmp((char*)buf, ENABLE_FW_PROTECTION_CMD)) {

                FirmwareUpdate_ack();
                s_rdp_enable_flag = true;

            } else if (0 == strcmp((char*)buf, DISABLE_FW_PROTECTION_CMD)) {

                FirmwareUpdate_ack();
                s_rdp_disable_flag = true;

            } else {
                //FirmwareUpdate_noAck();
            }

            break;

        case fwUpdateState_CHECK_SIGNATURE:

            success = FirmwareUpdate_checkIfHasSignature(buf);
            s_update_state = fwUpdateState_CMD_ACTION_SELECT;

            if (success) {
                s_is_flashing = true;
                FirmwareUpdate_ack();
            } else {
                FirmwareUpdate_noAck();
            }

            break;

        case fwUpdateState_RECEIVE_FIRMWARE_SIZE:

            firmware_size = Utils_StringToInt(buf, length);

            if (firmware_size <= FIRMWARE_FLASH_SIZE_LIMIT) {
                s_update_state = fwUpdateState_ERASE;
                FirmwareUpdate_ack();
            } else {
                FirmwareUpdate_noAck();
            }

            break;

        case fwUpdateState_ERASE:

            success = FlashAdapter_erase(firmware_size, s_flash_address);
            if (success) {
                FirmwareUpdate_ack();
            }

            if (success) {
                s_update_state = fwUpdateState_DOWNLOADING;
                FirmwareUpdate_ack();
            } else {
                FirmwareUpdate_noAck();
            }

            break;

        case fwUpdateState_DOWNLOADING:

            package_index = firmware_size_counter % PACKET_SIZE;

            memcpy(&(s_fw_buffer[package_index]), buf, length);
            firmware_size_counter += length;

            package_index = firmware_size_counter % PACKET_SIZE;
            if ((0u == package_index) && (firmware_size != firmware_size_counter)) {

                flash_lenght = PACKET_SIZE;
                s_flashing_state = fwFlashingState_FLASH;

            } else if (firmware_size == firmware_size_counter) {

                flash_lenght = (firmware_size_counter % PACKET_SIZE);
                if (flash_lenght == 0u) {
                    flash_lenght = PACKET_SIZE;
                }
                s_flashing_state = fwFlashingState_LAST;
                s_update_state = fwUpdateState_CRC;
            } else {
                //keep collecting
                s_flashing_state = fwFlashingState_SKIP;
            }

            break;

        case fwUpdateState_FLASHING:
            break;

        case fwUpdateState_CRC:

            crc_received = Utils_StringToInt(buf, length);
            s_crc_calculated ^= s_crc_xor_value;
            if (crc_received == s_crc_calculated) {
                FirmwareUpdate_ack();
                s_is_flashing = false;
                s_is_flashed  = true;
            } else {
                FirmwareUpdate_noAck();
            }
            s_update_state = fwUpdateState_END;

            break;

        case fwUpdateState_END:
            break;

        default:
            break;
    }

    success = success & FirmwareUpdate_flashingHandler(flash_lenght);

    return success;
}


bool
FirmwareUpdate_flashingHandler(uint32_t flash_lenght) {

    bool success = false;

    switch (s_flashing_state) {

        case fwFlashingState_FLASH:

            success = FirmwareUpdate_flash(&(s_fw_buffer[0]), flash_lenght);

            if (success) {
                FirmwareUpdate_ack();
            } else {
                FirmwareUpdate_noAck();
            }
            break;

        case fwFlashingState_LAST:

            success = FirmwareUpdate_flash(&(s_fw_buffer[0]), flash_lenght);

            if (success) {
                FirmwareUpdate_ack();
            } else {
                FirmwareUpdate_noAck();
            }
            break;

        case fwFlashingState_SKIP:
            //do nothing
            break;

        default:
            //do nothing
            break;
    }

    return success;
}

bool
FirmwareUpdate_flash(uint8_t* write_buffer, uint32_t flash_lenght) {
    bool success = false;
    static uint32_t index = 0;
    uint32_t address;
    uint8_t readoutBuffer[PACKET_SIZE];

    address = s_flash_address + (index * PACKET_SIZE);
    success = FlashAdapter_program(address, write_buffer, flash_lenght);

    if (success) {
        success = FlashAdapter_readBytes(address, readoutBuffer, flash_lenght);
    }

    //calculate crc
    if (success) {
        s_crc_calculated = CalculateCRC32(readoutBuffer, flash_lenght, s_crc_calculated, false, false);
    }

    if (success) {
        for (uint32_t i = 0; success & (i < flash_lenght); i++) {
            if (write_buffer[i] == readoutBuffer[i]) {
                success = true;
            } else {
                success = false;
            }
        }
    }

    index++;

    return success;
}

bool
FirmwareUpdate_bootloaderLoop(uint32_t timeout) {

    // timeout = 0 -> no timeout
    bool retVal = true;
    if ((timeout != 0) && (HAL_GetTick() > timeout) && (!s_is_flashing)) {
        retVal = false;
    }

    if (s_is_flashed) {
        HAL_Delay(100); // wait for last ACK to be send (main loop shall wait)
        FlashAdapter_finish();
    }

    if (s_is_flashing) {
        HAL_Delay(100);
        GpioAdapter_ledToggle();
    }

    if (s_exit_loop) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        retVal = false;
    }

    if (s_rdp_enable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        FlashAdapter_setReadProtection(true);
    }

    if (s_rdp_disable_flag) {
        HAL_Delay(100); // wait for ACK to be send (main loop shall wait)
        //BL is deleted after this, it needs to be flashed again
        FlashAdapter_setReadProtection(false);
    }

    return retVal;
}

static void
FirmwareUpdate_ack() {
    CDC_Transmit_FS(ack_pack, sizeof(ack_pack));
}

static void
FirmwareUpdate_noAck() {
    s_is_flashing = false;
    s_update_state = fwUpdateState_CMD_ACTION_SELECT;
    CDC_Transmit_FS(no_ack_pack, sizeof(no_ack_pack));
}

static bool
FirmwareUpdate_checkIfHasSignature(uint8_t* buffer) {

    bool success = false;
    uint64_t signatureMagicKey = SINGATURE_MAGIC_KEY;

    if (0 == memcmp(buffer, &signatureMagicKey, sizeof(SINGATURE_MAGIC_KEY))) {
        success = true;
    }

    return success;
}


