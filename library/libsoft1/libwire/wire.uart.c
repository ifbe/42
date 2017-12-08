#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
int decstr2data(void*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
int startuart(u8* name, int speed);
int stopuart();
int listuart(u8* p);
int chooseuart(u8* p);
int readuart(int fd, u8* buf, int off, int len);
int writeuart(int fd, u8* buf, int off, int len);
//
void printmemory(void*, int);
void say(void*, ...);




int uart_read(int fd, u8* buf, int off, int len)
{
	readuart(fd, buf, off, len);
	return 0;
}
int uart_write(int fd, u8* buf, int off, int len)
{
	for(len=0;len<256;len++)
	{
		if(buf[len] == 0)break;
	}
	if(len <= 0)return 0;
	if(len >= 256)return 0;

	len = readuart(fd, buf, off, len);
	return len;
}
int uart_list(u8* p)
{
	int j;
	j = listuart(p);
	return j;
}
int uart_choose(u8* p)
{
	return 0;
}
int uart_stop(u8* p)
{
	return 0;
}
int uart_start(u8* p)
{
	int j;
	u8 name[64];
	u64 speed = 115200;
	if(p==0)return 0;

	for(j=0;j<64;j++)
	{
		if(p[j] == 0)break;
		if(p[j] == ',')
		{
			decstr2data(p+j+1, &speed);
			break;
		}
		else name[j] = p[j];
	}
	name[j] = 0;

	j = startuart(name, speed);
	if(j <= 0)return 0;

	return 0;
}
void uart_delete()
{
}
void uart_create(void* world,u64* p)
{
	p[0]=0x6563616669;	//type
	p[1]=0x74726175;	//id
	p[2]=(u64)uart_start;
	p[3]=(u64)uart_stop;
	p[4]=(u64)uart_list;
	p[5]=(u64)uart_choose;
	p[6]=(u64)uart_read;
	p[7]=(u64)uart_write;
}
