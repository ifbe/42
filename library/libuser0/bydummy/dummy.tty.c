#include "libuser.h"




int uartnode_read(struct arena* win)
{
	return 0;
}
int uartnode_write(struct arena* win)
{
	return 0;
}
int uartnode_delete(struct arena* win)
{
	return 0;
}
int uartnode_create(struct arena* win, void* str)
{
	int j;
	void* tmp;
	if(0 == str)str = "/dev/ttyUSB0";

	tmp = systemcreate(_uart_, str);
	if(tmp == 0)return 0;

	relationcreate(win, 0, _win_, tmp, 0, _fd_);
	return 0;
}