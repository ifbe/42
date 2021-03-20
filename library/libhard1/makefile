CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
sensor/baro/baro.ms5611.c \
sensor/gyro/gyro.l3gd20.c \
sensor/pack/g_a_b.icm20789.c \
sensor/pack/g_a_m.lsm9ds1.c \
sensor/pack/g_a_m.mpu9250.c \
devmap/acpi/devmap.acpi.c \
devmap/dts/devmap.dts.c \
memmap/memmap.bios.c \
memmap/memmap.uefi.c \
driver.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o
