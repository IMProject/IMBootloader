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
#include "gpio_adapter.h"
#include "flash_adapter.h"
#include "system_adapter.h"
#include "system_clock_adapter.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "binary_update.h"
#include "communication.h"
#include "security_adapter.h"
#include "signature.h"

typedef void (*pFunction)(void);

static void JumpToAddress();

int
main(void) {
    HAL_Init();
    SystemClock_Config();
    GpioAdapter_init();
    Communication_init();
    SecurityAdapter_init();
    BinaryUpdate_handleBootInfo();
    bool enter_bootloader_loop = false;

#ifdef SECURED
    if (!FlashAdapter_isFlashRDPProtected()) {

        bool success = FlashAdapter_setReadProtection(true);

        if (success) {
            success = FlashAdapter_isFlashRDPProtected();

            if (!success) {
                Error_Handler();
            }
        }
    }
#endif

#if defined(MAGIC_KEY_ADDRESS_RAM)
    // Check RAM KEY
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to get value that is stored at MAGIC_KEY_ADDRESS_RAM
    if (*(uint64_t*)MAGIC_KEY_ADDRESS_RAM == MAGIC_KEY_VALUE) {
        enter_bootloader_loop = true;
    }
#endif

#if defined(MAGIC_KEY_ADDRESS_FLASH)
    // Check FLASH KEY
    // cppcheck-suppress misra-c2012-11.4; conversion is needed to get value that is stored at MAGIC_KEY_ADDRESS_FLASH
    if (*(uint64_t*)MAGIC_KEY_ADDRESS_FLASH != MAGIC_KEY_VALUE) {
        enter_bootloader_loop = true;
    }
#endif

    // Check for skip flag
    if (BinaryUpdate_checkSkipLoopFlag()) {
        BinaryUpdate_disableLoopFlag();
        enter_bootloader_loop = false;
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

        while (Communication_mainLoop(0U)) {
            //wait here until flashing is finished
        }

        GpioAdapter_led1Off();

#if defined(STM32N657xx)
        JumpToAddress();
#else
        SystemAdapter_reset();
#endif

    }

    JumpToAddress();

    return -1; //error
}

static void
JumpToAddress() {

    pFunction JumpToApplication;
    uint32_t primask_bit;
    uint32_t application_vector;
    /* Suspend SysTick */
    HAL_SuspendTick();

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* if I-Cache is enabled, disable I-Cache-----------------------------------*/
    if (SCB->CCR & SCB_CCR_IC_Msk) {
        SCB_DisableICache();
    }
#endif /* defined(ICACHE_PRESENT) && (ICACHE_PRESENT == 1U) */

#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    /* if D-Cache is enabled, disable D-Cache-----------------------------------*/
    if (SCB->CCR & SCB_CCR_DC_Msk) {
        SCB_DisableDCache();
    }
#endif /* defined(DCACHE_PRESENT) && (DCACHE_PRESENT == 1U) */

    /* Initialize user application's Stack Pointer & Jump to user application  */
    primask_bit = __get_PRIMASK();
    __disable_irq();

    application_vector = BinaryUpdate_getJumpAddress();

    SCB->VTOR = (uint32_t)application_vector;
    JumpToApplication = (pFunction) (*(__IO uint32_t*)(application_vector + 4));

#if ((defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1)) || \
       (defined (__ARM_ARCH_8_1M_MAIN__ ) && (__ARM_ARCH_8_1M_MAIN__ == 1)) || \
       (defined (__ARM_ARCH_8M_BASE__ ) && (__ARM_ARCH_8M_BASE__ == 1))    )
    /* on ARM v8m, set MSPLIM before setting MSP to avoid unwanted stack overflow faults */
    __set_MSPLIM(0x00000000);
#endif  /* __ARM_ARCH_8M_MAIN__ or __ARM_ARCH_8M_BASE__ */

    __set_MSP(*(__IO uint32_t*)application_vector);

    /* Re-enable the interrupts */
    __set_PRIMASK(primask_bit);

    JumpToApplication();
}

void
Error_Handler(void) {
    while (true) {}
}
