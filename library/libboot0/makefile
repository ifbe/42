DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC




#--------bare rpi os--------
barerpi_c = \
bare/rpi/fw/main.c \
bare/rpi/fw/helpenv.c \
bare/rpi/fw/helpgpio.c \
bare/rpi/fw/helpuart.c \
bare/initer.c \
bare/memory.c \
origin.c
barerpi_o = $(patsubst %.c, %.o, $(barerpi_c))

.PHONY:barerpi
barerpi:$(barerpi_o)
	$(AS) -c bare/rpi/fw/entry.s -o $(DIR)/entry.o




#--------bare bios os--------
barebioslive_c = \
bare/bios/live/entry.c \
bare/bios/live/helpenv.c \
bare/bios/live/helpkbd.c \
bare/bios/live/helpuart.c \
bare/initer.c \
bare/memory.c \
origin.c
barebioslive_o = $(patsubst %.c, %.o, $(barebioslive_c))

.PHONY:barebioslive
barebioslive:$(barebioslive_o)




#--------bare efi os--------
bareefimsabi_c = \
bare/efi/common/main.c \
bare/efi/common/helpenv.c \
bare/efi/common/helpkbd.c \
bare/efi/common/helpuart.c \
bare/initer.c \
bare/memory.c \
origin.c
bareefimsabi_o = $(patsubst %.c, %.o, $(bareefimsabi_c))

.PHONY:bareefimsabi
bareefimsabi:$(bareefimsabi_o)


bareefisysvabi_s = \
bare/efi/sysvabi/start.S
bareefisysvabi_s_o = $(patsubst %.S, %.o, $(bareefisysvabi_s))

bareefisysvabi_c = \
bare/efi/common/helpenv.c \
bare/efi/common/helpkbd.c \
bare/efi/common/helpuart.c \
bare/efi/common/main.c \
bare/efi/sysvabi/start.S \
bare/initer.c \
bare/memory.c \
origin.c
bareefisysvabi_c_o = $(patsubst %.c, %.o, $(bareefisysvabi_c))

.PHONY:bareefisysvabi
bareefisysvabi:$(bareefisysvabi_s_o) $(bareefisysvabi_c_o)


bareeficarrybin_c = \
bare/efi/common/main.c \
bare/efi/common/helpenv.c \
bare/efi/common/helpkbd.c \
bare/efi/common/helpuart.c \
bare/initer.c \
bare/memory.c \
origin.c
bareeficarrybin_o = $(patsubst %.c, %.o, $(bareeficarrybin_c))

.PHONY:bareeficarrybin
bareeficarrybin:$(bareeficarrybin_o)




#--------app myos--------
myos_c = \
app/myos/entry.c \
origin.c
myos_o = $(patsubst %.c, %.o, $(myos_c))

.PHONY:myos
myos:$(myos_o)




#--------app efi--------
efiapp_c = \
app/efi/entry.c \
app/efi/helpenv.c \
app/efi/helpuart.c \
bare/initer.c \
bare/memory.c \
origin.c
efiapp_o = $(patsubst %.c, %.o, $(efiapp_c))

.PHONY:efiapp
efiapp:$(efiapp_o)




#--------app libc--------
none_c = \
app/none/entry.c \
app/none/helpenv.c \
app/none/helpuart.c \
app/initer.c \
app/memory.c \
origin.c
none_o = $(patsubst %.c, %.o, $(none_c))

.PHONY:none
none:$(none_o)


wincmd_c = \
app/win/entry.c \
app/win/helpenv.c \
app/win/helpuart.c \
app/initer.c \
app/memory.c \
origin.c
wincmd_o = $(patsubst %.c, %.o, $(wincmd_c))

.PHONY:wincmd
wincmd:$(wincmd_o)




#--------rule--------
%.o: %.S
	outname=$(DIR)/$(notdir $@);\
	if [ ! -e $$outname ] || [ $< -ot $$outname ]; then \
		echo $(AS) $(AF) -c $< -o $$outname;\
		$(AS) $(AF) -c $< -o $$outname;\
	fi
%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libboot0/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi




#--------clean--------
clean:
	rm -f *.o
