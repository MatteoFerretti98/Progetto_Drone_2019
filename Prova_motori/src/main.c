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

#include <stdint.h>
#include <stdio.h>
#include "CMT.h"
#include "PrintOnScreen.h"
#include "SetupAHRS.h"


main(void)
{
	/*Dichiarazioni strutture dati utilizzate.*/
	AHRS_out ahrs;
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
	extern struct timerClocks timers;

	/* funzione di setup dello schermo, CMT, accelerometro, giroscopio, magnetometro e calibrazione magnetometro.*/
	Setup_MARG(&ahrs);

	while (1)
	{
		/********************
 	 	* La frequenza di lettura va impostata in base alla frequenza del mpu e magnetometro.
 	 	* Consultare i rispettivi file imu.c e mag.c.
 	 	*
 	 	* Esempio:
 	 	* La frequenza attuale del mpu nel file imu.c è 200Hz, mentre del magnetometro è 220Hz.
 	 	* 1/200 = 0.005 s = 5mS
 	 	* ********************/
		if(timers.timer_5mS)
		{
			Read_MARG(&ahrs);
			RealTimeChart(&ahrs);

			timers.timer_5mS=0;

			/*************
		 	 * Frequenza di stampa su schermo dei dati.
		 	 * Lo schermo ha un suo tempo di aggiornamento dello schermo.
		 	 * Aumentare la frequenza oltre i 100mS è sconsigliato.
		 	 *************/
			if(timers.timer_100mS)
			{
				Print_ABS(&ahrs);
				Print_Angoli(&ahrs);
				Print_VelAng(&ahrs);
				//Print_Temp(&ahrs);

				timers.timer_100mS = 0;
			}
		}
	}
}/* End of Main*/

