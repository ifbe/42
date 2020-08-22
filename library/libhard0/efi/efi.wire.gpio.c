#include "libhard.h"


int gpioread(int type, int addr, u8* buf, int len)
{
	say("@gpioread:%x,%x\n", type, addr);
	return 0;
}
int gpiowrite(int type, int addr, u8* buf, int len)
{
	say("@gpiowrite:%x,%x,%x,%x\n", type, addr, buf, len);
	return 0;
}
int gpiostart(int name, int mode)
{
	say("@gpiostart:%x,%x\n", name, mode);
	return 0;
}
int gpiostop(int pin)
{
	say("@gpiostop:%x\n", pin);
	return 0;
}
int gpiodelete()
{
	return 0;
}
int gpiocreate()
{
	return 0;
}
