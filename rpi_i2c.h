/* -------------------------------------------------------------------------------------------------- */
/* The LongMynd receiver: pri_i2c.h                                                                   */
/* Copyright 2019 Heather Lomond                                                                      */
/* Copyright 2020 Janis YL3AKC                                                                        */
/* -------------------------------------------------------------------------------------------------- */
/*
    This file is part of longmynd.

    Longmynd is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Longmynd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with longmynd.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RPI_I2C_TYPE_H
#define RPI_I2C_TYPE_H

#include <stdint.h>
#include <stdbool.h>


uint8_t rpi_i2c_init ();

uint8_t rpi_i2c_read_reg16 (uint8_t, uint16_t, uint8_t*);
uint8_t rpi_i2c_read_reg8  (uint8_t, uint8_t,  uint8_t*);
uint8_t rpi_i2c_write_reg16(uint8_t, uint16_t, uint8_t );
uint8_t rpi_i2c_write_reg8 (uint8_t, uint8_t,  uint8_t );

#endif
