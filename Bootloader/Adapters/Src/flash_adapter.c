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

#include "main.h"
#include <string.h>
#include "flash_adapter.h"

#ifdef STM32L4xx
static uint32_t type_program = FLASH_TYPEPROGRAM_DOUBLEWORD;
#elif defined(STM32H7xx)
#define FLASH_WORD_SIZE     (32U)               //!< 32 bytes (256 bits)
static uint32_t type_program = FLASH_TYPEPROGRAM_FLASHWORD;
#elif defined(STM32F7xx)
#define FLASH_SIZE_1_MB     (0x100000U)         //!< 1 MB flash size
#define FLASH_SIZE_2_MB     (0x200000U)         //!< 2 MB flash size
#define KB_TO_B             (1024U)             //!< 1 kB = 1024 B
#define BANK_1_START        (0x08000000U)       //!< Bank 1 start address
#define BANK_2_START_2MB    (0x08100000U)       //!< Bank 2 start address -> 2 MB flash size
#define BANK_2_START_1MB    (0x08080000U)       //!< Bank 2 start address -> 1 MB flash size
#define MAX_NUM_SECT_2MB    (12U)               //!< Maximum number of sectors per bank (2 MB flash size)
#define MAX_NUM_SECT_1MB    (8U)                //!< Maximum number of sectors per bank (1 MB flash size)
#define FLASH_WORD_SIZE     (4U)                //!< Flash word size in bytes
#endif

HAL_StatusTypeDef ActivateProtection(FLASH_OBProgramInitTypeDef* ob_struct, uint32_t protect_address_start, uint32_t protect_address_end);

#ifdef EXTERNAL_FLASH
bool
FlashAdapter_erase(uint32_t firmware_size, uint32_t flash_address) {
    return W25q_dynamicErase(firmware_size, flash_address);
}

bool
FlashAdapter_blockErase(uint32_t address) {
    return W25q_blockErase64k(address);
}

bool
FlashAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    return W25q_quadPageProgram(address, buffer, length);
}

bool
FlashAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length) {
    return W25q_readBytes(address, buffer, length);
}

bool
FlashAdapter_finish(void) {
    return true;
}

#else
bool
FlashAdapter_erase(uint32_t firmware_size, uint32_t flash_address) {
    bool success = false;

    if (firmware_size <= FLASH_SIZE) {
        HAL_FLASH_Unlock();

        HAL_StatusTypeDef      status = HAL_OK;
        FLASH_EraseInitTypeDef pEraseInit;
        uint32_t               erase_error = 0U;

#ifdef STM32L4xx
        /* Get the number of PAGES to erase */
        uint32_t number_of_pages = firmware_size / FLASH_PAGE_SIZE;
        if ((firmware_size % FLASH_PAGE_SIZE) != 0U) {
            number_of_pages += 1U;
        }
        uint32_t start_page = (flash_address - FLASH_BASE) / FLASH_PAGE_SIZE;

        pEraseInit.Banks     = FLASH_BANK_1;
        pEraseInit.NbPages   = number_of_pages + 1U;
        pEraseInit.Page      = start_page;
        pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

        success = true;

#elif defined(STM32H7xx)

        /* Get the number of SECTORS to erase */
        uint32_t number_of_sectors = firmware_size / FLASH_SECTOR_SIZE;
        if ((number_of_sectors % FLASH_SECTOR_SIZE) != 0U) {
            number_of_sectors += 1U;
        } else {
            number_of_sectors = 1U;
        }

        uint32_t start_sector = (flash_address - FLASH_BASE) / FLASH_SECTOR_SIZE;

        pEraseInit.Banks        = FLASH_BANK_1;
        pEraseInit.Sector       = start_sector;
        pEraseInit.NbSectors    = number_of_sectors;
        pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
        pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

        success = true;

#elif defined(STM32F7xx)

        bool is_start_sector_in_first_bank = false;
        bool is_start_sector_in_second_bank = false;
        bool is_dual_bank;
        bool is_good_flash_size = true;
        bool number_of_sectors_found = false;
        uint32_t temp_size = 0U;
        uint32_t start_sector = 0U;
        uint32_t number_of_sectors = 0U;
        uint32_t max_num_of_sectors = 0U;
        uint32_t bank_2_start_address = 0U;

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
                    }

                    if (flash_address == (bank_2_start_address + temp_size)) {
                        start_sector = i;
                        pEraseInit.Banks = FLASH_BANK_2;
                        is_start_sector_in_second_bank = true;
                    }

                    if ((is_start_sector_in_first_bank) || (is_start_sector_in_second_bank)) {
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
#else
        if (flash_address == 0U) {
            // MISRA
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
FlashAdapter_blockErase(uint32_t address) {
    bool success = true;
    if (address == 0U) {
        // MISRA
    }
    return success;
}

#ifdef STM32L4xx
bool
FlashAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint64_t data;

    if (length % sizeof(uint64_t) == 0U ) {
        for (uint32_t i = 0U; i < (length / sizeof(uint64_t)); ++i) {
            uint32_t memory_index = i * sizeof(uint64_t);
            (void*)memcpy((void*)&data, (void*)&buffer[memory_index], sizeof(uint64_t));
            HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, data);

            if (status != HAL_OK) {
                success = false;
                break;
            }
        }
    } else if (length < sizeof(uint64_t)) {
        (void*)memcpy((void*)&data, (void*)buffer, length);
        HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address, data);
        if (status != HAL_OK) {
            success = false;
        }
    } else {
        // MISRA
    }

    return success;
}

