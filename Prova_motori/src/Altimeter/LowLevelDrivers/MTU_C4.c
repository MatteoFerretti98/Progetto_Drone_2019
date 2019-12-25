/******************************************************************************
 *Author: Enrico Gambini
 ******************************************************************************/
/*******************************************************************************
* File Name    : MTU_C4.c
* Version      : 2.00
* Description  : PWM modulation
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 07.12.2015     1.00        First release
*         : 18.12.2019	   2.00		   Update (Angelo Bonomi D'Agostino)
*******************************************************************************/
#include "MTU_C4.h"

void Set_MTU_U0_C4(void) {

	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	//Turn on MTU4 module
	MSTP(MTU4) = 0;

	//Stop first
	HaltCount_MTU_U0_C4();
	//Set prescaler to CLK/16, count at rising edge and Counter clear at TGRA compare match
	MTU4.TCR.BYTE = 0x22;
	//Set operation mode to PWM mode 1
	MTU4.TMDR.BIT.MD = 0x02;
	//Enable output on pin MTIOC4A
	MTU.TOER.BIT.OE4A = 1;
	//Enable output on pin MTIOC4C
	MTU.TOER.BIT.OE4C = 1;
	//Output goes high when TGRA compare match and goes low when TGRB compare match
	MTU4.TIORH.BYTE = 0x52;
	//Output goes high when TGRC compare match and goes low when TGRD compare match
	MTU4.TIORL.BYTE = 0x52;
	//Set TGRA and TGRC value
	MTU4.TGRA = MTU_C4_TGR_PERIOD_VALUE;
	MTU4.TGRC = MTU_C4_TGR_PERIOD_VALUE;
	//Set TGRB and TGRD
	MTU4.TGRB = MTU_C4_DEFAULT_TGR_DUTY_VALUE;
	MTU4.TGRD = MTU_C4_DEFAULT_TGR_DUTY_VALUE;

	//setting pin 22 of JN2(PORT E bit 1)
	//setting pin 23 of JN2(PORT E bit 2)
	
	PORTE.PMR.BIT.B2 = 1;
	PORTE.PMR.BIT.B1 = 1;

	MPC.PE2PFS.BYTE = 0x01;
	MPC.PE1PFS.BYTE = 0x01;
		
	PORTE.PDR.BIT.B2 = 1;
	PORTE.PDR.BIT.B1 = 1;

}

void StartCount_MTU_U0_C4(void) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU.TSTR.BIT.CST4 = 1;
}

void HaltCount_MTU_U0_C4(void) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU.TSTR.BIT.CST4 = 0;
}

uint16_t GetTGR_A_MTU_U0_C4() {
	return MTU4.TGRA;
}

void SetTGR_A_MTU_U0_C4(uint16_t tgr_a_val4) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU4.TGRA = tgr_a_val4;
}
void SetTGR_B_MTU_U0_C4(uint16_t tgr_b_val4) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU4.TGRB = tgr_b_val4;
}
void SetTGR_C_MTU_U0_C4(uint16_t tgr_c_val4) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU4.TGRC = tgr_c_val4;
}
void SetTGR_D_MTU_U0_C4(uint16_t tgr_d_val4) {
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
	MTU4.TGRD = tgr_d_val4;
}

