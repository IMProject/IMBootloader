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

#define CHECK_SINGATURE_CMD         "check_signature"   //!< String command for bootloader to check firmware signature
#define ERASE_CMD                   "erase"             //!< String command for bootloader to erase the flash
#define GET_BOARD_ID_CMD            "board_id"          //!< String command for bootloader to send board id to IMFlasher application
#define GET_VERSION_CMD             "version"           //!< String command for bootloader to send version
#define VERIFY_FLASHER_CMD          "IMFlasher_Verify"  //!< String for bootloader to verify IMFlasher application
#define SW_TYPE_STR                 "software_type"     //!< String for bootloader to send if IMFlasher is connected to bootloader
#define IM_BOOTLOADER_STR           "IMBootloader"      //!< String for IMFlasher application to verify bootloader

#define BUFFER_SIZE     2048u   //!< Bootloader buffer size
#define HASH_SIZE       32u     //!< Hashed board id value size in bytes
#define CRC_SIZE        4u      //!< CRC size in bytes (CRC32)

/* Enumeration for bootloader state machine*/
typedef enum fwUpdateState_ENUM {
    fwUpdateState_SEND_BOARD_ID,
    fwUpdateState_CMD_ACTION_SELECT,
    fwUpdateState_ERASE,
    fwUpdateState_RX_FW_SIZE,
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

static void FirmwareUpdate_ack(uint8_t* data, uint16_t lenght);
static bool FirmwareUpdate_checkIfHasSignature(uint8_t* buffer);

uint8_t im_bootloader[] = IM_BOOTLOADER_STR;
uint8_t ack_pack[]      = "OK";     //! ACK OK packet for IMFlasher
uint8_t no_ack_pack[]   = "NOK";    //! ACK NOK packet for IMFlasher
uint32_t flashAddress   = 0u;

static bool isFlashing = false;     //! Flash for main loop indicating flashing state
static bool isFlashed  = false;     //! Flash for main loop indicating end of the flashing process

static fwUpdateState_E s_updateState = fwUpdateState_SEND_BOARD_ID;
static fwFlashingState_E s_flashingState = fwFlashingState_SKIP;

uint8_t fwBuffer[BUFFER_SIZE] = {0};
uint8_t hashBuffer[HASH_SIZE + CRC_SIZE];


static uint32_t s_crcCalculated = 0xFFFFFFFF;   //!< First (init) CRC value
static uint32_t s_crc_XorValue = 0xFFFFFFFF;    //!< XOR CRC value

void
FirmwareUpdate_init(void) {
#ifdef SECURITY_ENABLED
    Auth_calcHashBoardId();
    uint8_t boardKey[BOARD_KEY_SIZE];
#endif
}

bool
FirmwareUpdate_handler(uint8_t* buf, uint32_t length) {

    static uint32_t firmwareSize = 0;
    static volatile uint32_t crcReceived;
    static uint32_t firmwareSizeCounter = 0; // we are collecting batches of 64 bytes
    static uint32_t flashLenght;
    bool success = true;
    uint32_t packageIndex;

    if  ((0 == strcmp((char*)buf, SW_TYPE_STR)) && (!isFlashing)) {
        CDC_Transmit_FS(im_bootloader, sizeof(im_bootloader));
        s_updateState = fwUpdateState_CMD_ACTION_SELECT;
        flashAddress = FLASH_FIRMWARE_ADDRESS;
    }

    switch (s_updateState) {

        case fwUpdateState_CMD_ACTION_SELECT:
            /* Different bootloader actions. Not necessary to flash firmware from here. */

            firmwareSize = Utils_StringToInt(buf, length);

            if (0 == strcmp((char*)buf, VERIFY_FLASHER_CMD)) {
                s_updateState = fwUpdateState_RX_FW_SIZE;
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, CHECK_SINGATURE_CMD)) {

                s_updateState = fwUpdateState_CHECK_SIGNATURE;
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));

            } else if (0 == strcmp((char*)buf, GET_VERSION_CMD)) {

                uint8_t buffer[300] = {0};
                Version_copyToBuffer(buffer, sizeof(buffer));
                CDC_Transmit_FS((uint8_t*)buffer, strlen((char*)buffer));

            } else if (0 == strcmp((char*)buf, GET_BOARD_ID_CMD)) {
                isFlashing = true;
#ifdef SECURITY_ENABLED
                Auth_getHashedBoardId(hashBuffer);
                s_updateState = fwUpdateState_CMD_ACTION_SELECT;
#else
                memcpy(hashBuffer, NOT_SECURED_MAGIC_STRING, HASH_SIZE);
                s_updateState = fwUpdateState_CMD_ACTION_SELECT;
#endif
                uint32_t crc = CalculateCRC32(hashBuffer, HASH_SIZE, s_crcCalculated, false, false);
                crc ^= s_crc_XorValue;
                Utils_Serialize32BE(&hashBuffer[HASH_SIZE], crc);
                CDC_Transmit_FS(hashBuffer, sizeof(hashBuffer));

                break;

            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_CHECK_SIGNATURE:
            success = FirmwareUpdate_checkIfHasSignature(buf);

            if (success) {
                s_updateState = fwUpdateState_CMD_ACTION_SELECT;
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_RX_FW_SIZE:

            firmwareSize = Utils_StringToInt(buf, length);

            if (firmwareSize <= FIRMWARE_FLASH_SIZE_LIMIT) {
                s_updateState = fwUpdateState_ERASE;
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_ERASE:

            success = FirmwareUpdateAdapter_flashErase(firmwareSize, flashAddress);
            if (success) {
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            }

            if (success) {
                s_updateState = fwUpdateState_DOWNLOADING;
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }

            break;

        case fwUpdateState_DOWNLOADING:

            packageIndex = firmwareSizeCounter % PACKET_SIZE;

            memcpy(&(fwBuffer[packageIndex]), buf, length);
            firmwareSizeCounter += length;

            packageIndex = firmwareSizeCounter % PACKET_SIZE;
            if ((0u == packageIndex) && (firmwareSize != firmwareSizeCounter)) {

                flashLenght = PACKET_SIZE;
                s_flashingState = fwFlashingState_FLASH;

            } else if (firmwareSize == firmwareSizeCounter) {

                flashLenght = (firmwareSizeCounter % PACKET_SIZE);
                if (flashLenght == 0u) {
                    flashLenght = PACKET_SIZE;
                }
                s_flashingState = fwFlashingState_LAST;
                s_updateState = fwUpdateState_CRC;
            } else {
                //keep collecting
                s_flashingState = fwFlashingState_SKIP;
            }

            break;

        case fwUpdateState_FLASHING:
            break;

        case fwUpdateState_CRC:

            crcReceived = Utils_StringToInt(buf, length);
            s_crcCalculated ^= s_crc_XorValue;
            if (crcReceived == s_crcCalculated) {
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
                isFlashing = false;
                isFlashed  = true;
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }
            s_updateState = fwUpdateState_END;

            break;

        case fwUpdateState_END:
            break;

        default:
            break;
    }

    switch (s_flashingState) {

        case fwFlashingState_FLASH:

            success = FirmwareUpdate_flash(&(fwBuffer[0]), flashLenght);

            if (success) {
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
            }
            break;

        case fwFlashingState_LAST:

            success = FirmwareUpdate_flash(&(fwBuffer[0]), flashLenght);

            if (success) {
                FirmwareUpdate_ack(ack_pack, sizeof(ack_pack));
            } else {
                FirmwareUpdate_ack(no_ack_pack, sizeof(no_ack_pack));
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
FirmwareUpdate_flash(uint8_t* writeBuffer, uint32_t flashLenght) {
    bool success = false;
    static uint32_t index = 0;
    uint32_t address;
    uint8_t readoutBuffer[PACKET_SIZE];

    address = flashAddress + (index * PACKET_SIZE);
    success = FirmwareUpdateAdapter_program(address, writeBuffer, flashLenght);

    if (success) {
        success = FirmwareUpdateAdapter_readBytes(address, readoutBuffer, flashLenght);
    }

    //calculate crc
    if (success) {
        s_crcCalculated = CalculateCRC32(readoutBuffer, flashLenght, s_crcCalculated, false, false);
    }

    if (success) {
        for (uint32_t i = 0; success & (i < flashLenght); i++) {
            if (writeBuffer[i] == readoutBuffer[i]) {
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
FirmwareUpdate_isFlashing(uint32_t timeout) {

    // timeout = 0 -> no timeout
    bool retVal = true;
    if ((timeout != 0) && (HAL_GetTick() > timeout) && (!isFlashing)) {
        retVal = false;
    }

    if (isFlashed) {
        HAL_Delay(100); // wait for last ACK to be send
        FirmwareUpdateAdapter_finish();
    }

    if (isFlashing) {
        HAL_Delay(100);
        FirmwareUpdateAdapter_ledToggle();
    }

    return retVal;
}

static void
FirmwareUpdate_ack(uint8_t* data, uint16_t lenght) {
    CDC_Transmit_FS(data, lenght);
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


