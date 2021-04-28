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

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h>

uint32_t Utils_StringToInt(uint8_t* buf, uint32_t lenght);
void Utils_Serialize32BE(uint8_t* buf, uint32_t value);
void Utils_Serialize24BE(uint8_t* buf, uint32_t value);
void Utils_Serialize16BE(uint8_t* buf, uint16_t value);
void Utils_Serialize8BE(uint8_t* buf, uint8_t value);
void Utils_Deserialize32BE(uint8_t* buf, uint32_t* value);
void Utils_Deserialize16BE(uint8_t* buf, uint16_t* value);
void Utils_Deserialize8BE(uint8_t* buf, uint8_t* value);


#endif /* INC_UTILS_H_ */
