/*************************************************
 * File Name  : PID.c
 * Version    : 2.0
 *  Created on: 06/dec/2018
 *      Author: Francesco Gaudeni
 *************************************************/
#include "PID.h"

/*************************************************************
 *Function name: PID_Init
 *Description:   Initialize all Pid variables
 *Arguments:     pointer 'conf' to PID_config struct, six float variables
 *Return value:  none
 ************************************************************/
void PID_Init(PID_config* conf, float kp, float kd, float ki, float dt, float outMin, float outMax)
{
	conf->kp = kp;
	conf->kd = kd;
	conf->ki = ki;
	conf->dt = dt;
	conf->ITerm = 0;
	conf->lastError = 0;
	conf->outMax = outMax;
	conf->outMin = outMin;
}

/*************************************************************
 *Function name: PID_Compute
 *Description:   Calculate new output value
 *Arguments:     two float variables to indicate the input from the altitude sensor and the desired altitude value, a pointer 'conf' to PID_Config struct
 *Return value:  float variable
 ************************************************************/
float PID_Compute(float input, float setPoint, PID_config* conf)
{
      /*Compute all the working error variables*/
      float error = setPoint - input;
      conf->ITerm += (error * conf->dt)* conf->ki;
      if((conf->ITerm) > conf->outMax) conf->ITerm = conf->outMax;
      else if((conf->ITerm) < conf->outMin) conf->ITerm = conf->outMin;
      float dInput = (error - conf->lastError) / conf->dt;

      /*Compute PID Output*/
      float output = (conf->kp * error) + (conf->ITerm) + (conf->kd * dInput);

	  if(output > conf->outMax) output = conf->outMax;
      else if(output < conf->outMin) output = conf->outMin;

      /*Remember some variables for next time*/
	  conf->lastError = error;
	  return output;
}

