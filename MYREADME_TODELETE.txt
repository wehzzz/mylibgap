Infos MPU6050:
Register map: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

HAL_I2C_Mem_Write: https://www.disca.upv.es/aperles/arm_cortex_m3/llibre/st/STM32F439xx_User_Manual/group__i2c__exported__functions__group2.html#ga33e725a824eb672f9f999d9d5ce088fb
	Pointer to i2c struct
	target device addr
	internal memory addr
	size of internal memory addr
	pointer to data
	amount of data to be sent
	timeout
Dans mpu6050_init, mettre DPLF ? filtre passe-bas numérique ?
// Config gyroscope full scale range 250°/s
// status |= mem_write(MPU6050_GYRO_CONFIG, 0);
// gap_log(LOG_DEBUG, "Configure range %d", status);
Conversion entre raw valeur et °/s ?
Comment interpréter le retour du gyro ?
Est-ce que mes 0bxxxx sont considérés comme valeur en dur ? ("mpu6050.c")


Finir remplir README


Selftest on commence ?
"Les 2 points restants sont distribués dès l’établissement de l’infrastructure de
test."
