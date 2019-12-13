
//general includes
#include <stdint.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "s12adc.h"

//altimeter includes
#include "Ducted_Drivers/Motor.h"
#include "Ducted_Drivers/PID.h"
#include "LowLevelDrivers/CMT.h"
#include "LowLevelDrivers/I2C.h"
#include "Ducted_Drivers/lcd_buffer.h"
#include "Altimetro.h"
#include "Ducted_Drivers/map.h"
#include "I2C_new.h"

//IMU includes
#include "CMT.h"
#include "PrintOnScreen.h"
#include "SetupAHRS.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 /******************************************************************************
* File Name: Main.c
* Version: 1.0
* Description: File main sistema A.H.R.S.
*******************************************************************************/
/*******************************************************************************
* Author: Ragaini Davide
*******************************************************************************/
/*******************************************************************************
* History
*			DD.MM.YYYY     Version     Description
*			03.12.2018     1.0         Creazione File
*******************************************************************************/
/*




main(void)
{
	/*Dichiarazioni strutture dati utilizzate.*/
	//AHRS_out ahrs;
	/***************************************************************************************
	* extern struct timerClocks timers
	* Description: Avremmo potuto utilizzare per la struttura dati "timerClocks", nel file "CMT.h",
	* 				il typedef e dichiarare nel corrente main la struttura dati (come fatto per le altre stutture dati).
	* 				Per compatibilità, abbiamo deciso di lasciare invariata la libreria "CMT.h"
	* 				(poichè è libreria standatd della renesas) in caso di import del
	* 				codice del sistema AHRS in un altro progetto.
	* 				In tal caso basterebbe dichiarare la struttura come "extern" lasciando invariato il "CMT.h" del
	* 				nuovo progetto.
	**************************************************************************************/
	//extern struct timerClocks timers;

	/* funzione di setup dello schermo, CMT, accelerometro, giroscopio, magnetometro e calibrazione magnetometro.*/
	//Setup_MARG(&ahrs);

	/*while (1)
	{
		/********************
 	 	* La frequenza di lettura va impostata in base alla frequenza del mpu e magnetometro.
 	 	* Consultare i rispettivi file imu.c e mag.c.
 	 	*
 	 	* Esempio:
 	 	* La frequenza attuale del mpu nel file imu.c è 200Hz, mentre del magnetometro è 220Hz.
 	 	* 1/200 = 0.005 s = 5mS
 	 	* ********************/
		/*if(timers.timer_5mS)
		{
			Read_MARG(&ahrs);
			RealTimeChart(&ahrs);

			timers.timer_5mS=0;

			/*************
		 	 * Frequenza di stampa su schermo dei dati.
		 	 * Lo schermo ha un suo tempo di aggiornamento dello schermo.
		 	 * Aumentare la frequenza oltre i 100mS è sconsigliato.
		 	 *************/
			/*if(timers.timer_100mS)
			{
				Print_ABS(&ahrs);
				Print_Angoli(&ahrs);
				Print_VelAng(&ahrs);
				//Print_Temp(&ahrs);

				timers.timer_100mS = 0;
			}
		}
	}
}// End of Main
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////


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

struct angles {
	float yaw;
	float pitch;
	float roll;
};

struct dynamic {
	struct axis acc;
	struct axis vel;
	struct axis pos;
};

struct physicalState {
//TODO: maybe these structures are useless, because the IMU program does the work
	//struct dynamic accel;
	//struct dynamic gyro;
	//struct dynamic magn;
	struct dynamic abs;
	//struct dynamic Kalman;

	struct angles angle;
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
PID_config Yaw_PID;
PID_config Pitch_PID;
PID_config Roll_PID;


/* Used to store value of altitude, yaw, pitch, roll need to be reached */
float altitudeValue = 0.5;
float yawValue = 0;
float pitchValue = 0;
float rollValue = 0;

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

	//variables needed to arm the motors (I think)
	desiredState.key.motor_diff_us = 0; // variable to control the rotation
	desiredState.key.abs.pos.z = 0.20;

	/*************************************************************
	 * TODO here we need to implements kp, ki, kd for IMUs PID
	PID_Init(&Pitch_PID, kpPITCH, kdPITCH, kiPITCH, dt, 0, 1);
	PID_Init(&Roll_PID, kpROLL, kdROLL, kiROLL, dt, 0, 1);
	PID_Init(&Yaw_PID,  kpYAW, kdYAW, kiYAW, dt, 0, 1);
	**************************************************************/
}

void Callback_1ms(){

}

void Callback_5ms(){

}

void Callback_10ms(){

}

void Callback_20ms(){

}


float outValue_alt;	// Temporary storage for PID results
void Callback_50ms(){

		distanza = Read(temp);
		float distanza_metri = (float)distanza/1000;
		display_results(distanza_metri);

		desiredState.key.abs.pos.z = altitudeValue;

		/* Setting motor speed based on altitude */
		char result_string2[15];
		char result_string3[15];
		outValue_alt = PID_Compute(distanza_metri, desiredState.key.abs.pos.z, &z_axis_PID);

		/*********************************************************************************************
		 * CODE FOR IMU PIDs

		  TODO: here we need to get IMU's measures

		  desiredState.key.angle.pitch = pitchValue;
		  desiredState.key.angle.roll = rollValue;
		  desiredState.key.angle.yaw = yawValue;

		  TODO: instead of pitch, roll, yaw as the first arguments we need to pass the angles found by IMU
		  outValue_pitch = PID_Compute(pitch,  desiredState.key.angle.pitch, &Pitch_PID);
		  outValue_roll = PID_Compute(roll,  desiredState.key.angle.roll, &Roll_PID);
		  outValue_yaw = PID_Compute(yaw,  desiredState.key.angle.yaw,  &Yaw_PID);

		/*****************************************************************************************************/

		//sprintf(result_string2,"%5.2f",outValue_alt);
		//lcd_display(LCD_LINE5,(const uint8_t *) result_string2);
		desiredState.key.avg_motor_us = map(outValue_alt, 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		//sprintf(result_string3,"%5.2f",desiredState.key.avg_motor_us);
		//lcd_display(LCD_LINE3,(const uint8_t *) result_string3);
		// Write new results to motors and servos
		//******************************************************************************************
		Motor_Write_up(MOTOR_1, desiredState.key.avg_motor_us + desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_2, desiredState.key.avg_motor_us - desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_3, desiredState.key.avg_motor_us + desiredState.key.motor_diff_us);
		Motor_Write_up(MOTOR_4, desiredState.key.avg_motor_us - desiredState.key.motor_diff_us);
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

