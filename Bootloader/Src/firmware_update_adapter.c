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
#include "main.h"
#include <string.h>

#ifdef STM32L4xx
uint32_t type_program = FLASH_TYPEPROGRAM_DOUBLEWORD;
#elif STM32H7xx
#define FLASH_SECTOR_SIZE  0x00020000UL        /* 128 KB */
uint32_t type_program = FLASH_TYPEPROGRAM_FLASHWORD;
#endif

void
FirmwareUpdateAdapter_InitGPIO(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, LED_OFF);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, LED_OFF);

    /*Configure GPIO pins : LED1_Pin*/
    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : LED2_Pin*/
    GPIO_InitStruct.Pin = LED2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);
}


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

#ifdef STM32L4xx
        /* Get the number of PAGES to erase */
        uint32_t number_of_pages = firmware_size / FLASH_PAGE_SIZE;
        if ((firmware_size % FLASH_PAGE_SIZE) != 0) {
            number_of_pages += 1;
        }
        uint32_t start_page = (flash_address - FLASH_BASE) / FLASH_PAGE_SIZE;

        pEraseInit.Banks     = FLASH_BANK_1;
        pEraseInit.NbPages   = number_of_pages + 1;
        pEraseInit.Page      = start_page;
        pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

#elif STM32H7xx

        /* Get the number of SECTORS to erase */
        uint32_t number_of_sectors = firmware_size / FLASH_SECTOR_SIZE;
        if ((number_of_sectors % FLASH_SECTOR_SIZE) != 0) {
            number_of_sectors += 1;
            success = true;
        } else {
            number_of_sectors = 1;
        }

        uint32_t start_sector = (flash_address - FLASH_BASE) / FLASH_SECTOR_SIZE;

        pEraseInit.Sector       = start_sector;
        pEraseInit.NbSectors    = number_of_sectors;
        pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
        pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
#endif

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

#ifdef STM32L4xx
bool
FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint64_t data = UINT64_MAX;


    if (length % sizeof(uint64_t) == 0 ) {

        for (uint32_t i = 0; i < length / sizeof(uint64_t); i++) {
            uint32_t memory_index = i * sizeof(uint64_t);
            memcpy((void*)&data, (void*)&buffer[memory_index], sizeof(uint64_t));
            HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, data);

            if (status != HAL_OK) {
                success = false;
                break;
            }
        }

    } else if (length < sizeof(uint64_t)) {

        memcpy((void*)&data, (void*)buffer, length);
        HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address, data);
        if (status != HAL_OK) {
            success = false;
        }
    }

    return success;
}

#elif STM32H7xx
bool
FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint16_t flash_word = 32; //32 bytes (256 bits)
    uint32_t memory_index = 0;

    if ((length / flash_word) != 0 ) {

        for (uint32_t i = 0; i < length / flash_word; i++) {
            memory_index = i * flash_word;
            HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, (uint32_t)&buffer[memory_index]);

            if (status != HAL_OK) {
                success = false;
                break;
            }
        }

        length = length % flash_word;
        if (length > 0) {
            memory_index += flash_word;
        }
    }

    if ((length != 0) && (length < flash_word)) {

        uint8_t data[32] = {0};
        memcpy((void*)data, (void*)&buffer[memory_index], length);
        HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, (uint32_t)data);
        if (status != HAL_OK) {
            success = false;
        }
    }

    return success;
}
#endif

bool
FirmwareUpdateAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;

    memcpy(buffer, (void*)address, length);

    return success;
}

void
FirmwareUpdateAdapter_finish(void) {

    HAL_FLASH_Unlock();

#ifdef STM32L4xx
    HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS, MAGIC_KEY_VALUE);
#elif STM32H7xx
    uint8_t data[32] = {0};
    uint64_t magic_key_value = MAGIC_KEY_VALUE;
    memcpy((void*)data, (void*)&magic_key_value, sizeof(uint64_t));
    HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS, (uint32_t)data);
#endif

    if (status == HAL_OK) {

        HAL_NVIC_SystemReset();
    }
}

void
FirmwareUpdateAdapter_ledToggle(void) {
    static GPIO_PinState pinSet = LED_ON;

    pinSet ^= 1;

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, pinSet);
}

#endif
