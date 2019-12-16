/******************************************************************************
 * File Name: SetupAHRS.c
* Version: 1.0
* Description: Routine di setup e lettura per A.H.R.S.
*******************************************************************************/
/*******************************************************************************
* Author: Ragaini Davide
 *******************************************************************************/
/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			13.04.2019     1.0         First Release.
*******************************************************************************/



#include "SetupAHRS.h"
#include "platform.h"

//Routine di setup dell'imu
void Setup_MARG(AHRS_out* ahrs)
{
	char msg[12];

	lcd_initialize();
	lcd_clear();

	lcd_display(LCD_LINE1," IMU SETUP ");
	lcd_display(LCD_LINE2," IN CORSO ");

	CMT_init();
	imu_init(&ahrs->sens);
	mag_init(&ahrs->mag);

	lcd_display(LCD_LINE4,"Calibrazione");
	lcd_display(LCD_LINE5,"Magnetometro");

	calibrationYPR(msg, &ahrs->mag);
}



// Funzione di lettura dei dati dell' MARG filtrati
void Read_MARG(AHRS_out* ahrs)
{
	imu_read(&ahrs->raw, &ahrs->sens, &ahrs->temp);
	mag_read(&ahrs->mag);
	getYPR(&ahrs->mag, &ahrs->temp, &ahrs->ahrs_data);
}



// Centro di massa
void Centro_di_massa(AHRS_out* ahrs)
{

}

