compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) i2c/i2c.icm20789.c
	$(compiler) $(cflag) i2c/i2c.l3gd20.c
	$(compiler) $(cflag) i2c/i2c.lsm9ds1.c
	$(compiler) $(cflag) i2c/i2c.mpu9250.c
	$(compiler) $(cflag) i2c/i2c.ms5611.c
	$(compiler) $(cflag) info/acpi.c
	$(compiler) $(cflag) info/dts.c
	$(compiler) $(cflag) driver.c
clean:
	rm -f *.o
