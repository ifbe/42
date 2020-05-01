#include "libhard.h"


int boardread(int type, int addr, u8* buf, int len)
{
	say("@boardread:%x,%x\n", type, addr);
	return 0;
}
int boardwrite(int type, int addr, u8* buf, int len)
{
	say("@boardwrite:%x,%x,%x,%x\n", type, addr, buf, len);
	return 0;
}
int boardstart(int name, int mode)
{
	say("@boardstart:%x,%x\n", name, mode);
	return 0;
}
int boardstop(int pin)
{
	say("@boardstop:%x\n", pin);
	return 0;
}
int boarddelete()
{
	return 0;
}
int boardcreate()
{
	return 0;
}