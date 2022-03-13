CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
map/acpi/devmap.acpi.c \
map/dts/devmap.dts.c \
map/bios/memmap.bios.c \
map/uefi/memmap.uefi.c \
sensor/baro/baro.ms5611.c \
sensor/gyro/gyro.l3gd20.c \
sensor/pack/g_a_b.icm20789.c \
sensor/pack/g_a_m.lsm9ds1.c \
sensor/pack/g_a_m.mpu9250.c \
usb/perdev/usbhub-driver.c \
usb/perdev/usbds4-driver.c \
usb/perdev/usbxbox-driver.c \
usb/perdev/usbvmware-driver.c \
usb/perfunc/usbhid-driver.c \
usb/perfunc/usbstor-driver.c \
usb/usbcore.c \
eth/eth.c \
driver.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libhard1/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi


clean:
	rm -f *.o
