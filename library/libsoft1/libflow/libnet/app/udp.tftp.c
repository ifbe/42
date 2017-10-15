#include "artery.h"
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);




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




#define TFTP 0x50544654
#define tftp 0x70746674
int serve_tftp_s(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int serve_tftp_c(struct object* obj, int fd, u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int serve_tftp(struct object* obj, int fd, u8* buf, int len)
{
	u64 type = obj[fd].type_road;
	if(type == TFTP)serve_tftp_s(obj, fd, buf, len);
	if(type == tftp)serve_tftp_c(obj, fd, buf, len);
	return 0;
}
