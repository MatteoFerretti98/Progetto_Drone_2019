/*******************************************************************************
* Author: Ragaini Davide
 *******************************************************************************/
/******************************************************************************
 * File Name: SetupAHRS.c
* Version: 1.0
* Description: Routine di setup e lettura per A.H.R.S.
*******************************************************************************/
/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			13.04.2019     1.0         First Release.
*******************************************************************************/



#include "../../../Progetto_Drone_2019/Prova_motori/src/IMU/SetupAHRS.h"
#include "platform.h"

//Routine di setup dell'imu


void Setup_MARG(AHRS_out* ahrs)
{
	char msg[12];

	//lcd_initialize();
	//lcd_clear();
	lcd_display(LCD_LINE1,"  SETUP IMU ");
	lcd_display(LCD_LINE2,"  PRESS SW1 ");
	lcd_display(LCD_LINE3,"  FOR NEW   ");
	lcd_display(LCD_LINE4,"CALIBRATION ");
	lcd_display(LCD_LINE6,"  PRESS SW2 ");
	lcd_display(LCD_LINE7,"FOR DEFAULT ");
	lcd_display(LCD_LINE8,"  PARAMS    ");

	imu_init(&ahrs->sens);
	mag_init(&ahrs->mag);


// Asks to press SW1 or SW2 to choose the desired parameters
	while( PORT4.PIDR.BIT.B0 && PORT4.PIDR.BIT.B1){

	}

// Starts a new calibration if you press SW1
	if(!(PORT4.PIDR.BIT.B0)){

		//lcd_initialize();
			lcd_clear();

		lcd_display(LCD_LINE1," IMU SETUP ");
		lcd_display(LCD_LINE2," IN CORSO ");

		lcd_display(LCD_LINE4,"Calibrazione");
		lcd_display(LCD_LINE5,"Magnetometro");

		calibrationYPR(msg, &ahrs->mag);//new calibration
	}
	else if (!(PORT4.PIDR.BIT.B1)) //case of SW2 pressed. Sets the default values for calibration.
		calibrationYPR1(msg, &ahrs->mag);
	else calibrationYPR(msg, &ahrs->mag); //in case of errors, starts a new calibration


}


// Filtered Data (that come from MARG) read function
void Read_MARG(AHRS_out* ahrs)
{
	imu_read(&ahrs->raw, &ahrs->sens, &ahrs->temp);
	mag_read(&ahrs->mag);
	getYPR(&ahrs->mag, &ahrs->temp, &ahrs->ahrs_data);
}

