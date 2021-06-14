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
static uint32_t type_program = FLASH_TYPEPROGRAM_DOUBLEWORD;
#elif STM32H7xx
#define FLASH_SECTOR_SIZE  0x00020000UL        /* 128 KB */
static uint32_t type_program = FLASH_TYPEPROGRAM_FLASHWORD;
#elif STM32F7xx
#define FLASH_SIZE_1_MB     (0x100000U)         //!< 1 MB flash size
#define FLASH_SIZE_2_MB     (0x200000U)         //!< 2 MB flash size
#define KB_TO_B             (1024U)             //!< 1 kB = 1024 B
#define BANK_1_START        (0x08000000U)       //!< Bank 1 start address
#define BANK_2_START_2MB    (0x08100000U)       //!< Bank 2 start address -> 2 MB flash size
#define BANK_2_START_1MB    (0x08080000U)       //!< Bank 2 start address -> 1 MB flash size
#define MAX_NUM_SECT_2MB    (12U)               //!< Maximum number of sectors per bank (2 MB flash size)
#define MAX_NUM_SECT_1MB    (8U)                //!< Maximum number of sectors per bank (1 MB flash size)
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
        uint32_t               erase_error = 0U;

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

        success = true;

#elif STM32H7xx

        /* Get the number of SECTORS to erase */
        uint32_t number_of_sectors = firmware_size / FLASH_SECTOR_SIZE;
        if ((number_of_sectors % FLASH_SECTOR_SIZE) != 0) {
            number_of_sectors += 1;
        } else {
            number_of_sectors = 1;
        }

        uint32_t start_sector = (flash_address - FLASH_BASE) / FLASH_SECTOR_SIZE;

        pEraseInit.Sector       = start_sector;
        pEraseInit.NbSectors    = number_of_sectors;
        pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
        pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

        success = true;

#elif STM32F7xx

        bool is_start_sector_in_first_bank = false;
        bool is_start_sector_in_second_bank = false;
        bool is_dual_bank;
        bool is_good_flash_size = true;
        bool number_of_sectors_found = false;
        uint32_t temp_size = 0U;
        uint32_t start_sector = 0U;
        uint32_t number_of_sectors = 0U;
        uint32_t max_num_of_sectors;
        uint32_t bank_2_start_address;

        // Sector sizes when 2 banks are used, in case of 1 bank, sizes have to be doubled
        const uint32_t sector_sizes_kb[MAX_NUM_SECT_2MB] = { 16U, 16U, 16U, 16U,
                                                             64U, 128U, 128U, 128U,
                                                             128U, 128U, 128U, 128U
                                                           };

#ifdef FLASH_OPTCR_nDBANK
        is_dual_bank = false;
#else
        is_dual_bank = true;
#endif

#if defined (FLASH_SIZE) && (FLASH_SIZE == FLASH_SIZE_1_MB)
        max_num_of_sectors = MAX_NUM_SECT_1_MB;
        bank_2_start_address = BANK_2_START_1MB;
#elif defined (FLASH_SIZE) && (FLASH_SIZE == FLASH_SIZE_2_MB)
        max_num_of_sectors = MAX_NUM_SECT_2MB;
        bank_2_start_address = BANK_2_START_2MB;
#else
        is_good_flash_size = false;
