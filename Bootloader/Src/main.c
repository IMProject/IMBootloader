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

/* Includes ------------------------------------------------------------------*/
#include <firmware_update.h>
#include <stdbool.h>
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

typedef  void (*pFunction)(void);
pFunction JumpToApplication;

int
main(void) {
    HAL_Init();
    FirmwareUpdateAdapter_SystemClockConfig();
    FirmwareUpdateAdapter_InitGPIO();
    MX_USB_DEVICE_Init();

    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

    if (*(uint64_t*)MAGIC_KEY_ADDRESS != MAGIC_KEY_VALUE) {

        while (FirmwareUpdate_isFlashing(0)) {
            //wait here until flashing is finished
        }

        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_NVIC_SystemReset();
    }

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    SCB->VTOR = FLASH_FIRMWARE_ADDRESS;

    JumpToApplication = (pFunction) (*(__IO uint32_t*) (FLASH_FIRMWARE_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t*) FLASH_FIRMWARE_ADDRESS);
    JumpToApplication();

    return -1; //error
}

void
Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}
