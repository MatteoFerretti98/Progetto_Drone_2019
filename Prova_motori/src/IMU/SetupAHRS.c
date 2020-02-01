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


// chiede di premere o lo sw1 o lo sw2 in base alla scelta dei parametri desiderati
	while( PORT4.PIDR.BIT.B0 && PORT4.PIDR.BIT.B1){

	}
// se premuto sw1 parte una nuova calibrazione
	if(!(PORT4.PIDR.BIT.B0)){

		//lcd_initialize();
			lcd_clear();


		lcd_display(LCD_LINE1," IMU SETUP ");
		lcd_display(LCD_LINE2," IN CORSO ");

		lcd_display(LCD_LINE4,"Calibrazione");
		lcd_display(LCD_LINE5,"Magnetometro");

		calibrationYPR(msg, &ahrs->mag);//calibrazione nuova
	}
	else if (!(PORT4.PIDR.BIT.B1)) //se viene premuto lo sw2 partono i calori di default
		calibrationYPR1(msg, &ahrs->mag);
	else calibrationYPR1(msg, &ahrs->mag); // nel caso in cui ci siano errori parte in default la calibrazione dei valori già calcolati


}


// Funzione di lettura dei dati dell' MARG filtrati
void Read_MARG(AHRS_out* ahrs)
{
	imu_read(&ahrs->raw, &ahrs->sens, &ahrs->temp);
	mag_read(&ahrs->mag);
	getYPR(&ahrs->mag, &ahrs->temp, &ahrs->ahrs_data);
}



// Centro di massa
/*void Centro_di_massa(AHRS_out* ahrs)
{

}*/

