DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC


rpi_c = \
bare/initer.c \
bare/memory.c \
bare/rpi/main.c \
bare/rpi/helpenv.c \
bare/rpi/helpgpio.c \
bare/rpi/helpuart.c \
origin.c
rpi_o = $(patsubst %.c, %.o, $(rpi_c))

bare_c = \
bare/initer.c \
bare/memory.c \
bare/barex64/entry.c \
bare/barex64/helpenv.c \
bare/barex64/helpkbd.c \
bare/barex64/helpuart.c \
origin.c
bare_o = $(patsubst %.c, %.o, $(bare_c))

bareapp_c = \
app/bareapp/entry.c \
origin.c
bareapp_o = $(patsubst %.c, %.o, $(bare_c))

efios_c = \
bare/initer.c \
bare/memory.c \
bare/efi/entry.c \
bare/efi/helpenv.c \
bare/efi/helpkbd.c \
bare/efi/helpuart.c \
origin.c
efios_o = $(patsubst %.c, %.o, $(efios_c))

efiapp_c = \
bare/initer.c \
bare/memory.c \
app/efi/entry.c \
app/efi/helpenv.c \
app/efi/helpuart.c \
origin.c
efiapp_o = $(patsubst %.c, %.o, $(efiapp_c))

none_c = \
app/initer.c \
app/memory.c \
app/none/entry.c \
app/none/helpenv.c \
app/none/helpuart.c \
origin.c
none_o = $(patsubst %.c, %.o, $(none_c))

wincmd_c = \
app/initer.c \
app/memory.c \
app/win/entry.c \
app/win/helpenv.c \
app/win/helpuart.c \
origin.c
wincmd_o = $(patsubst %.c, %.o, $(wincmd_c))

.PHONY:rpi
rpi:$(rpi_o)
	$(AS) -c bare/rpi/entry.s -o $(DIR)/entry.o

.PHONY:bare
bare:$(bare_o)

.PHONY:bareapp
bareapp:$(bareapp_o)

.PHONY:efios
efios:$(efios_o)

.PHONY:efiapp
efiapp:$(efiapp_o)

.PHONY:none
none:$(none_o)

.PHONY:wincmd
wincmd:$(wincmd_o)

%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libboot0/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi


clean:
	rm -f *.o
