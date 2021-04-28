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

#include <utils.h>
#include <math.h>

uint32_t
Utils_StringToInt(uint8_t* buf, uint32_t lenght) {
    uint32_t integer = 0;
    uint32_t i = 0;

    while ((buf[i] != '\0') && (lenght > i)) {
        integer += (buf[i] - '0') * pow(10, (lenght - (i + 1)));
        i++;
    }
    return integer;
}

void
Utils_Serialize32BE(uint8_t* buf, uint32_t value) {
    buf[0] = (uint8_t)(value >> 24u) & 0xFF;
    buf[1] = (uint8_t)(value >> 16u) & 0xFF;
    buf[2] = (uint8_t)(value >> 8u) & 0xFF;
    buf[3] = (uint8_t)(value >> 0u) & 0xFF;
}

void
Utils_Serialize24BE(uint8_t* buf, uint32_t value) {
    buf[0] = (uint8_t)(value >> 16u) & 0xFF;
    buf[1] = (uint8_t)(value >> 8u) & 0xFF;
    buf[2] = (uint8_t)(value >> 0u) & 0xFF;
}

void
Utils_Serialize16BE(uint8_t* buf, uint16_t value) {
    buf[0] = (uint8_t)(value >> 8u) & 0xFF;
    buf[1] = (uint8_t)(value >> 0u) & 0xFF;
}

void
Utils_Serialize8BE(uint8_t* buf, uint8_t value) {
    buf[0] = (uint8_t)(value >> 0u) & 0xFF;
}

void
Utils_Deserialize32BE(uint8_t* buf, uint32_t* value) {
    *value = (uint32_t)(buf[0] << 24u);
    *value |= (uint32_t)(buf[1] << 16u);
    *value |= (uint32_t)(buf[2] << 8u);
    *value |= (uint32_t)(buf[3] << 0u);
}

void
Utils_Deserialize16BE(uint8_t* buf, uint16_t* value) {
    *value = (uint32_t)(buf[0] << 8u);
    *value |= (uint32_t)(buf[1] << 0u);
}

void
Utils_Deserialize8BE(uint8_t* buf, uint8_t* value) {
    *value = (uint32_t)(buf[0] << 0u);
}


