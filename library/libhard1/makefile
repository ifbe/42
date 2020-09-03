CC:=gcc
CF:=-c -O2 -fPIC

target:
	$(CC) $(CF) sensor/baro/baro.ms5611.c
	$(CC) $(CF) sensor/gyro/gyro.l3gd20.c
	$(CC) $(CF) sensor/pack/g_a_b.icm20789.c
	$(CC) $(CF) sensor/pack/g_a_m.lsm9ds1.c
	$(CC) $(CF) sensor/pack/g_a_m.mpu9250.c
	$(CC) $(CF) hwinfo/hwinfo.acpi.c
	$(CC) $(CF) hwinfo/hwinfo.dts.c
	$(CC) $(CF) driver.c
clean:
	rm -f *.o