#endif

        if (is_good_flash_size) {

            if (is_dual_bank) {

                // Find start sector
                for (uint32_t i = 0U; i < max_num_of_sectors; ++i) {

                    if (flash_address == (BANK_1_START + temp_size)) {
                        start_sector = i;
                        pEraseInit.Banks = FLASH_BANK_1;
                        is_start_sector_in_first_bank = true;
                        break;
                    }

                    if (flash_address == (bank_2_start_address + temp_size)) {
                        start_sector = i;
                        pEraseInit.Banks = FLASH_BANK_2;
                        is_start_sector_in_second_bank = true;
                        break;
                    }

                    temp_size = temp_size + (sector_sizes_kb[i] * KB_TO_B);
                }

                temp_size = 0U;

                if ((is_start_sector_in_first_bank) || (is_start_sector_in_second_bank)) {

                    // Find number of sectors
                    for (uint32_t i = start_sector; i < max_num_of_sectors; ++i) {

                        if (firmware_size > temp_size) {

                            ++number_of_sectors;

                        } else {
                            number_of_sectors_found = true;
                            success = true;
                            break;
                        }

                        temp_size = temp_size + (sector_sizes_kb[i] * KB_TO_B);
                    }

                    if (!number_of_sectors_found) {
                        // There is overlay between bank 1 and bank 2
                        // or firmware does not fit in flash memory
                        if (is_start_sector_in_first_bank) {
                            pEraseInit.Sector       = start_sector;
                            pEraseInit.NbSectors    = number_of_sectors;
                            pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
                            pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

                            status = HAL_FLASHEx_Erase(&pEraseInit, &erase_error);

                            if (status == HAL_OK) {

                                start_sector = 0U;
                                number_of_sectors = 0U;
                                uint32_t rest_firmware_size = firmware_size - temp_size;
                                temp_size = 0U;

                                for (uint32_t i = start_sector; i < max_num_of_sectors; ++i) {

                                    if (rest_firmware_size > temp_size) {

                                        ++number_of_sectors;

                                    } else {
                                        number_of_sectors_found = true;
                                        pEraseInit.Banks = FLASH_BANK_2;
                                        success = true;
                                        break;
                                    }

                                    temp_size = temp_size + (sector_sizes_kb[i] * KB_TO_B);
                                }

                            }

                        }
                    }
                }

            } else {
                // Find start sector
                for (uint32_t i = 0U; i < max_num_of_sectors; ++i) {

                    if (flash_address == (BANK_1_START + temp_size)) {
                        start_sector = i;
                        is_start_sector_in_first_bank = true;
                        break;
                    }

                    temp_size = temp_size + (2U * sector_sizes_kb[i] * KB_TO_B);
                }

                temp_size = 0U;

                if (is_start_sector_in_first_bank) {

                    // Find number of sectors
                    for (uint32_t i = start_sector; i < max_num_of_sectors; ++i) {

                        if (firmware_size > temp_size) {

                            ++number_of_sectors;

                        } else {
                            success = true;
                            break;
                        }

                        temp_size = temp_size + (2U * sector_sizes_kb[i] * KB_TO_B);
                    }
                }
            }
        }

        if (success) {
            pEraseInit.Sector       = start_sector;
            pEraseInit.NbSectors    = number_of_sectors;
            pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
            pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        }
#endif

        if (success) {
            status = HAL_FLASHEx_Erase(&pEraseInit, &erase_error);

            if (status == HAL_OK) {
                success = true;
            } else {
                success = false;
            }
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

#elif STM32F7xx
bool
FirmwareUpdateAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;

    if (length != 0U) {

        for (uint32_t i = 0U; i < length; ++i) {
            HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address + i, (uint64_t) buffer[i]);

            if (status != HAL_OK) {
                success = false;
                break;
            }
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
    HAL_StatusTypeDef status;

#ifdef STM32L4xx
    status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS, MAGIC_KEY_VALUE);
#elif STM32H7xx
    uint8_t data[32] = {0};
    uint64_t magic_key_value = MAGIC_KEY_VALUE;
    memcpy((void*)data, (void*)&magic_key_value, sizeof(uint64_t));
    status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS, (uint32_t)data);
#elif STM32F7xx
    uint32_t least_significant_data = (uint32_t) MAGIC_KEY_VALUE;
    uint32_t most_significant_data = (uint32_t) (MAGIC_KEY_VALUE >> 32U);

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, MAGIC_KEY_ADDRESS, (uint64_t) least_significant_data);

    if (status == HAL_OK) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, MAGIC_KEY_ADDRESS + 4U, (uint64_t) most_significant_data);
    }
#else
    status = HAL_ERROR;
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