#elif defined(STM32H7xx)
bool
FlashAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint32_t memory_index = 0U;
    uint32_t length_program = length;

    if ((length_program / FLASH_WORD_SIZE) != 0U ) {

        for (uint32_t i = 0U; i < (length_program / FLASH_WORD_SIZE); ++i) {
            memory_index = i * FLASH_WORD_SIZE;
            // cppcheck-suppress misra-c2012-11.4; function expects address of data as uint32_t
            HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, (uint32_t)&buffer[memory_index]);

            if (status != HAL_OK) {
                success = false;
                break;
            }
        }

        length_program = length_program % FLASH_WORD_SIZE;
        if (length_program > 0U) {
            memory_index += FLASH_WORD_SIZE;
        }
    }

    if ((length_program != 0U) && (length_program < FLASH_WORD_SIZE)) {

        uint8_t data[FLASH_WORD_SIZE];
        (void*)memset((void*)data, 0xFF, FLASH_WORD_SIZE);
        (void*)memcpy((void*)data, (void*)&buffer[memory_index], length_program);
        // cppcheck-suppress misra-c2012-11.4; function expects address of data as uint32_t
        HAL_StatusTypeDef status = HAL_FLASH_Program(type_program, address + memory_index, (uint32_t)data);
        if (status != HAL_OK) {
            success = false;
        }
    }

    return success;
}

#elif defined(STM32F7xx)
bool
FlashAdapter_program(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    uint32_t memory_index = 0U;
    uint32_t data;
    uint32_t length_program = length;

    if ((length_program / FLASH_WORD_SIZE) != 0U) {

        for (uint32_t i = 0U; i < (length_program / FLASH_WORD_SIZE); ++i) {

            memory_index = i * FLASH_WORD_SIZE;
            (void*)memcpy((void*)&data, (void*)&buffer[memory_index], sizeof(data));

            HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + memory_index, (uint64_t) data);

            if (status != HAL_OK) {
                success = false;
                break;
            }
        }

        length_program = length_program % FLASH_WORD_SIZE;

        if (length_program > 0U) {
            memory_index += FLASH_WORD_SIZE;
        }
    }

    if ((length_program != 0U) && (success)) {

        for (uint32_t i = 0U; i < length_program; ++i) {

            HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address + memory_index + i, (uint64_t) buffer[memory_index + i]);

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
FlashAdapter_readBytes(uint32_t address, uint8_t* buffer, uint32_t length) {
    bool success = true;
    // cppcheck-suppress misra-c2012-11.6; address is received as uint32_t
    (void*)memcpy((void*)buffer, (void*)address, length);

    return success;
}

bool
FlashAdapter_finish(void) {

    bool success = false;
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status;

#ifdef STM32L4xx
    status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS_FLASH, MAGIC_KEY_VALUE);
#elif defined(STM32H7xx)
    uint8_t data[32];
    uint64_t magic_key_value = MAGIC_KEY_VALUE;
    (void*)memcpy((void*)data, (void*)&magic_key_value, sizeof(uint64_t));
    // cppcheck-suppress misra-c2012-11.4; function expects address of data as uint32_t
    status = HAL_FLASH_Program(type_program, MAGIC_KEY_ADDRESS_FLASH, (uint32_t)data);
#elif defined(STM32F7xx)
    uint32_t least_significant_data = (uint32_t) MAGIC_KEY_VALUE;
    uint32_t most_significant_data = (uint32_t) (MAGIC_KEY_VALUE >> 32U);

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, MAGIC_KEY_ADDRESS_FLASH, (uint64_t) least_significant_data);

    if (status == HAL_OK) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, MAGIC_KEY_ADDRESS_FLASH + 4U, (uint64_t) most_significant_data);
    }
