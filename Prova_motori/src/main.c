/*******************************************************************************
* Authors:
* Alessandro De Toni,
* Angelo D'Agostino Bonomi,
* Matteo Ferretti,
* Simone Di Rado
*******************************************************************************/
/******************************************************************************
* File Name: main.c
* Version: 2.0
* Description: file 'main' quadrotor
*******************************************************************************/
/*******************************************************************************
* History
*			DD.MM.YYYY     Version     Description
*			03.12.2018     1.0         IMU File creation (Ragaini Davide)
*			16.12.2019	   2.0 		   Merge of IMU's and altimeter's codes
*******************************************************************************/

//general includes
#include <CMT.h>
#include <CMT.h>
#include <stdint.h>
#include <stdio.h>
#include <machine.h>
#include <PID.h>
#include "platform.h"
#include "s12adc.h"
#include "I2C_new.h"
#include <r_switches.h>

//Altimeter includes
#include "Ducted_Drivers/Motor.h"
#include "LowLevelDrivers/I2C.h"
#include "Ducted_Drivers/lcd_buffer.h"
#include "Altimetro.h"
#include "Ducted_Drivers/map.h"


//IMU includes
#include "PrintOnScreen.h"
#include "SetupAHRS.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
 Prototypes for local functions
 *******************************************************************************/

void Callback_1ms();
void Callback_5ms();
void Callback_10ms();
void Callback_20ms();
void Callback_50ms();
void Callback_100ms();
void Callback_500ms();
void Callback_1000ms();
void Fallback();
void Altimeter_init();
void display_results (uint16_t);
void Setup_Motor_PID();
float* SpeedCompute (float virtualInputs[4], float b, float l, float d);

/*******************************************************************************
 Global variables
 *******************************************************************************/
double motor_switch = false;

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
	//struct dynamic Kalman;

	struct dynamic abs;
	struct angles angle;
	float avg_motor1_us;
	float avg_motor2_us;
	float avg_motor3_us;
	float avg_motor4_us;
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


/* Create PID structure used for PID properties */
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

/*Structure used by IMU for initialization*/
AHRS_out ahrs;


void Altimeter_init()
{
	temp = &sensore;
	i2c_init();
	Sensor_Init(temp);
	setTimeout(500, temp);
	init(temp);
	stopContinuous(temp);
	setDistanceMode(Short, temp);
	setMeasurementTimingBudget(20000, temp);
	startContinuous(30, temp);
}

void display_results (uint16_t distanzam)
{
	lcd_display(LCD_LINE1, "****TEST****");
	lcd_display(LCD_LINE2, "Sens:VL53L1X");
	lcd_display(LCD_LINE3, "-------------");
	lcd_display(LCD_LINE4, "***SHORT****");

	char result_string[20];
    sprintf(result_string, "D:  %d  mm", distanza);

    /* Update the display LINE 6 */
    lcd_display(LCD_LINE6, (const uint8_t *)result_string);

    lcd_display(LCD_LINE8, "-------------");
} /* End function display_results() */

// Structure containing timer flags
extern struct timerClocks timers;

