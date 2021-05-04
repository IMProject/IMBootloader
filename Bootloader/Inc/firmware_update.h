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

#ifndef INC_FIRMWAREUPDATE_H_
#define INC_FIRMWAREUPDATE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef EXTERNAL_FLASH

#include "quadspi.h"
#include "w25q.h"
#define FIRMWARE_FLASH_SIZE_LIMIT W25Q_CHIP_SIZE
#define PACKET_SIZE W25Q_PAGE_SIZE //TODO: this time size is 256 and 64 byte package can fit in, Check in the future.

#else
#define FIRMWARE_FLASH_SIZE_LIMIT (FLASH_SIZE - 0x8000u)    //!< Max available flash size for firmware. Flash bank size - FW start address
#define PACKET_SIZE               256                       //!< amount of memory for collecting data from flashing application. Value is synchronized with Flasher APP.
#endif

#define NOT_SECURED_MAGIC_STRING "NOT_SECURED_MAGIC_STRING_1234567" //!< 32 bytes magic string, means this board is not secure
#define MAGIC_KEY_ADDRESS       (0x08007800UL)                      //!< Place in internal flash for communication between bootloader and firmware
#define MAGIC_KEY_VALUE         (0x28101987A5B5C5D5)                //!< Hex value if written in flash bootloader will jump to firmware
#define SINGATURE_MAGIC_KEY     (0xDEC0DE5528101987)                //!< Hex value if the firmware is signed with bootloader will accept it
#define FLASH_FIRMWARE_ADDRESS  (0x08008000UL)                      //!< Address where firmware will be written

void FirmwareUpdate_init(void);
bool FirmwareUpdate_handler(uint8_t* buf, uint32_t length);
bool FirmwareUpdate_flash(uint8_t* buf, uint32_t flashLength);
bool FirmwareUpdate_isFlashing(uint32_t timeout);

/* Adapter functions are separated in order to make porting to different systems easier */
void FirmwareUpdateAdapter_SystemClockConfig(void);
void FirmwareUpdateAdapter_InitGPIO(void);
bool FirmwareUpdateAdapter_flashErase(uint32_t firmware_size, uint32_t flash_address);
bool FirmwareUpdateAdapter_blockErase(uint32_t address);
bool FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length);
bool FirmwareUpdateAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length);
void FirmwareUpdateAdapter_finish(void);
void FirmwareUpdateAdapter_ledToggle(void);

#endif /* INC_FIRMWAREUPDATE_H_ */
