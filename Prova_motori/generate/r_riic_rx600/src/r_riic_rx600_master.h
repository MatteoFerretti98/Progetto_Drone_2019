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
* File Name    : r_riic_rx600_master.h
* Version      : 1.00  
* Device(s)    : Renesas RX600 family  
* Description  : RIIC driver master mode API function declarations header file.
********************************************************************************
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.10.2011 1.00    First Release
*******************************************************************************/
#ifndef _IIC_DEF_H
#define _IIC_DEF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_riic_rx600.h"


/******************************************************************************
Functions Prototypes
*******************************************************************************/
riic_ret_t R_RIIC_MasterTransmitHead(uint8_t          channel,                                      
                                     uint8_t *        p_data_buff, 
                                     const uint32_t   num_bytes);
riic_ret_t R_RIIC_MasterReceive(uint8_t         channel,
                                uint8_t         slave_addr,
                                uint8_t *       p_data_buff,
                                const uint32_t  num_bytes);
riic_ret_t R_RIIC_MasterTransmit(uint8_t        channel,
                                 uint8_t *      p_data_buff, 
                                 const uint32_t num_bytes);
                              
#endif /* _IIC_DEF_H */


