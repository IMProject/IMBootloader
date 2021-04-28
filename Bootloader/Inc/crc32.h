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

#ifndef CRC32_H
#define CRC32_H

#include "stdint.h"
#include "stdbool.h"

uint32_t CalculateCRC32(
    const uint8_t* crc_DataPtr,
    uint32_t crc_Length,
    uint32_t crc_InitialValue,
    bool reflectedOutput,
    bool reflectedInput
);

#endif // CRC32_H
