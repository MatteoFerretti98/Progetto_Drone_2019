/******************************************************************************
 *Author: Matteo Gianduiotto
 ******************************************************************************/
/*******************************************************************************
* File Name    : MTU_C5W.c
* Version      : 1.00
* Description  : PWM modulation
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 16.02.2016     1.00        First release
*******************************************************************************/
#include "MTU_C5W.h"


void MTU5W_Start() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	// Reset counter
	MTU5.TCNTW = 0x0;
	// Start timer
	MTU5.TSTR.BIT.CSTW5 = 0x1;

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5W_Stop() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	// Stop timer
	MTU5.TSTR.BIT.CSTW5 = 0x0;
	// Reset counter
	MTU5.TCNTW = 0x0;

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5W_Setup() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	/* Setup echo read port PD5 (pin 18 on jn2) input */
	// Input data register
	PORTD.PIDR.BIT.B5 = 0x0;
	// Direction register (input)
	PORTD.PDR.BIT.B5 = 0x0;
	// Mode register (peripheral)
	PORTD.PMR.BIT.B5 = 0x1;

	// Turn on MTU5
	MSTP(MTU5) = 0;

	// Set MTU5 pre-scaler to PCLK/16
	MTU5.TCRW.BIT.TPSC = 0x02;
	// Make MTU5W count
	MTU5.TIORW.BYTE= 0x1d;
	// Enable interrupt on input detected
//	MTU5.TIER.BIT.TGIE5W = 0x1;
	// Use pin with timer
	MPC.PD5PFS.BIT.PSEL = 0x01;

	// Ensure timer is stopped
	MTU5W_Stop();

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5W_SetTimerCounter(uint16_t count) {
	MTU5.TCNTW = count;
}

uint16_t MTU5W_GetTimerCounter() {
	return MTU5.TCNTW;
}
