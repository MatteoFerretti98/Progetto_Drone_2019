/******************************************************************************
* Author: Ragaini Davide
 ******************************************************************************/
/*******************************************************************************
* File Name: PrintOnScreen.h
* Version: 1.0
* Description: Header for the PrintOnScreen.c class
*******************************************************************************/
/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			13.04.2019     1.0         First Release.
*******************************************************************************/



#ifndef SRC_PRINTONSCREEN_H_
#define SRC_PRINTONSCREEN_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
//#include "platform.h"
#include "mag.h"
#include "AHRS.h"
#include "SetupAHRS.h"


/***************************************************************************************
 * Function name: Print_ABS
 * Description: Stampa su schermo integrato il parametro ABS (si rimanda alla teoria AHRS).
 * 				Fornisce un indicazione della validità della precisione del magnetometro.
 * 				Valore ottimo è il più vicino possibile a 1.
 * Arguments: Puntatore alla struttura MAG_data
 * Return value: Stampa sulla prima riga.
 **************************************************************************************/
void Print_ABS(AHRS_out*);


/***************************************************************************************
 * Function name: Print_Angoli
 * Description: Stampa su schermo integrato gli angoli in gradi.
 * Arguments: Puntatore alla struttura AHRS_data.
 * Return value: Stampa su righe 3, 4 e 5.
 **************************************************************************************/
void Print_Angoli(AHRS_out*);


/***************************************************************************************
 * Function name: Print_VelAng
 * Description: Stampa su schermo integrato le velocità angolari in gradi.
 * Arguments: Puntatore alla struttura AHRS_data.
 * Return value: Stampa su righe 6, 7 e 8.
 **************************************************************************************/
void Print_VelAng(AHRS_out*);

/***************************************************************************************
 * Function name: RealTimeChart
 * Description  : Aggiorna le variabili dichiarate globalmente, per la funzionalità di debug RealTime-Chart.
 * Arguments    : Puntatore alla struttura AHRS_data.
 * Return value : None.
 **************************************************************************************/
void RealTimeChart(AHRS_out*);

/***************************************************************************************
 * Print_Temp
 * Description  : Stampa su schermo integrato le velocità angolari della struttura dati imu_temp.
 * Arguments    : Puntatore alla struttura AHRS_data.
 * Return value : Stampa su righe 6,7 e 8.
 **************************************************************************************/
void Print_Temp(AHRS_out*);

#endif /* SRC_PRINTONSCREEN_H_ */
