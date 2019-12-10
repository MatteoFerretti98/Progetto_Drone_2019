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
 * Description:	4 channels motor driver.
 * 				Default used timer channel is MTU3 and MTU4 with a PWM period of 20ms
 * 				This driver uses an automatic generated (by Renesas PDG2-"Peripheral Driver Generator 2")
 * 					low-level driver to interact with board peripherals
 *
 * Usage: 	-Motors are assigned to the port PC0 (channel 1), PC1 (channel 2), PE1 (channel 3) and PE2 (channel 4)
 * 				respectively on board pin JN1-23, JN2-9, JN2 22 and JN2 23
 * 			-Relay port is PD0 on pin JN2-13 used for both motors
 *  ----------------------------------------------------------------------------------------------------- */

#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_3 3
#define MOTOR_4 4
#define MOTOR_PWM_SIGNAL_PERIOD_UP 20000.00	//20ms
#define MOTOR_ARM_UP 950	//Duty cycle 4.75% to arm up the motor
#define MOTOR_MIN_UP 1200	//Min value of duty cycle - 6% (min speed)
#define MOTOR_MAX_UP 3000	//Max value of duty cycle - 15% (Max speed)

void Motors_Init();
void Motors_On();
void Motor_Arm(int channel);
void Motor_Write_PWM(int channel, float value);
void Motor_Write_up(int channel, float us);
void Motors_Off();

#endif /* DUCTED_DRIVERS_MOTOR_H_ */