void main(void) {

	/* One time initialization instructions */
	CMT_init();

	/* Dichiarazione switch 3*/
	PORT4.PODR.BIT.B4 =0;
	PORT4.PDR.BIT.B4 =0;
	PORT4.PMR.BIT.B4 =0;

	lcd_initialize();
	lcd_clear();

	/*arms the motors, initializes PIDs*/
	 Setup_Motor_PID();

	 /* Altimeter */
	 Altimeter_init();

	 /* IMU */
	 /* LCD, CMT, accelerometer, gyroscope, magnetometer setup and calibration*/
	 Setup_MARG(&ahrs);

	 /* necessary time to arm motor 1, 2, 3 and 4 */
	 while(!timers.timer_2000mS) {
			//lcd_display(LCD_LINE2, " 2s to arm ");
	}
	timers.timer_2000mS = 0;

	/* Endless loop*/
	while (1) {

		if (timers.timer_1mS) {
			timers.timer_1mS = 0;
			//Callback_1ms();								//Operations to do every 1ms

			if (timers.timer_5mS) {
				timers.timer_5mS=0;
				Callback_5ms();							// Operations to do every 5ms

				if (timers.timer_10mS) {
					timers.timer_10mS = 0;
					//Callback_10ms();					// Operations to do every 10ms

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
								//Callback_500ms();		// Operations to do every half a second

								if (timers.timer_1000mS) {
									timers.timer_1000mS = 0;
									//Callback_1000ms();	// Operations to do every second
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




void Setup_Motor_PID() {

	/* Display message on LCD */
	lcd_display(LCD_LINE2, "    SETUP   ");

	/* Initialize motors */
	Motors_Init();
	/* Turn on motors relay */
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	StartCount_MTUs();
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	/* Send arm signal to motors */
	Motor_Arm(MOTOR_1);
	Motor_Arm(MOTOR_2);
	Motor_Arm(MOTOR_3);
	Motor_Arm(MOTOR_4);

	/*TODO: maybe no need to initialize CMT, Setup() does the work*/
	/* Setup Compare Match Timer */
	//CMT_init();

	// Initialize PID structures used for PID properties
	// with their respective coefficents for proportional,
	// derivative and integrative
	PID_Init(&z_axis_PID, 0.7, 0.05, 0.3, dt, 0, 1);

	//variables needed to arm the motors (I think)
	desiredState.key.motor_diff_us = 0; // variable to control the rotation
	desiredState.key.abs.pos.z = 0.20;

	/*IMUs PID: they need to be calibrated, right now Kp and Kd are set to 1*/
	PID_Init(&Pitch_PID, 1, 1, 0, dt, 0, 1);
	PID_Init(&Roll_PID, 1, 1, 0, dt, 0, 1);
	PID_Init(&Yaw_PID,  1, 1, 0, dt, 0, 1);
}

void Callback_1ms(){

}


/*************************************************************
 *Function name: Callback_5ms()
 *Description:   collects IMU data
 *Arguments:     none
 *Return value:  none
 ************************************************************/
void Callback_5ms(){

	Read_MARG(&ahrs);
	RealTimeChart(&ahrs);

}

void Callback_10ms(){

}

void Callback_20ms(){

}


/*float outValue_alt;
float outValue_pitch;
float outValue_roll;
float outValue_yaw;*/

/*************************************************************
 *Function name: Callback_50ms()
 *Description:   Core function of this program, computes and updates
 	 	 	 	 the right speed that every motor needs to rotate with
 *Arguments:     none
 *Return value:  none
 ************************************************************/
void Callback_50ms(){



		distanza = Read(temp);
		float distanza_metri = (float)distanza/1000;

		/*gets the angles measured by IMU*/
		currentState.key.angle.pitch=ahrs.ahrs_data.PitchDeg;
		currentState.key.angle.roll=ahrs.ahrs_data.RollDeg;
		currentState.key.angle.yaw=ahrs.ahrs_data.YawDeg;


		//calculates the altitude value when pitch and roll are not null
		if (currentState.key.angle.pitch!=0||currentState.key.angle.roll!=0)
			{
				float hpitch =distanza_metri*cos(currentState.key.angle.pitch);
				float hroll =distanza_metri*cos(currentState.key.angle.pitch);

				distanza_metri=sqrt(pow(hpitch,2)+pow(hroll,2));
			}

		display_results(distanza_metri);



		/* Setting desired values */
		desiredState.key.abs.pos.z = altitudeValue;
		desiredState.key.angle.pitch = pitchValue;
		desiredState.key.angle.roll = rollValue;
		desiredState.key.angle.yaw = yawValue;


		/* computing IMU PIDs results*/

		float virtualInputs [4]; // Temporary storage for PID results

		virtualInputs[0] = PID_Compute(distanza_metri, desiredState.key.abs.pos.z, &z_axis_PID);
		virtualInputs[1] = PID_Compute(currentState.key.angle.pitch,  desiredState.key.angle.pitch, &Pitch_PID);
		virtualInputs[2] = PID_Compute(currentState.key.angle.roll,  desiredState.key.angle.roll, &Roll_PID);
		virtualInputs[3] = PID_Compute(currentState.key.angle.yaw,  desiredState.key.angle.yaw,  &Yaw_PID);

		float* Speeds;

		//computes motor speeds (B1 is for 4-cell battery, if you use a 3-cell, change it with B2)
		Speeds = SpeedCompute (virtualInputs, B_4, L, D);

		//sprintf(result_string2,"%5.2f",outValue_alt);
		//lcd_display(LCD_LINE5,(const uint8_t *) result_string2);

		//converts the speed in a measure that can be read by the motors
		desiredState.key.avg_motor1_us = map(*(Speeds+0), 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		desiredState.key.avg_motor2_us = map(*(Speeds+1), 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		desiredState.key.avg_motor3_us = map(*(Speeds+2), 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		desiredState.key.avg_motor4_us = map(*(Speeds+3), 0, 1, MOTOR_MIN_UP, MOTOR_MAX_UP);
		//sprintf(result_string3,"%5.2f",desiredState.key.avg_motor_us);
		//lcd_display(LCD_LINE3,(const uint8_t *) result_string3);

		if((1 != PORT4.PIDR.BIT.B4)&&(motor_switch==true)) //Press SW3 to send the pwm signal to the ESC
		{
			motor_switch=false;
		}
		else if((1 != PORT4.PIDR.BIT.B4)&&(motor_switch==false)) //Press SW3 to stop the pwm signal sending to the ESC
		{
			motor_switch=true;
		}

		if(motor_switch==true) //Press SW3 to send the pwm signal to the ESC
		{
			StartCount_MTUs();
		//******************************************************************************************
			// writes new results to motors and servos
			Motor_Write_up(MOTOR_1, desiredState.key.avg_motor1_us);
			Motor_Write_up(MOTOR_2, desiredState.key.avg_motor2_us);
			Motor_Write_up(MOTOR_3, desiredState.key.avg_motor3_us);
			Motor_Write_up(MOTOR_4, desiredState.key.avg_motor4_us);
		//*******************************************************************************************
		}
		else if(motor_switch==false) //Press SW3 to stop the pwm signal sending to the ESC
		{
		//******************************************************************************************
			Motors_Off();
			HaltCount_MTUs();
		//******************************************************************************************
		}
}

/*************************************************************
 *Function name: Callback_100ms()
 *Description:   Prints IMU measures on the screen
 *Arguments:     none
 *Return value:  none
 ************************************************************/
void Callback_100ms(){

	/************************************************************
	 * The screen has his own time of updating, increasing the update
	 * frequency over 1/100ms is not recommended
	 ************************************************************/

	Print_ABS(&ahrs);
	Print_Angoli(&ahrs);
	Print_VelAng(&ahrs);
	//Print_Temp(&ahrs);

}

void Callback_500ms(){

}

void Callback_1000ms(){

}


/******************************************************************
 *Function name: SpeedCompute
 *Description  : Computes the speed that each motor has to generate to reach the desired attitude
 *Arguments    :
 * @param virtualInputs
 * @param b thrust coefficient
 * @param l distance between motor and center of the drone
 * @param d drag coefficient
 * @return array of the speeds
 */
float* SpeedCompute (float virtualInputs [], float b, float l, float d)
{
	static float Speeds[4];

	Speeds[0] = (1/4*b)*virtualInputs[0] - (1/2*b)*virtualInputs[2] - (1/4*d)*virtualInputs[3];
	Speeds[0] = (1/4*b)*virtualInputs[0] - (1/2*b)*virtualInputs[1] + (1/4*d)*virtualInputs[3];
	Speeds[0] = (1/4*b)*virtualInputs[0] + (1/2*b)*virtualInputs[2] - (1/4*d)*virtualInputs[3];
	Speeds[0] = (1/4*b)*virtualInputs[0] + (1/2*b)*virtualInputs[1] + (1/4*d)*virtualInputs[3];

	return Speeds;
}


void Fallback() {

	lcd_display(LCD_LINE2, "  Fallback  ");
	lcd_display(LCD_LINE3, " condition! ");

	/* Turn off motors */
	Motors_Off();
	HaltCount_MTUs();

	while(1)
		nop();
}
