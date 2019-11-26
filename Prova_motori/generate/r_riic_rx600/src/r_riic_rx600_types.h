/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : r_riic_rx600_types.h
* Version      : 1.00  
* Device(s)    : Renesas RX600 family  
* Description  : Type definitions for the RIIC driver API.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 16.01.2012 1.00    First Release
*******************************************************************************/

#ifndef RIIC_RX600_TYPES_H /* Multiple inclusion protection. */
#define RIIC_RX600_TYPES_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>

/******************************************************************************
Typedef definitions
*******************************************************************************/

/* Enumeration of various RIIC operational modes */ 
typedef enum
{
    RIIC_IDLE_MODE = 0,
    MASTER_TRANSMIT_MODE,
    MASTER_RECEIVE_MODE,
    SLAVE_TRANSMIT_MODE,
    SLAVE_RECEIVE_MODE,
    MODE_ERROR    
}i2c_mode_t;

/* Enumeration of various RIIC operational states that the application may read */ 
typedef enum
{
    UNKNOWN_STATE = 0,
    START_STATE,    
    STOP_STATE,
    NACK_STATE,
    RECEIVING_STATE,
    TRANSMITTING_STATE,
    ERROR_STATE     
}i2c_state_t;


/* Define return codes. */
typedef enum        
{
    RIIC_OK         = 0x0000,
    RIIC_LOCKED     = 0x0001,
    RIIC_NO_CHANNEL = 0x0002,
    RIIC_BUSY_TMO   = 0x0004,
    RIIC_TDRE_TMO   = 0x0008,
    RIIC_TEND_TMO   = 0x0010,
    RIIC_START_TMO  = 0x0020,  
    RIIC_STOP_TMO   = 0x0040,
    RIIC_RDRF_TMO   = 0x0080,
    RIIC_NACK_ERR   = 0x0100,
    RIIC_VERIFY_ERR = 0x0200,
    RIIC_MODE_ERR   = 0x0400,
    RIIC_RESET_ERR  = 0x0800, //error reset IMU
	RIIC_WRITE_ERR  = 0x1000, //error write IMU
	RIIC_READ_ERR   = 0x2000, //error read IMU     
	RIIC_AXIS_ERR   = 0x3000, //ERROR READ X,Y,Z AXIS (ACCELEROMETER BMA180)
    
}riic_ret_t;


typedef enum        
{
    RIIC_MASTER_CONFIG,
    RIIC_SLAVE_CONFIG              
}riic_config_mode_t;


/* Definition of the RIIC configuration settings structure */
typedef struct
{
    uint8_t riic_channel;             /* The RIIC channel selection index */
    riic_config_mode_t configuration; /* To select master or slave mode initialization */
    uint8_t * receive_queue_ptr;   /* Pre-allocated memory to be used as a receive queue. Set to 0 to use defaults. */
    uint32_t  receive_queue_size;  /* Size of the receive queue in bytes. */     
    uint8_t * transmit_queue_ptr;  /* Pre-allocated memory to be used as a transmit queue. Set to 0 to use defaults. */
    uint32_t  transmit_queue_size; /* Size of the transmit queue in bytes. */    
    uint8_t self_slave_addr_lo;    /* The low byte of address assigned to this RIIC channel. */
    uint8_t self_slave_addr_hi;    /* The high byte of address assigned to this RIIC channel. */
} riic_config_t;

#endif /* RIIC_RX600_TYPES_H */