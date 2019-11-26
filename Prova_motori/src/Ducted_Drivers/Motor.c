/*************************************************
 * File Name  : Motor.c
 * Version    : 2.0
 *  Created on: 06/dec/2018
 *      Author: Francesco Gaudeni
 *************************************************/
#include "Motor.h"

/*************************************************************
 *Function name: Motors_Init
 *Description:   Set all parameter needed by MTU3 unit
 *Arguments:     none
 *Return value:  none
 ************************************************************/
void Motors_Init()
{
	//Set all parameter needed by MTU3 unit
	Set_MTU_U0_C3();
}

/*************************************************************
 *Function name: Motors_On
 *Description:   Start MTU3 count
 *Arguments:     none
 *Return value:  none
 ************************************************************/
void Motors_On()
{
	//Start MTU3 count
	StartCount_MTU_U0_C3();
}

/*************************************************************
 *Function name: Motor_Arm
 *Description  : Recalls Motor_Write_up function based to the select motor
 *Arguments    : int variable to select the motor
 *Return value : none
 ************************************************************/
void Motor_Arm(int channel)
{
	//channel value must be 1 or 2
	if(channel < 1 || channel > 2)	return;

	Motor_Write_up(channel, MOTOR_ARM_UP);
}

/*************************************************************
 *Function name: Motor_Write_PWM
 *Description  : Sets new pwm value due to duty cycle value
 *Arguments    : int variable to select the motor, float variable to indicate the percentage of duty cycle
 *Return value : none
 ************************************************************/
void Motor_Write_PWM(int channel, float value)
{
	//channel value must be 1 or 2
	if(channel < 1 || channel > 2)	return;

	//check if value is a percentage, if not in range 0-100 return
	if(value<=0 || value>=100)	return;

	uint16_t tgr_a_val, tgr_b_val, tgr_d_val;
	tgr_a_val = GetTGR_A_MTU_U0_C3();

	// calculate tgrb value from duty cycle
	switch(channel){
	case 1:
		tgr_b_val = (tgr_a_val*value)/100;
		SetTGR_B_MTU_U0_C3(tgr_b_val);
		break;
	case 2:
		tgr_d_val = (tgr_a_val*value)/100;
		SetTGR_D_MTU_U0_C3(tgr_d_val);
		break;
	}

}

/*************************************************************
 *Function name: Motor_Write_up
 *Description  : Recalls Motor_Write_PWM function calculating the duty cycle value
 *Arguments    : int variable to select the motor, float variable to indicate the high value time
 *Return value : none
 ************************************************************/
void Motor_Write_up(int channel, float up)
{
	//channel value must be 1 or 2
	if(channel < 1 || channel > 2)	return;

	Motor_Write_PWM(channel, (up*100)/MOTOR_PWM_SIGNAL_PERIOD_UP);
}

/*************************************************************
 *Function name: Motors_Off
 *Description  : Stop MTU3 count
 *Arguments    : none
 *Return value : none
 ************************************************************/
void Motors_Off()
{
	HaltCount_MTU_U0_C3();	//Stop MTU3 count
}
