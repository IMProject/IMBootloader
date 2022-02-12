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

#ifndef BOOTLOADER_INC_FLASH_ADAPTER_H_
#define BOOTLOADER_INC_FLASH_ADAPTER_H_

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

#ifdef STM32L4xx
#define FLASH_FIRMWARE_ADDRESS  (0x08008000UL)                      //!< Address where firmware will be written
#define MAGIC_KEY_ADDRESS_FLASH (0x08007800UL)                      //!< Address in internal flash for communication between bootloader and firmware
#define MAGIC_KEY_ADDRESS_RAM   (0x20000000UL)                      //!< Address in ram for communication between bootloader and firmware
#elif STM32H7xx
#define FLASH_FIRMWARE_ADDRESS  (0x08020000UL)                      //!< Address where firmware will be written
#define MAGIC_KEY_ADDRESS_FLASH (0x080202A0UL)                      //!< Address in internal flash for communication between bootloader and firmware
#define MAGIC_KEY_ADDRESS_RAM   (0x38000000UL)                      //!< Address in ram for communication between bootloader and firmware
#elif STM32F7xx
#define FLASH_FIRMWARE_ADDRESS  (0x08020000UL)                      //!< Address where firmware will be written
#define MAGIC_KEY_ADDRESS_FLASH (0x08020200UL)                      //!< Address in internal flash for communication between bootloader and firmware
#define MAGIC_KEY_ADDRESS_RAM   (0x20000000UL)                      //!< Address in ram for communication between bootloader and firmware
#endif

#define MAGIC_KEY_VALUE         (0x28101987A5B5C5D5)                //!< Hex value if written in flash bootloader will jump to firmware
#define SIGNATURE_MAGIC_KEY     (0xDEC0DE5528101987)                //!< Hex value if the firmware is signed with bootloader will accept it


bool FlashAdapter_erase(uint32_t firmware_size, uint32_t flash_address);
bool FlashAdapter_blockErase(uint32_t address);
bool FlashAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length);
bool FlashAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length);
void FlashAdapter_finish(void);
bool FlashAdapter_isFlashRDPProtected(void);
bool FlashAdapter_isFlashPCROPProtected(void);
bool FlashAdapter_setReadProtection(bool enable);
/* proprietary code readout protection */
bool FlashAdapter_setPCROP(bool enable, uint32_t protect_address_start, uint32_t protect_address_end);


#endif /* BOOTLOADER_INC_FLASH_ADAPTER_H_ */
