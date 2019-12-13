/******************************************************************************
* File Name: imu.c
* Version: 1.01.1
* Description: file contenente funzioni relative all'imu.
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

#include <imu.h>

int imu_read(IMU_raw* imu_raw, IMU_sens* imu_sens, IMU_temp* imu_temp){

	short data_acc[3] = {imu_raw->accRoll,imu_raw->accPitch,imu_raw->accYaw};
	mpu_get_accel_reg(data_acc, NULL);
	imu_raw->accRoll = data_acc[0];
	imu_raw->accPitch = data_acc[1];
	imu_raw->accYaw = data_acc[2];

	short data_gyr[3] = {imu_raw->gyrRoll, imu_raw->gyrPitch, imu_raw->gyrYaw};
	mpu_get_gyro_reg(data_gyr, NULL);
	imu_raw->gyrRoll = data_gyr[0];
	imu_raw->gyrPitch = data_gyr[1];
	imu_raw->gyrYaw = data_gyr[2];

	imu_temp->accRoll=imu_raw->accRoll / imu_sens->acc_sens;
	imu_temp->gyrRoll = imu_raw->gyrRoll / imu_sens->gyr_sens * 0.01745329252;
	imu_temp->accPitch = imu_raw->accPitch / imu_sens->acc_sens;
	imu_temp->gyrPitch = imu_raw->gyrPitch / imu_sens->gyr_sens * 0.01745329252;
	imu_temp->accYaw = imu_raw->accYaw / imu_sens->acc_sens;
	imu_temp->gyrYaw = imu_raw->gyrYaw / imu_sens->gyr_sens * 0.01745329252;
	return 0;
}

int imu_init(IMU_sens* imu_sens){
	// avvia i2c
	if (i2c_init())
		return 0x1;
	// avvia sensore con la libreria Inversense
	if (mpu_init(0))
		return 0x2;
	// abilita sensori Giroscopio e Accelerometro
	if (mpu_set_sensors(INV_XYZ_ACCEL | INV_XYZ_GYRO))
		return 0x3;
	// imposta frequenza la conversione dei dati inerziali
	if (mpu_set_sample_rate(200/*Hz*/)) //Hz
		return 0x4;

	// leggi sensibilità  del sensore
	mpu_get_accel_sens(&imu_sens->acc_sens);
	mpu_get_gyro_sens(&imu_sens->gyr_sens);
	return 0x0;
}

void calibrazioneAngoliRiferimento(IMU_rif* imu_rif){
	// nel caso di errore eccessivo della imu in quanto posizione, effettuare filtro qui
}
