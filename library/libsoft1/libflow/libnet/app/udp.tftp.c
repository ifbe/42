#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x0f]
	u64 type_sock;  //raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;  //ssh, tls?
	u64 stage1;
	u64 type_app;   //http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;  //html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};




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
