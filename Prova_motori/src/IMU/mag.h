/******************************************************************************
* File Name: mag.h
* Version: 1.01.1
* Description: Header file per mag.c
*******************************************************************************/
/*******************************************************************************
 * Author: Omar Cocchairella
 *******************************************************************************/
/*******************************************************************************
* History
* 			DD.MM.YYYY     Version     Description
*			22.01.2018     1.01        Revisione della versione di Sebastian Giles
* 										da parte di Omar Cocchairella.
*			10.01.2019     1.01.1      Martin Breccia commento della versione
* 										di Omar Cocchairella.
*******************************************************************************/


#ifndef SRC_MAG_H_  /* Multiple inclusion prevention. */
#define SRC_MAG_H_

#include <CMT.h>
#include "I2C_new.h"
#include "r_riic_rx600_master.h"

#ifdef DROTEK_IMU_10DOF_V2  /* Multiple inclusion prevention. */
#include "eMPL/inv_mpu.h"
#endif

typedef struct {
	short raw[3];
	float x,y,z;
	float sens;
	float bias[3];
	float scale[3];
	float ABS;
} MAG_data;


/*******************************************************************************
 * Function name: mag_init
 * Description  : iniziaizza il magnetometro settando i registri per proseguire
 * 				   ed i reativi bias del magnetometro reativi al luogo di utilizzo.
 * Arguments    : (MAG_data*) mag_data -puntatore alla struttura mag_data
 * 					contenente i dati relativi al magnetometro.
 * Return value : hex - 0x0 se ok.
 * 				  hex - 0x1 non riesce ad accedere al magnetometro 
 * 					     avvia i2c se non riesce ad avviare i2c 0x1.
 * 				  hex - 0x2 non legge il registro HMC5983_ID_A_VAL.
 * 				  hex - 0x3 il magnetometro non e' stato identificato come
 * 							 HMC5983_ID_A_VAL come nel datasheet.
 * 				  hex - 0x4 non abilita la continuous mode HMC5983_MODE
 * 							 o non riesce a scaricare il registro HMC5983_CONF_A.
 * 				  hex - 0x5 non riesce a caricare il registro HMC5983_CONF_A.
 *  			  hex - 0x6 non riesce a scaricare il registro HMC5983_CONF_B.
 *******************************************************************************/
int mag_init(MAG_data*);

/*******************************************************************************
 * Function name: mag_read_raw
 * Description  : lettura dati grezzi (raw).
 * Arguments    : (MAG_data*) mag_data -puntatore alla struttura mag_data
 * 					contenente i dati relativi al magnetometro.
 * Return value : hex - 0x0 se ok.
 * 				  hex - 0x1 non riesce ad accedere al registro di lettura dati 
 * 				   grezzi (raw).
 *******************************************************************************/
int mag_read_raw(MAG_data*);

/*******************************************************************************
 * Function name: mag_read
 * Description  : aggiustamento dati grezzi (raw) secondo il campo magnetico locale.
 * Arguments    : (MAG_data*) mag_data -puntatore alla struttura mag_data
 * 					contenente i dati relativi al magnetometro.
 * Return value : hex - 0x0 se ok.
 * 				  hex - 0x1 se c'e' stato un errore nella lettura dati grezzi (raw).
 *******************************************************************************/
int mag_read(MAG_data*);

/*******************************************************************************
 * Function name: mag_read
 * Description  : settaggio bias scale e abs locale.
 * Arguments    : (MAG_data*) mag_data -puntatore alla struttura mag_data
 * 					contenente i dati relativi al magnetometro.
 * Return value : int -0 se ok.
 *******************************************************************************/
int magcal(MAG_data*);


#endif /* SRC_MAG_H_  Multiple inclusion prevention. */
