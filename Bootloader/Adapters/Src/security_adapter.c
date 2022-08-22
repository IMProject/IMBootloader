/****************************************************************************
 *
 *   Copyright (c) 2022 IMProject Development Team. All rights reserved.
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

#include "security_adapter.h"
#include "main.h"

#if defined(STM32H7xx)
static RNG_HandleTypeDef hrng;
#endif

void
SecurityAdapter_init() {

#if defined(STM32H7xx)
    hrng.Instance = RNG;
    HAL_RNG_Init(&hrng);
#endif
}

bool
SecurityAdapter_getRandomData(uint8_t* data, uint32_t size) {

#if defined(STM32H7xx)
    bool success = true;
    uint32_t random_data;

    HAL_StatusTypeDef status = HAL_OK;

    for (uint32_t i = 0u; i < size; ++i) {

        if ((i % 4u) == 0u) {
            status = HAL_RNG_GenerateRandomNumber(&hrng, &random_data);
        }

        if (status != HAL_OK) {
            success = false;
            break;
        }

        data[i] = (uint8_t)(random_data >> (8u * (i % 4u)));
    }

#else
    if (data && size) {} //make MISRA happy
    bool success = false;
#endif

    return success;

}
