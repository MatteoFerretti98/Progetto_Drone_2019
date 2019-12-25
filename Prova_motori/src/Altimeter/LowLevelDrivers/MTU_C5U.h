/******************************************************************************
 *Author: Matteo Gianduiotto
 ******************************************************************************/
/*******************************************************************************
* File Name    : MTU_C5.h
* Version      : 1.00
* Description  : Header for the MTUC5.c class
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 16.02.2016     1.00        First release
*******************************************************************************/
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
