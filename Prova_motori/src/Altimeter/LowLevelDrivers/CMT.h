/*******************************************************************************
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
/******************************************************************************
* File Name     : CMT.h
* Version       : 1.0
* Description   : Header file for CMT.c
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 19.01.2012     1.00        First release
*******************************************************************************/


#ifndef _CMT_H_             /* Multiple inclusion prevention. */
#define _CMT_H_

#include <stdint.h>

struct timerClocks {
	uint8_t timer_1mS;
	uint8_t timer_5mS;
	uint8_t timer_10mS;
	uint8_t timer_20mS;
	uint8_t timer_50mS;
	uint8_t timer_100mS;
	uint8_t timer_500mS;
	uint8_t timer_1000mS;
	uint8_t timer_2000mS;
	//uint8_t timer_4000mS; //TODO maybe useful for IMU instead of 2000ms
};

/*******************************************************************************
 Prototypes for exported functions
*******************************************************************************/
void CMT_init (void) ;
void ms_delay(unsigned int ms);
unsigned long getTime();

/*******************************************************************************
 Exported variables
*******************************************************************************/


#endif                       /* Multiple inclusion prevention. */
