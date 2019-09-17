compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) sensor/sensor.icm20789.c
	$(compiler) $(cflag) sensor/sensor.l3gd20.c
	$(compiler) $(cflag) sensor/sensor.lsm9ds1.c
	$(compiler) $(cflag) sensor/sensor.mpu9250.c
	$(compiler) $(cflag) sensor/sensor.ms5611.c
	$(compiler) $(cflag) hwinfo/hwinfo.acpi.c
	$(compiler) $(cflag) hwinfo/hwinfo.dts.c
	$(compiler) $(cflag) driver.c
clean:
	rm -f *.o