#else
    status = HAL_ERROR;
#endif

    if (status == HAL_OK) {
        success = true;
    }

    return success;
}

bool
FlashAdapter_isFlashRDPProtected(void) {
    bool success = false;

    FLASH_OBProgramInitTypeDef ob_sturct = {0U};

    HAL_FLASHEx_OBGetConfig(&ob_sturct);

    if (ob_sturct.RDPLevel == OB_RDP_LEVEL_1) {
        success = true;
    }

    return success;
}

bool
FlashAdapter_isFlashPCROPProtected(void) {
    bool success = false;

#ifdef STM32H7xx
    FLASH_OBProgramInitTypeDef ob_struct = {0U};
    ob_struct.Banks = FLASH_BANK_1;

    HAL_FLASHEx_OBGetConfig(&ob_struct);

    if (ob_struct.PCROPStartAddr <= ob_struct.PCROPEndAddr) {
        success = true;
    }
#endif // STM32H7xx

    return success;
}

bool
FlashAdapter_setReadProtection(bool enable) {

    bool success = false;

#ifdef STM32H7xx

    FLASH_OBProgramInitTypeDef ob_sturct = {0U};
    HAL_StatusTypeDef status = HAL_ERROR;

    HAL_FLASHEx_OBGetConfig(&ob_sturct);

    if ((enable) && (ob_sturct.RDPLevel == OB_RDP_LEVEL_0)) {
        ob_sturct.RDPLevel = OB_RDP_LEVEL_1;
        status = ActivateProtection(&ob_sturct, 0U, 0U);

    } else {
        ob_sturct.RDPLevel = OB_RDP_LEVEL_0;
        status = ActivateProtection(&ob_sturct, 0U, 0U);
    }

    if (status == HAL_OK) {
        success = true;
    }
#else
    if (enable) {
        // MISRA
    }
#endif

    return success;
}

bool
FlashAdapter_setPCROP(bool enable, uint32_t protect_address_start, uint32_t protect_address_end) {

    bool success = false;

#ifdef STM32H7xx

    FLASH_OBProgramInitTypeDef ob_sturct = {0};
    HAL_StatusTypeDef status = HAL_ERROR;

    HAL_FLASHEx_OBGetConfig(&ob_sturct);

    if ((!enable) && (ob_sturct.RDPLevel == OB_RDP_LEVEL_0)) {
        ob_sturct.RDPLevel = OB_RDP_LEVEL_1;
        ob_sturct.OptionType = OPTIONBYTE_RDP;
        status = ActivateProtection(&ob_sturct, 0U, 0U);
        ob_sturct.RDPLevel = OB_RDP_LEVEL_0;
        ob_sturct.OptionType = OPTIONBYTE_RDP | OPTIONBYTE_PCROP;
        status |= ActivateProtection(&ob_sturct, 0x0803FFFFUL, 0x08020000UL);

    } else {
        ob_sturct.RDPLevel = OB_RDP_LEVEL_0;
        ob_sturct.OptionType = OPTIONBYTE_RDP | OPTIONBYTE_PCROP;
        status = ActivateProtection(&ob_sturct, 0x0803FFFFUL, 0x08020000UL);
    }
#endif

    if ((enable) && (protect_address_start == 0U) && (protect_address_end == 0U)) {
        // MISRA
    }

    return success;
}

HAL_StatusTypeDef
ActivateProtection(FLASH_OBProgramInitTypeDef* ob_struct, uint32_t protect_address_start, uint32_t protect_address_end) {

    HAL_StatusTypeDef status = HAL_ERROR;

#ifdef STM32H7xx

    /* Bank 1 */
    ob_struct->Banks    = FLASH_BANK_1;
    ob_struct->PCROPConfig = OB_PCROP_RDP_ERASE;
    ob_struct->PCROPStartAddr = protect_address_start;
    ob_struct->PCROPEndAddr   = protect_address_end;

    status = HAL_FLASH_Unlock();
    status |= HAL_FLASH_OB_Unlock();

    if (status == HAL_OK) {
        status = HAL_FLASHEx_OBProgram(ob_struct);
    }

    if (status == HAL_OK) {
        status = HAL_FLASH_OB_Launch();
    }

    if (status == HAL_OK) {
        HAL_FLASH_OB_Lock();
    }
#else
    if ((ob_struct == NULL) && (protect_address_start == 0U) && (protect_address_end == 0U)) {
        // MISRA
    }
#endif

    return status;
}

#endif // EXTERNAL_FLASH
