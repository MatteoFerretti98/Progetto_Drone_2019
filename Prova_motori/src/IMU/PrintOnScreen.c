/******************************************************************************
* Author: Ragaini Davide
 ******************************************************************************/
/*******************************************************************************
* File Name: PrintOnScreen.c
* Version: 1.0
* Description: Stampa su schermo integrato al microcontrollore dei dati.
*				Non è necessaria al fine del sistema di orientamento.
*				Da utilizzare in caso di test del codice sistema di orientamento.
*******************************************************************************/
/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			13.04.2019     1.0         First Release.
*******************************************************************************/


#include "PrintOnScreen.h"

/* Variabili per la funzione RealTimeChart */
volatile float yaw = 0;
volatile float pitch = 0;
volatile float roll = 0;
volatile float oy = 0;
volatile float op = 0;
volatile float or = 0;


void Print_ABS(AHRS_out* ahrs)
{
	char msg[12];
	float mq = (ahrs->mag.x * ahrs->mag.x + ahrs->mag.y * ahrs->mag.y + ahrs->mag.z * ahrs->mag.z);
	float abs = ( mq - ahrs->mag.ABS ) / ahrs->mag.ABS;
	sprintf(msg,"ABS:%4.3f",abs);
	//lcd_display(LCD_LINE1,(uint8_t*) msg);
}



void Print_Angoli(AHRS_out* ahrs)
{
	char msg[12];
	sprintf(msg,"Y: %3.1f",ahrs->ahrs_data.YawDeg);
	lcd_display(LCD_LINE3,(uint8_t*) msg);
	sprintf(msg,"P: %3.1f",ahrs->ahrs_data.PitchDeg);
	lcd_display(LCD_LINE4,(uint8_t*) msg);
	sprintf(msg,"R: %3.1f",ahrs->ahrs_data.RollDeg);
	lcd_display(LCD_LINE5,(uint8_t*) msg);

}



void Print_VelAng(AHRS_out* ahrs)
{
	char msg[12];
	sprintf(msg,"oY: %4.1f",ahrs->ahrs_data.omegaYawDeg);
	lcd_display(LCD_LINE6,(uint8_t*) msg);
	sprintf(msg,"oP: %4.1f",ahrs->ahrs_data.omegaPitchDeg);
	lcd_display(LCD_LINE7,(uint8_t*) msg);
	sprintf(msg,"oR: %4.1f",ahrs->ahrs_data.omegaRollDeg);
	lcd_display(LCD_LINE8,(uint8_t*) msg);

}



void Print_Temp(AHRS_out* ahrs)
{
	char msg[12];
	sprintf(msg,"Yr: %4.1f",ahrs->temp.gyrYaw);
	lcd_display(LCD_LINE6,(uint8_t*) msg);
	sprintf(msg,"Pr: %4.1f",ahrs->temp.gyrPitch);
	lcd_display(LCD_LINE7,(uint8_t*) msg);
	sprintf(msg,"Rr: %4.1f",ahrs->temp.gyrRoll);
	lcd_display(LCD_LINE8,(uint8_t*) msg);
}



void RealTimeChart(AHRS_out* ahrs)
{
	yaw = ahrs->ahrs_data.YawDeg;
	pitch = ahrs->ahrs_data.PitchDeg;
	roll = ahrs->ahrs_data.RollDeg;
	oy = ahrs->ahrs_data.omegaYawDeg;
	op = ahrs->ahrs_data.omegaPitchDeg;
	or = ahrs->ahrs_data.omegaRollDeg;
}
