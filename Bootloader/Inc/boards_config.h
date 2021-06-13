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

#ifndef BOOTLOADER_INC_BOARDS_CONFIG_H_
#define BOOTLOADER_INC_BOARDS_CONFIG_H_

/* LED configuration for board available on the market */

#ifdef MATEK_H743_SLIM
#define LED1_Pin GPIO_PIN_4  // Green
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_3  // Blue
#define LED2_GPIO_Port GPIOE
#define LED_ON    GPIO_PIN_RESET
#define LED_OFF   GPIO_PIN_SET
#elif PIXHAWK4
#define LED1_Pin GPIO_PIN_6     // Green
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_7     // Blue
#define LED2_GPIO_Port GPIOC
#define LED_ON    GPIO_PIN_RESET
#define LED_OFF   GPIO_PIN_SET
#else
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED_ON    GPIO_PIN_SET
#define LED_OFF   GPIO_PIN_RESET

#endif


#endif /* BOOTLOADER_INC_BOARDS_CONFIG_H_ */
