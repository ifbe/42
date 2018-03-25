#include "artery.h"




int tftp_read(u8* buf, int len)
{
	return 0;
}
int tftp_write(u8* buf, int len)
{
	buf[0] = 0xd;
	buf[1] = 0xa;
	buf[2] = 0;
	return 3;
}




#define tftp 0x70746674
#define TFTP 0x50544654
int tftp_client(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int tftp_server(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
