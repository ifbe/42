#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




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
