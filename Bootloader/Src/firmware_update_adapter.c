/*
 * Copyright (C) 2021 Igor Misic, igy1000mb@gmail.com
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

#include <firmware_update.h>
#include "main.h"
#include "stm32l4xx_hal.h"
#include <string.h>

#ifdef EXTERNAL_FLASH
bool
FirmwareUpdateAdapter_flashErase(uint32_t firmwareSize, uint32_t flashAddress) {
    return W25q_dynamicErase(firmwareSize, flashAddress);
}

bool
FirmwareUpdateAdapter_blockErase(uint32_t address) {
    return W25q_blockErase64k(address);
}

bool
FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    return W25q_quadPageProgram(address, buffer, length);
}

bool
FirmwareUpdateAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length) {
    return W25q_readBytes(address, buffer, length);
}

void
FirmwareUpdateAdapter_finish(void) {
}

#else
bool
FirmwareUpdateAdapter_flashErase(uint32_t firmware_size, uint32_t flash_address) {
    bool success = false;

    if (firmware_size > FLASH_SIZE) {
        success = false;

    } else {

        HAL_FLASH_Unlock();

        HAL_StatusTypeDef      status = HAL_OK;
        FLASH_EraseInitTypeDef pEraseInit;
        uint32_t               PageError  = 0;

        /* Get the number of pages to erase */
        uint32_t number_of_pages = firmware_size / FLASH_PAGE_SIZE;
        if ((firmware_size % FLASH_PAGE_SIZE) != 0) {
            number_of_pages += 1;
        }
        uint32_t start_page = (flash_address - FLASH_BASE) / FLASH_PAGE_SIZE;

        pEraseInit.Banks     = FLASH_BANK_1;
        pEraseInit.NbPages   = number_of_pages + 1;
        pEraseInit.Page      = start_page;
        pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        status               = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

        if (status == HAL_OK) {
            success = true;
        }

    }

    return success;
}

bool
FirmwareUpdateAdapter_blockErase(uint32_t address) {
    bool success = true;
    return success;
}

bool
FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint64_t data = UINT64_MAX;

    if (length % sizeof(uint64_t) == 0 ) {

        for (uint32_t i = 0; i < length / sizeof(uint64_t); i++) {
            uint32_t memory_index = i * sizeof(uint64_t);
            memcpy((void*)&data, (void*)&buffer[memory_index], sizeof(uint64_t));
            HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + memory_index, data);
            if (status != HAL_OK) {
                success = false;
                break;
            }
        }

    } else if (length < sizeof(uint64_t)) {

        memcpy((void*)&data, (void*)buffer, length);
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);
        if (status != HAL_OK) {
            success = false;
        }
    }

    return success;
}

bool
FirmwareUpdateAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;

    memcpy(buffer, (void*)address, length);

    return success;
}

void
FirmwareUpdateAdapter_finish(void) {

    HAL_FLASH_Unlock();

    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, MAGIC_KEY_ADDRESS, ENTER_MAGIC_KEY);

    if (status == HAL_OK) {

        HAL_NVIC_SystemReset();
    }
}

void
FirmwareUpdateAdapter_ledToggle(void) {
    static GPIO_PinState pinSet = GPIO_PIN_RESET;

    pinSet ^= 1;

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, pinSet);
}

#endif
