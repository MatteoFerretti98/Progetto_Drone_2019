/*
 * MTU_C5.c
 *
 *  Created on: 16/feb/2016
 *      Author: Matteo
 */

#include "MTU_C5U.h"


void MTU5U_Start() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	// Reset counter
	MTU5.TCNTU = 0x0;
	// Start timer
	MTU5.TSTR.BIT.CSTU5 = 0x1;

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5U_Stop() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	// Stop timer
	MTU5.TSTR.BIT.CSTU5 = 0x0;
	// Reset counter
	MTU5.TCNTU = 0x0;

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5U_Setup() {
	// Unlock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B;
#endif

	/* Setup echo read port PD7 (pin 20 on jn2) input */
	// Input data register
	PORTD.PIDR.BIT.B7 = 0x0;
	// Direction register (input)
	PORTD.PDR.BIT.B7 = 0x0;
	// Mode register (peripheral)
	PORTD.PMR.BIT.B7 = 0x1;

	// Turn on MTU5
	MSTP(MTU5) = 0;

	// Set MTU5 pre-scaler to PCLK/64
	MTU5.TCRU.BIT.TPSC = 0x03;
	// Make MTU5U count
	MTU5.TIORU.BIT.IOC = 0x1d;
	// Enable interrupt on input detected
	MTU5.TIER.BIT.TGIE5U = 0x0;
	// Use pin with timer
	MPC.PD7PFS.BIT.PSEL = 0x01;

	// Ensure timer is stopped
	MTU5U_Stop();

	// Lock ports
#ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500;
#endif
}

void MTU5U_SetTimerCounter(uint16_t count) {
	MTU5.TCNTU = count;
}

uint16_t MTU5U_GetTimerCounter() {
	return MTU5.TCNTU;
}
