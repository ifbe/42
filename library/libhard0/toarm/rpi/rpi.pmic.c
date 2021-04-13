#include "libhard.h"




void poweroff()
{
	say("@poweroff\n");

	//qemu -semihosting
/*	asm(
		"mov w0, 0x18\n"
		"mov x1, #0x20000\n"
		"add x1, x1, #0x26\n"
		"hlt #0xf000\n"
	);*/
}