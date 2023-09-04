CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC:=$(shell cat makelist)
OBJ:=$(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libuser1/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi

clean:
	rm -f *.o *.a