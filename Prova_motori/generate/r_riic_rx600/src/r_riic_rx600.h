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
* File Name    : r_riic_rx600.h
* Version      : 1.00  
* Device(s)    : Renesas RX600 family  
* Description  : Definitions and declarations supporting the RIIC driver API.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 07.09.2011 1.00    First Release
*         : 24.10.2011 1.1     Added support for multiple MCUs, Multiple RIIC
*                              channels, and both master and slave configurations.
*         : 05.01.2012 1.2     Changed riic_ret_t enumeration values to 16-bit
*                              to permit storing combinations of errors.
*******************************************************************************/

#ifndef RIIC_RX600_H /* Multiple inclusion protection. */
#define RIIC_RX600_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"

/* User specific RIIC configuration options */

#include "r_riic_rx600_config.h"

/* Private macro definitions and function prototypes. */
#include "r_riic_rx600_private.h" 

/* Type definitions for the RIIC driver API. */ 
#include "r_riic_rx600_types.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/

/* Use only these channel definitions in application when specifying an       */ 
/* RIIC channel number: CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3            */
/* This translates physical channel to an index used by the driver            */


/*******************************************************************************
API functions Prototypes
*******************************************************************************/
riic_ret_t R_RIIC_Init(riic_config_t * settings);
riic_ret_t R_RIIC_ReleaseChannel(uint8_t channel);
riic_ret_t R_RIIC_Reset(uint8_t channel);   

#endif /* RIIC_RX600_H */



