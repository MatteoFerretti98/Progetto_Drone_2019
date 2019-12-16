/*
 * MTU_C4.h
 *
 *  Created on: 07/dic/2015
 *      Author: Enrico Gambini
 */

#ifndef SRC_LOWLEVELDRIVERS_MTU2A_MTU_C4_H_
#define SRC_LOWLEVELDRIVERS_MTU2A_MTU_C4_H_

#include <stdint.h>
#include <stdbool.h>
#include "platform.h"

#define MTU_C4_TGR_PERIOD_VALUE 60000 //20ms period
#define MTU_C4_DEFAULT_TGR_DUTY_VALUE 0 //0% duty cycle by default

void Set_MTU_U0_C4(void);
void StartCount_MTU_U0_C4(void);
void HaltCount_MTU_U0_C4(void);
uint16_t GetTGR_A_MTU_U0_C4();
void SetTGR_A_MTU_U0_C4(uint16_t tgr_a_val);
void SetTGR_B_MTU_U0_C4(uint16_t tgr_b_val);
void SetTGR_C_MTU_U0_C4(uint16_t tgr_c_val);
void SetTGR_D_MTU_U0_C4(uint16_t tgr_d_val);


#endif /* SRC_LOWLEVELDRIVERS_MTU2A_MTU_C4_H_ */
