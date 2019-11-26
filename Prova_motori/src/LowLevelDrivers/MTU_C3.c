/*
 * MTU_C3.c
 *
 *  Created on: 07/dic/2015
 *      Author: Enrico Gambini
 */

#include "MTU_C3.h"

void Set_MTU_U0_C3(void) {

	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	//Turn on MTU3 module
	MSTP(MTU3) = 0;

	//Stop first
	HaltCount_MTU_U0_C3();
	//Set prescaler to CLK/16, count at rising edge and Counter clear at TGRA compare match
	MTU3.TCR.BYTE = 0x22;
	//Set operation mode to PWM mode 1
	MTU3.TMDR.BIT.MD = 0x02;
	//Enable output on pin MTIOC3B
	MTU.TOER.BIT.OE3B = 1;
	//Enable output on pin MTIOC3D
	MTU.TOER.BIT.OE3D = 1;
	//Output goes high when TGRA compare match and goes low when TGRB compare match
	MTU3.TIORH.BYTE = 0x52;
	//Output goes high when TGRC compare match and goes low when TGRD compare match
	MTU3.TIORL.BYTE = 0x52;
	//Set TGRA and TGRC value
	MTU3.TGRA = MTU_C3_TGR_PERIOD_VALUE;
	MTU3.TGRC = MTU_C3_TGR_PERIOD_VALUE;
	//Set TGRB and TGRD
	MTU3.TGRB = MTU_C3_DEFAULT_TGR_DUTY_VALUE;
	MTU3.TGRD = MTU_C3_DEFAULT_TGR_DUTY_VALUE;

	PORTC.PMR.BIT.B1 = 1;
	PORTC.PMR.BIT.B0 = 1;

	MPC.PC1PFS.BYTE = 0x01;
	MPC.PC0PFS.BYTE = 0x01;

	PORTC.PDR.BIT.B1 = 1;
	PORTC.PDR.BIT.B0 = 1;
}

void StartCount_MTU_U0_C3(void) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU.TSTR.BIT.CST3 = 1;
}

void HaltCount_MTU_U0_C3(void) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU.TSTR.BIT.CST3 = 0;
}

uint16_t GetTGR_A_MTU_U0_C3() {
	return MTU3.TGRA;
}

void SetTGR_A_MTU_U0_C3(uint16_t tgr_a_val) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU3.TGRA = tgr_a_val;
}
void SetTGR_B_MTU_U0_C3(uint16_t tgr_b_val) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU3.TGRB = tgr_b_val;
}
void SetTGR_C_MTU_U0_C3(uint16_t tgr_c_val) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU3.TGRC = tgr_c_val;
}
void SetTGR_D_MTU_U0_C3(uint16_t tgr_d_val) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU3.TGRD = tgr_d_val;
}
