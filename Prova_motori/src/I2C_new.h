#ifndef _I2C_new_H_
#define _I2C_new_H_

#include <stdint.h>
#include <string.h>
#include <machine.h>
#include "platform.h"
#include "r_riic_rx600.h"
#include "r_riic_rx600_master.h"

/*typedef union{
	uint16_t dato_16bit;
	uint8_t dato_8bit[2];
} VALUE; */

/*******************************************************************************
 * Function name: riic_master_init
 * Description  : Prepare an RIIC channel for master mode communications
 * Arguments    : none
 * Return value : int - 0 if OK
 *******************************************************************************/
int i2c_init();

/*******************************************************************************
 * Function name: i2c_write
 * Description  : Writes a specified number of bytes starting from the specfied
 *                slave register. If more than 1 byte is requested then
 *                the slave will automatically increment to the next
 *                register number with each sequential write.
 * Arguments    : riic_channel -
 *                   Which IIC channel of the MCU to use.
 *                slave_addr -
 *                   IIC slave address of the slave.
 *                register_number -
 *                   Which register of the slave to be written.
 *                (uint8_t*)source_buff -
 *                   pointer to the buffer where data will be copied from.
 *                num_bytes -
 *                   The number of bytes to be written
 *
 * Return value : int - 0 if OK
 *******************************************************************************/
int i2c_write(uint8_t slave_addr, uint16_t register_number, uint8_t num_bytes,
					 uint8_t* source_buff,uint8_t send_num_byte);
int i2c_write(uint8_t slave_addr, uint8_t register_number, uint8_t num_bytes,
					 uint8_t *source_buff);
/******************************************************************************
 * Function name: i2c_read
 * Description  : Reads a specified number of bytes starting from the specfied
 *                slave register. If more than 1 byte is requested then
 *                the slave will automatically increment to the next
 *                register number with each sequential read.
 * Arguments    : riic_channel -
 *                   Which IIC channel of the MCU to use.
 *                slave_addr -
 *                   IIC slave address of the slave.
 *                register_number -
 *                   Which register of the slave to be written.
 *                (uint8_t*)dest_buff -
 *                   pointer to the buffer into which the read data will be stored.
 *                num_bytes -
 *                   The number of bytes to be read from the slave
 *
 * Return value : int - 0 if OK
 ******************************************************************************/
int i2c_read(uint8_t slave_addr, uint16_t register_number, uint8_t num_bytes,
					uint8_t* dest_buff,uint8_t send_num_byte);
int i2c_read (uint8_t slave_addr, uint8_t register_number, uint8_t num_bytes,
					 uint8_t *dest_buff);
#endif
