#ifndef MTU_C5V_H
#define MTU_C5V_H
#include <machine.h>
#include <platform.h>

#define PCLK_DIVV 64

void MTU5W_Start();

void MTU5W_Stop();

void MTU5W_Setup();

void MTU5W_SetTimerCounter(uint16_t count);

uint16_t MTU5W_GetTimerCounter();
#endif // MTU_C5V_H
