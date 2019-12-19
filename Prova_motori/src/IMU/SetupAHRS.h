/******************************************************************************
* File Name: SetupAHRS.h
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



#ifndef SRC_SETUPAHRS_H_
#define SRC_SETUPAHRS_H_

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include "platform.h"
#include "imu.h"
#include "mag.h"
#include "AHRS.h"


/********************************************************************************
 * typedef struct AHRS_out
 * Al fine di facilitare le inizializzazioni e le chiamate a funzioni, abbiamo
 * creato la struttura dati AHRS_out.
 * Dichiarando un istanza di questa struttura, si dichiarano automaticamente
 *  anche le strutture dati necessarie all'algoritmo AHRS.
 * Basta diciarare una istanza di questa struttura nel main e passarla come
 * riferimeno alle funzioni.
 *
 * IMPORTANTE!
 * Da non confondere con AHRS_data che contiene i dati relativi all'orientamento.
 ********************************************************************************/
typedef struct{
	MAG_data mag;
	IMU_sens sens;
	IMU_raw raw;
	IMU_temp temp;
	AHRS_data ahrs_data;
}AHRS_out;


/***************************************************************************************
 * Function name: Setup_MARG
 * Description  : Esegue l'inizializzazione dello schermo, CMT, accelerometro, giroscopio
 * 				  e magneometro; infine esegue la calibrazione del magnetometro.
 * Arguments    : Puntatore alla struttura dati AHRS_out.
 * Return Value : Non restituisce nulla, ma stampa i parametri della calibrazione su schermo.
 **************************************************************************************/
void Setup_MARG(AHRS_out* ahrs);


/***************************************************************************************
 * Function name: Read_MARG
 * Description  : Questa funzione legge i dati dall'IMU, magnetometro, li filtra e aggiorna
 * 				  la struttura dati AHRS_data.
 * Arguments    : Puntatore alla struttura dati AHRS_out.
 * Return value : Non restituisce niente ma aggiorna i dati della struttura dati AHRS_data.
 **************************************************************************************/
void Read_MARG(AHRS_out* ahrs);


/***************************************************************************************
 * Function name: Centro_di_massa
 * Description  : Corregge l'orientamento se il sensore non posizionato nel centro di massa.
 * Arguments    :
 * Return value :
 **************************************************************************************/
void Centro_di_massa(AHRS_out* ahrs);


#endif /* SRC_SETUPAHRS_H_ */
