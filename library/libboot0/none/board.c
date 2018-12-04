#include "libboot.h"




int boardread(int type, int addr, u8* buf, int len)
{
	return 0;
}
int boardwrite(int type, int addr, u8* buf, int len)
{
	say("%x,%x,%c,%x\n", type, addr, buf[0], len);
	return 0;
}