/* -------------------------------------------------------------------------------------------------- */
/* The LongMynd receiver: i2c_bus                                                                     */
/*    - an implementation of the Serit NIM controlling software for the MiniTiouner Hardware          */
/*    - implements i2c switching layer for fdi and rpi i2caccessing routines                          */
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

/* -------------------------------------------------------------------------------------------------- */
/* ----------------- INCLUDES ----------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "ftdi.h"
#include "rpi_i2c.h"
#include "errors.h"
#include "i2c_bus.h"


/* -------------------------------------------------------------------------------------------------- */
uint8_t i2c_read_reg16(uint8_t addr, uint16_t reg, uint8_t *val) {
/* -------------------------------------------------------------------------------------------------- */
/* read an i2c 16 bit register from the nim                                                           */
/*   addr: the i2c buser address to access                                                            */
/*    reg: the i2c register to read                                                                   */
/*   *val: the return value for the register we have read                                             */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */
    int err = ERROR_NONE;
    if (i2c_bus_type == I2C_BUS_TYPE_RPI){
        err = rpi_i2c_read_reg16(addr, reg, val);
    }
    else {
        err=ftdi_i2c_read_reg16(addr, reg, val);
    }

    return err;
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t i2c_write_reg16(uint8_t addr, uint16_t reg, uint8_t val) {
/* -------------------------------------------------------------------------------------------------- */
/* write an 8 bit value into a 16 bit  i2c register                                                   */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to write to                                                               */
/*    val: the value to write into the register                                                       */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err = ERROR_NONE;
    if (i2c_bus_type == I2C_BUS_TYPE_RPI){
        err = rpi_i2c_write_reg16(addr, reg, val);
    } else {
        err = ftdi_i2c_write_reg16(addr, reg, val);
    }

    return err;
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t i2c_read_reg8(uint8_t addr, uint8_t reg, uint8_t *val) {
/* -------------------------------------------------------------------------------------------------- */
/* read an i2c 8 bit register from the nim                                                            */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to read                                                                   */
/*   *val: the return value for the register we have read                                             */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err = ERROR_NONE;

    if (i2c_bus_type == I2C_BUS_TYPE_RPI){
        err = rpi_i2c_read_reg8(addr, reg, val);
    } else {
        err = ftdi_i2c_read_reg8(addr, reg, val);
    }

    return err;
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t i2c_write_reg8(uint8_t addr, uint8_t reg, uint8_t val) {
/* -------------------------------------------------------------------------------------------------- */
/* write an 8 bit value to an i2c 8 bit register in the nim                                           */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to write to                                                               */
/*    val: the value to write into the register                                                       */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */
    int err = ERROR_NONE;
    if (i2c_bus_type == I2C_BUS_TYPE_RPI){
        err = rpi_i2c_write_reg8(addr, reg, val);
    } else {
        err = ftdi_i2c_write_reg8(addr, reg, val);
    }

    return err;
}

uint8_t i2c_bus_init(uint8_t usb_bus, uint8_t usb_addr) {
/* -------------------------------------------------------------------------------------------------- */
/* RPI I2C bus Init                                                                                   */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err = ERROR_NONE;
    err=ftdi_init(usb_bus, usb_addr);
    err = rpi_i2c_init();

    return err;
}
