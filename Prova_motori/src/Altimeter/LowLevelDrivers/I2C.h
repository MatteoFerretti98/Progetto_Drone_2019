/******************************************************************************
 *Author: Alessandro De Toni
 ******************************************************************************/
/*******************************************************************************
* File Name    : I2C.h
* Version      : 2.00
* Description  : Header for the I2C.c class
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 03.02.2018     1.00        First release
*         : 18.12.2019	   2.00		   Update for both imu and altimeter
*******************************************************************************/
#ifndef I2C_H   /* Multiple inclusion prevention. */
#define I2C_H

#include "r_riic_rx600_types.h"

#define MASTER_IIC_ADDRESS_LO	0x20	/* Master address low bits [7:0] */
#define MASTER_IIC_ADDRESS_HI	0x00	/* Master address high bits [2:0] */
#define RIIC_CHANNEL            CHANNEL_0
#define RW_BIT                  0x01     /* Read / #write bit value. 1 = read. */
#define CHANNEL				0
#define NUM_BYTES			1

riic_ret_t riic_master_init(void);

#endif /* I2C */

