#include <stdint.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "s12adc.h"
#include "Ducted_Drivers/Motor.h"
#include "Ducted_Drivers/PID.h"
#include "LowLevelDrivers/CMT.h"
#include "LowLevelDrivers/I2C.h"
#include "Ducted_Drivers/lcd_buffer.h"
#include "Altimetro.h"
#include "Ducted_Drivers/map.h"
#include "I2C_new.h"

/*******************************************************************************
 Prototypes for local functions
 *******************************************************************************/
void Setup();
void Callback_1ms();
void Callback_5ms();
void Callback_10ms();
void Callback_20ms();
void Callback_50ms();
void Callback_100ms();
void Callback_500ms();
void Callback_1000ms();
void Fallback();
void initialize();
void display_results (uint16_t);

/*******************************************************************************

 Global variables
 *******************************************************************************/

struct axis {
	float x;
	float y;
	float z;
};
struct dynamic {
	struct axis acc;
	struct axis vel;
	struct axis pos;
};

struct physicalState {
	struct dynamic accel;
	struct dynamic gyro;
	struct dynamic magn;
	struct dynamic abs;
	struct dynamic Kalman;
	float avg_motor_us;
	float motor_diff_us;
	float x_servo_deg;
	float y_servo_deg;
};
union {
	struct physicalState key;
	float index[sizeof(struct physicalState)];
} desiredState;	//state variables you want to reach

union {
	struct physicalState key;
	float index[sizeof(struct physicalState)];
} currentState;	//current state variables of the DuctedFan

// Structure containing timer flags
extern struct timerClocks timers;

/* Create PID structure used f+or PID properties */
PID_config z_axis_PID;
PID_config Pitch_PID;
PID_config Roll_PID;

/* Used to store value of altitude need to be reached */
float altitudeValue = 0.5;

/* Time in seconds every which PID control is made */
float dt = 0.05;

// variabili altimetro
vl53l1x sensore;
vl53l1x* temp;
uint16_t distanza;

void initialize()
{
	temp = &sensore;
	lcd_display(LCD_LINE1, "****TEST****");
	lcd_display(LCD_LINE2, "Sens:VL53L1X");
	//lcd_display(LCD_LINE3, "-------------");
	i2c_init();
	Sensor_Init(temp);
	setTimeout(500, temp);
	init(temp);

	lcd_display(LCD_LINE4, "***SHORT****");
	stopContinuous(temp);
	setDistanceMode(Short, temp);
	setMeasurementTimingBudget(20000, temp);
	startContinuous(30, temp);
}

void display_results (uint16_t distanzam)
{
    char result_string[20];
    sprintf(result_string, "D:  %d  mm", distanza);

    /* Update the display LINE 7 */
    lcd_display(LCD_LINE6, (const uint8_t *)result_string);
} /* End function display_results() */


// Structure containing timer flags
extern struct timerClocks timers;

void main(void) {
	/* One time initialize instructions */


	Setup();
	initialize();
	while(!timers.timer_2000mS) {
			//lcd_display(LCD_LINE2, " 2s to arm "); // time necessary to arm motor1 and motor2
	}
	timers.timer_2000mS = 0;

	/* Endless loop*/
	while (1) {

		if (timers.timer_1mS) {
			timers.timer_1mS = 0;
			Callback_1ms();								//Operations to do every 1ms
			if (timers.timer_5mS) {
				timers.timer_5mS = 0;
				Callback_5ms();							// Operations to do every 5ms
				if (timers.timer_10mS) {
					timers.timer_10mS = 0;
					Callback_10ms();					// Operations to do every 10ms
					if (timers.timer_20mS) {
						timers.timer_20mS = 0;
						//Callback_20ms();				// Operations to do every 20ms
					}
					if (timers.timer_50mS) {
						timers.timer_50mS = 0;
						Callback_50ms();				// Operations to do every 50ms
						if (timers.timer_100mS) {
							timers.timer_100mS = 0;
							Callback_100ms();			// Operations to do every 100ms
							if (timers.timer_500mS) {
								timers.timer_500mS = 0;
								Callback_500ms();		// Operations to do every half a second
								if (timers.timer_1000mS) {
									timers.timer_1000mS = 0;
									Callback_1000ms();	// Operations to do every second
								}
							}
						}

					}
				}
			}
		}
	}
	// Shutdown everything
	Fallback();

} /* End function main() */

void Setup() {
	/* Initialize LCD */
	lcd_initialize();
	/* Clear LCD */
	lcd_clear();
	/* Display message on LCD */
	//lcd_display(LCD_LINE2, "    SETUP   ");
	/* Initialize motors */
	Motors_Init();
	/* Turn on motors relay */
	Motors_On();
	/* Send arm signal to motors */
	Motor_Arm(MOTOR_1);
	Motor_Arm(MOTOR_2);
	Motor_Arm(MOTOR_3);
	Motor_Arm(MOTOR_4);
	/* Setup Compare Match Timer */
	CMT_init();
	// Initialize PID structures used for PID properties
	// with their respective coefficents for proportional,
	// derivative and integrative
	PID_Init(&z_axis_PID, 0.7, 0.05, 0.3, dt, 0, 1);

	desiredState.key.motor_diff_us = 0; // variable to control the rotation
	desiredState.key.abs.pos.z = 0.20;
}

void Callback_1ms(){

}

void Callback_5ms(){

}

void Callback_10ms(){

}

void Callback_20ms(){

}


float outValue;	// Temporary storage for PID results
void Callback_50ms(){
		distanza = Read(temp);
		float distanza_metri = (float)distanza/1000;
		display_results(distanza_metri);

		desiredState.key.abs.pos.z = altitudeValue;

		/* Setting motor speed based on altitude */
		char result_string2[15];
		char result_string3[15];
		outValue = PID_Compute(distanza_metri, desiredState.key.abs.pos.z, &z_axis_PID);
		//sprintf(result_string2,"%5.2f",outValue);
		//lcd_display(LCD_LINE5,(const uint8_t *) result_string2);
		desiredState.key.avg_motor_us = map(outValue, 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		sprintf(result_string3,"%5.2f",desiredState.key.avg_motor_us);
		lcd_display(LCD_LINE3,(const uint8_t *) result_string3);

		// Write new results to motors and servos
		//******************************************************************************************
		/*Motor_Write_up(MOTOR_1, desiredState.key.avg_motor_us + desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_2, desiredState.key.avg_motor_us - desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_3, desiredState.key.avg_motor_us + desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_4, desiredState.key.avg_motor_us - desiredState.key.motor_diff_us);*/
		Motor_Write_up(MOTOR_1, 1200);
		Motor_Write_up(MOTOR_2, 1800);
		Motor_Write_up(MOTOR_3, 2400);
		Motor_Write_up(MOTOR_4, 3000);
		//*******************************************************************************************
}

void Callback_100ms(){

}

void Callback_500ms(){

}

void Callback_1000ms(){

}

void Fallback() {
	lcd_display(LCD_LINE2, "  Fallback  ");
	lcd_display(LCD_LINE3, " condition! ");

	/* Turn off motors */
	Motor_Write_up(MOTOR_1, 0);
	Motor_Write_up(MOTOR_2, 0);
	Motor_Write_up(MOTOR_3, 0);
	Motor_Write_up(MOTOR_4, 0);
	Motors_Off();

	while(1)
		nop();
}

