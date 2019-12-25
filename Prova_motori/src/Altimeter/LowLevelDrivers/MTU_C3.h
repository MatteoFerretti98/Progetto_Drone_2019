/******************************************************************************
 *Author: Enrico Gambini
 ******************************************************************************/
/*******************************************************************************
* File Name    : MTU_C3.h
* Version      : 2.00
* Description  : Header for the MTU_C3.c class
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 07.12.2015     1.00        First release
*         : 18.12.2019	   2.00		   Update (Angelo Bonomi D'Agostino)
*******************************************************************************/
#ifndef SRC_LOWLEVELDRIVERS_MTU2A_MTU_C3_H_
#define SRC_LOWLEVELDRIVERS_MTU2A_MTU_C3_H_

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"

#define MTU_C3_TGR_PERIOD_VALUE 60000 //20ms period
#define MTU_C3_DEFAULT_TGR_DUTY_VALUE 0 //0% duty cycle by default

void Set_MTU_U0_C3(void);
void StartCount_MTU_U0_C3(void);
void HaltCount_MTU_U0_C3(void);
uint16_t GetTGR_A_MTU_U0_C3();
void SetTGR_A_MTU_U0_C3(uint16_t tgr_a_val);
void SetTGR_B_MTU_U0_C3(uint16_t tgr_b_val);
void SetTGR_C_MTU_U0_C3(uint16_t tgr_c_val);
void SetTGR_D_MTU_U0_C3(uint16_t tgr_d_val);

#endif /* SRC_LOWLEVELDRIVERS_MTU2A_MTU_C3_H_ */
