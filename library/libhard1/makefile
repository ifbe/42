CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
eth/eth.c \
gpio/gpio.test.c \
gpio/gpio.l298n.c \
mapofdev/acpi/devmap.acpi.c \
mapofdev/dts/devmap.dts.c \
mapofdev/devmap.c \
mapofmem/bios/memmap.bios.c \
mapofmem/uefi/memmap.uefi.c \
mapofmem/memmap.c \
sensor/baro/baro.ms5611.c \
sensor/gyro/gyro.l3gd20.c \
sensor/pack/g_a_b.icm20789.c \
sensor/pack/g_a_m.lsm9ds1.c \
sensor/pack/g_a_m.mpu9250.c \
sensor/volt/volt.ads1115.c \
usb/perdev/usbhub-driver.c \
usb/perdev/usbds4-driver.c \
usb/perdev/usbxbox-driver.c \
usb/perdev/usbvmware-driver.c \
usb/perfunc/usbhid-driver.c \
usb/perfunc/usbstor-driver.c \
usb/usbhost.c \
usb/usbperi.c \
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
