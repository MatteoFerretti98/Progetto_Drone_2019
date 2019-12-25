/******************************************************************************
 *Author: Matteo Gianduiotto
 ******************************************************************************/
/*******************************************************************************
* File Name    : MTU_C5W.h
* Version      : 1.00
* Description  : Header for the MTU_C5W.c class
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 16.02.2016     1.00        First release
*******************************************************************************/
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
