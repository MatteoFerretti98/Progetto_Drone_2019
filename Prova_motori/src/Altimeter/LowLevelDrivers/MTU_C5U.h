#ifndef MTU_C5U_H
#define MTU_C5U_H
#include <machine.h>
#include <platform.h>

#define PCLK_DIVU 64

void MTU5U_Start();

void MTU5U_Stop();

void MTU5U_Setup();

void MTU5U_SetTimerCounter(uint16_t count);

uint16_t MTU5U_GetTimerCounter();
#endif // MTU_C5U_H
