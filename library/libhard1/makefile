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
usb/peri/usbany-driver.c \
usb/peri/usbhub-driver.c \
usb/peri/usbds4-driver.c \
usb/peri/usbxbox-driver.c \
usb/peri/usbvmware-driver.c \
usb/func/usbhid-driver.c \
usb/func/usbstor-driver.c \
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
