/******************************************************************************
* File Name: i2c.c
* Version: 1.01.1
* Description: file contenente funzioni relative all'i2c.
*******************************************************************************/

/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			22.01.2018     1.01        Revisione della versione di Sebastian Giles
* 										da parte di Omar Cocchairella.
*			10.01.2019     1.01.1      Martin Breccia commento della versione
* 										di Omar Cocchairella.
*******************************************************************************/

#include "i2c.h"

#define MASTER_IIC_ADDRESS_LO	0x20	/* Master address low bits [7:0] */
#define MASTER_IIC_ADDRESS_HI	0x00	/* Master address high bits [2:0] */
#define RIIC_CHANNEL            CHANNEL_0
#define RW_BIT                  0x01     /* Read / #write bit value. 1 = read. */

int i2c_init()
{
	/* Initialize a data structure used by the R_RIIC_Init() API function. */
	/* See r_riic_rx600.h for definitions of this structure. */
	riic_config_t   riic_master_config = {RIIC_CHANNEL, RIIC_MASTER_CONFIG,
			0,
			0,
			0,
			0,
			MASTER_IIC_ADDRESS_LO,
			MASTER_IIC_ADDRESS_HI};

	return R_RIIC_Init(&riic_master_config);
} //END - i2c_init(..)

int i2c_read (uint8_t slave_addr, uint8_t register_number, uint8_t num_bytes,
					 uint8_t *dest_buff)
{
	 /* Storage for the slave address and target register. */
	uint8_t     addr_and_register[2];
	riic_ret_t  ret = RIIC_OK;

	addr_and_register[0] = slave_addr<<1;
	addr_and_register[1] = register_number;

	ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);

	/* Now read the data from the target register into the destination buffer. */
	ret |= R_RIIC_MasterReceive(CHANNEL_0, slave_addr<<1, dest_buff, num_bytes);

	return ret;
} //END - i2c_read(..)

int i2c_write(uint8_t slave_addr, uint8_t register_number, uint8_t num_bytes,
					 uint8_t *source_buff)
{
	/* Storage for the slave address and target register. */
	uint8_t     addr_and_register[2];
	riic_ret_t  ret = RIIC_OK;

	addr_and_register[0] = slave_addr<<1;
	addr_and_register[1] = register_number;

	ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);

	/* Now write the data from the source buffer into the target register. */
	ret |= R_RIIC_MasterTransmit(CHANNEL_0, source_buff, num_bytes);

	return ret;
} //END - i2c_write(..)
