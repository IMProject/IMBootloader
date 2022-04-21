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

#include <stdbool.h>
#include "main.h"
#include "system_clock_adapter.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "firmware_update.h"

typedef void (*pFunction)(void);

int
main(void) {
    HAL_Init();
    SystemClock_Config();
    GpioAdapter_init();
    FirmwareUpdate_init();
    bool enter_bootloader_loop = false;
    pFunction JumpToApplication;

    // Check RAM KEY
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to get value that is stored at MAGIC_KEY_ADDRESS_RAM
    if (*(uint64_t*)MAGIC_KEY_ADDRESS_RAM == MAGIC_KEY_VALUE) {
        enter_bootloader_loop = true;
    }

    // Check FLASH KEY
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to get value that is stored at MAGIC_KEY_ADDRESS_FLASH
    if (*(uint64_t*)MAGIC_KEY_ADDRESS_FLASH != MAGIC_KEY_VALUE) {
        enter_bootloader_loop = true;
    }

#if defined(BL_BUTTON_Pin) && defined(BL_BUTTON_Port) && defined(BL_BUTTON_ON) && defined(BL_BUTTON_PRESS_TIME)
    // Check bootloader button
    uint32_t time_start = HAL_GetTick();
    while (BL_BUTTON_ON == HAL_GPIO_ReadPin(BL_BUTTON_Port, BL_BUTTON_Pin)) {
        uint32_t time_elapsed = HAL_GetTick() - time_start;
        if (time_elapsed > BL_BUTTON_PRESS_TIME) {
            enter_bootloader_loop = true;
            break;
        }
    }
#endif

    if (enter_bootloader_loop) {

        MX_USB_DEVICE_Init();
        GpioAdapter_led1On();

        while (FirmwareUpdate_bootloaderLoop(0U)) {
            //wait here until flashing is finished
        }

        GpioAdapter_led1Off();
        HAL_NVIC_SystemReset();
    }

    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    SCB->VTOR = FLASH_FIRMWARE_ADDRESS;
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to jump to the application
    JumpToApplication = (pFunction) (*(__IO uint32_t*) (FLASH_FIRMWARE_ADDRESS + 4U));
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to jump to the application
    __set_MSP(*(__IO uint32_t*) FLASH_FIRMWARE_ADDRESS);
    JumpToApplication();

    return -1; //error
}

void
Error_Handler(void) {
    while (true) {}
}
