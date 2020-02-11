enum icm20689{
	CONFIG	      = 0x1A,
	GYRO_CONFIG   = 0x1B,
	ACCEL_CONFIG  = 0x1C,
	ACCEL_CONFIG2 = 0x1D,

	FIFO_EN       = 0x23,

	INT_STATUS    = 0x3A,

	INT_ENABLE    = 0x38,

	TEMP_OUT_H    = 0x41,
	TEMP_OUT_L    = 0x42,

	USER_CTRL     = 0x6A,
	PWR_MGMT_1    = 0x6B,

	FIFO_COUNTH   = 0x72,
	FIFO_COUNTL   = 0x73,
	FIFO_R_W      = 0x74,
	WHO_AM_I      = 0x75,
};
