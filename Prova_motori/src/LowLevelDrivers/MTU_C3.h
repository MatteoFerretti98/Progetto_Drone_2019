/*
 * MTU_C3.h
 *
 *  Created on: 07/dic/2015
 *      Author: Enrico Gambini
 */

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
