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
* File Name		: r_riic_rx600_config.h
* Version 		: 1.00
* Description   : Developer configuration file for RX600 family RIIC driver APIs
* Device    	: RX600 family
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 13.10.2011     1           First release
*         : 18.10.2011     1.1         Added support for multiple RX600 family
*         :                            devices.
*         : 12.01.2012     1.2         Removed MCU and Package definitions
*         :                            as they are now defined elsewhere.  
*******************************************************************************/
#ifndef RIIC_CONFIG_H
#define RIIC_CONFIG_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Default queue size definitions. This space will be allocated X number of 
   RIIC channels. Minimize these if application will use its own queue buffers */
#define TX_BUF_LEN 16  // Interrupt-mode transmit queue buffer size 
#define RX_BUF_LEN 16  // Interrupt-mode receive queue buffer size

                
/* I2C transfer rate and the SCL clock duty calculated as follows:    */
/* IRC = internal reference clock = PCLK * divisor ratio.             */
/*                                                                    */
/* Transfer rate = 1 / {[(ICBRH + 1) + (ICBRL + 1)] / IRC             */  
/*                      + SCL line rising time  [tr]                  */  
/*                      + SCL line falling time [tf]}                 */
/* Duty cycle = {SCL line rising time  [tr] + (ICBRH + 1) / IRC}      */
/*            : {SCL line falling time [tf] + (ICBRL + 1) / IRC}      */
/* Note: tr and tf depend on bus capacitance and pullup resistors.    */
/* Example: set bit rate registers for ~100 kbps, ~50% duty.          */
/*     100000 = 1 / (((ICBRH + 1) + (ICBRL + 1)) / IRC) + tr + tf
       for: tr  = 1000nS  (Max I2C spec for 100kbps)
            tf  = 300nS   (Max I2C spec for 100kbps)
            IRC = 6000000 (6MHz)
       100000 = ((ICBRH + 1) + (ICBRL + 1))/6000000) + .000001 + .0000003
       50.2 = ICBRH + ICBRL
       51   =   23  +  28  round to whole numbers                    */
/* Define these as required according to above formulas */

/* Possible values for PCLK_DIVISOR:
    0: PCLK/1 
    1: PCLK/2 
    2: PCLK/4 
    3: PCLK/8 
    4: PCLK/16 
    5: PCLK/32 
    6: PCLK/64 
    7: PCLK/128 */
/* Nominal settings for 100kbps */    
#define PCLK_DIVISOR            3   /* PCLK/8 */   
#define BITRATE_HIGH_PERIOD     23
#define BITRATE_LOW_PERIOD      28

/* Defines the maximium rise time of the SCL line, in nanoseconds */
#define SCL_RISE_TIME               300
/* Defines the maximum fall time of the SCL line, in nanoseconds */
#define SCL_FALL_TIME               300


/* For operating at clock rates greater than 400kbps the fm+ (fast mode+) 
   spec must be used. There is a control bit FMPE (Fast-mode Plus Enable) 
   in the RIIC register ICFER (I2C Bus Function Enable Register) that 
   must be set to 1. Use this define to enable the FMPE bit. */
// #define USE_FASTMODE_PLUS


/* Define this to enable internal port pin pull-up resistors on RIIC bus lines. */
/* Otherwise, leave it undefined to use only external pull-up resistors.  */
// #define USE_PULLUP 


/* Slave Wait function.
   Waiting will hold the SCL line low while the receive data register (ICDRR)
   is full after each byte received. When ICDRR is read, the SCL line is released. 
   This allows the slave to perform flow control if its receive queue becomes full
   and it can't process the received data soon enough before master wants to 
   send more data.
   
   If wait function is disabled, then the RIIC will continue to receive another 
   byte in its receive shift register (ICDRS) for double-buffering continuous 
   read operation. If both the ICDRR and ICDRS bcome full then SCL will be held low 
   before ACK is sent. */

#define WAIT_OPTION_ON  // Define this to enable receive operation in byte units.


/* Define an interrupt priority. Adjust as needed */                           
#define RIIC_INT_PRIO   5

/*******************************************************************************
Exported Functions Prototypes (accessible by other modules)
*******************************************************************************/
      
#endif /* RIIC_CONFIG_H */
