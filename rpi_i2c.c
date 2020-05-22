/* -------------------------------------------------------------------------------------------------- */
/* The LongMynd receiver: rpi_i2c.c                                                                   */
/*    - an implementation of the Serit NIM controlling software for the MiniTiouner Hardware          */
/*    - implements all the rpi i2c accessing routines                                                 */
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
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <errno.h>
#include "errors.h"
#include "rpi_i2c.h"

/* -------------------------------------------------------------------------------------------------- */
/* ----------------- GLOBALS ------------------------------------------------------------------------ */
/* -------------------------------------------------------------------------------------------------- */

int fd;             // RPi i2c bus file descriptor

/* -------------------------------------------------------------------------------------------------- */
uint8_t rpi_i2c_read_reg16(uint8_t addr, uint16_t reg, uint8_t *val) {
/* -------------------------------------------------------------------------------------------------- */
/* read an i2c 16 bit register from the nim                                                           */
/*   addr: the i2c buser address to access                                                            */
/*    reg: the i2c register to read                                                                   */
/*   *val: the return value for the register we have read                                             */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */
    int err=ERROR_NONE;
    char buffer[2];
    buffer[0]=reg>>8;
    buffer[1]=reg&0xff;
    if (ioctl(fd, I2C_SLAVE, (addr >> 1) ) < 0) {
        printf("ioctl error read16: %s\n", strerror(errno));
        err=-10;
    }
    if (write(fd, buffer, 2) <1) { err=-10; }
    if (read(fd, val, 1) <1) { err=-10; }

    return err;
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t rpi_i2c_write_reg16(uint8_t addr, uint16_t reg, uint8_t val) {
/* -------------------------------------------------------------------------------------------------- */
/* write an 8 bit value into a 16 bit  i2c register                                                   */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to write to                                                               */
/*    val: the value to write into the register                                                       */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err=ERROR_NONE;

    uint8_t outbuf[3];

    outbuf[0] = (reg & 0xff00) >> 8;
    outbuf[1] = reg & 0xff;
    outbuf[2] = val;

    struct i2c_msg messages[] = {
        {
            .addr = addr >> 1 ,
            .buf = outbuf,
            .len = 3,
        },
    };

    struct i2c_rdwr_ioctl_data payload = {
        .msgs = messages,
        .nmsgs = sizeof(messages) / sizeof(messages[0]),
    };

    if (ioctl(fd, I2C_RDWR, &payload) < 0) { err=-10; }

    return err;    
    
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t rpi_i2c_read_reg8(uint8_t addr, uint8_t reg, uint8_t *val) {
/* -------------------------------------------------------------------------------------------------- */
/* read an i2c 8 bit register from the nim                                                            */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to read                                                                   */
/*   *val: the return value for the register we have read                                             */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err=ERROR_NONE;
    uint8_t outbuf[1], inbuf[1];
    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data msgset[1];

    msgs[0].addr = addr>>1;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = addr>>1;
    msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
    msgs[1].len = 1;
    msgs[1].buf = inbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    outbuf[0] = reg;

    inbuf[0] = 0;

    *val = 0;
    if (ioctl(fd, I2C_RDWR, &msgset) < 0) {
        printf("ioctl(I2C_RDWR) in i2c_read\n");
        err= -10;
    }

    *val = inbuf[0];
    return err;
}

/* -------------------------------------------------------------------------------------------------- */
uint8_t rpi_i2c_write_reg8(uint8_t addr, uint8_t reg, uint8_t val) {
/* -------------------------------------------------------------------------------------------------- */
/* write an 8 bit value to an i2c 8 bit register in the nim                                           */
/*   addr: the i2c bus address to access                                                              */
/*    reg: the i2c register to write to                                                               */
/*    val: the value to write into the register                                                       */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */
    int err=ERROR_NONE;

    uint8_t outbuf[2];

    outbuf[0] = reg;
    outbuf[1] = val;

    struct i2c_msg messages[] = {
        {
            .addr = addr >> 1 ,
            .buf = outbuf,
            .len = 2,
        },
    };

    struct i2c_rdwr_ioctl_data payload = {
        .msgs = messages,
        .nmsgs = sizeof(messages) / sizeof(messages[0]),
    };

    //printf("RPI_i2c write8\n");
    if (ioctl(fd, I2C_RDWR, &payload) < 0) {
        //printf("ioctl error write16: %s\n", strerror(errno));
        err=-10;
    }

    return err;
}

uint8_t rpi_i2c_init () {
/* -------------------------------------------------------------------------------------------------- */
/* RPI I2C bus Init                                                                                   */
/* return: error code                                                                                 */
/* -------------------------------------------------------------------------------------------------- */

    int err=0;

    //FIXME: need to pass as command line parameter
    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) { err=-10; }
    // NIM reset must be here
    return err;
}
