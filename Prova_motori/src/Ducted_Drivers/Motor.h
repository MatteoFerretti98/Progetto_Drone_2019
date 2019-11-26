/*************************************************
 * File Name  : Motor.h
 * Version    : 2.0
 *  Created on: 06/dec/2018
 *      Author: Francesco Gaudeni
 *************************************************/
#ifndef DUCTED_DRIVERS_MOTOR_H_
#define DUCTED_DRIVERS_MOTOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include "support_functions.h"
#include "../LowLevelDrivers/MTU_C3.h"

/*  -----------------------------------------------------------------------------------------------------
 * Description:	2 channels motor driver.
 * 				Default used timer channel is MTU3 with a PWM period of 20ms
 * 				This driver uses an automatic generated (by Renesas PDG2-"Peripheral Driver Generator 2")
 * 					low-level driver to interact with board peripherals
 *
 * Usage: 	-Motors are assigned to the port PC1 (channel 1) and PC0 (channel 2)
 * 				respectively on board pin JN2-9 and JN1-23
 * 			-Relay port is PD0 on pin JN2-13 used for both motors
 *  ----------------------------------------------------------------------------------------------------- */

#define MOTOR_UPPER 1	//TODO: it could be moved on the upper-level motor control driver
#define MOTOR_BOTTOM 2	//TODO: it could be moved on the upper-level motor control driver
#define MOTOR_PWM_SIGNAL_PERIOD_UP 20000.00	//20ms
#define MOTOR_ARM_UP 950	//TODO: check if valid
#define MOTOR_MIN_UP 1200	//TODO: check if valid
#define MOTOR_MAX_UP 2000	//TODO: check if valid

void Motors_Init();
void Motors_On();
void Motor_Arm(int channel);
void Motor_Write_PWM(int channel, float value);
void Motor_Write_up(int channel, float us);
void Motors_Off();

#endif /* DUCTED_DRIVERS_MOTOR_H_ */
