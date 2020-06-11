compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) sensor/baro/baro.ms5611.c
	$(compiler) $(cflag) sensor/gyro/gyro.l3gd20.c
	$(compiler) $(cflag) sensor/pack/g_a_b.icm20789.c
	$(compiler) $(cflag) sensor/pack/g_a_m.lsm9ds1.c
	$(compiler) $(cflag) sensor/pack/g_a_m.mpu9250.c
	$(compiler) $(cflag) hwinfo/hwinfo.acpi.c
	$(compiler) $(cflag) hwinfo/hwinfo.dts.c
	$(compiler) $(cflag) driver.c
clean:
	rm -f *.o
