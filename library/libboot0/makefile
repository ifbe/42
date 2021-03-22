DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC


rpi_c = \
bare/initer.c \
bare/memory.c \
bare/rpi/main.c \
bare/rpi/runenv.c \
bare/rpi/serial.c \
origin.c
rpi_o = $(patsubst %.c, %.o, $(rpi_c))

bare_c = \
bare/initer.c \
bare/memory.c \
bare/barex64/entry.c \
bare/barex64/keyboard.c \
bare/barex64/runenv.c \
bare/barex64/serial.c \
origin.c
bare_o = $(patsubst %.c, %.o, $(bare_c))

efios_c = \
bare/initer.c \
bare/memory.c \
bare/efi/entry.c \
bare/efi/keyboard.c \
bare/efi/runenv.c \
bare/efi/serial.c \
origin.c
efios_o = $(patsubst %.c, %.o, $(efios_c))

efiapp_c = \
bare/initer.c \
bare/memory.c \
app/efi/entry.c \
app/efi/runenv.c \
origin.c
efiapp_o = $(patsubst %.c, %.o, $(efiapp_c))

none_c = \
app/initer.c \
app/memory.c \
app/none/entry.c \
app/none/runenv.c \
app/none/serial.c \
origin.c
none_o = $(patsubst %.c, %.o, $(none_c))

wincmd_c = \
app/initer.c \
app/memory.c \
app/win/entry.c \
app/win/runenv.c \
app/win/serial.c \
origin.c
wincmd_o = $(patsubst %.c, %.o, $(wincmd_c))

.PHONY:rpi
rpi:$(rpi_o)
	$(AS) -c bare/rpi/entry.s -o $(DIR)/entry.o

.PHONY:bare
bare:$(bare_o)

.PHONY:efios
efios:$(efios_o)

.PHONY:efiapp
efiapp:$(efiapp_o)

.PHONY:none
none:$(none_o)

.PHONY:wincmd
wincmd:$(wincmd_o)

%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o
