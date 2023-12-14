#include "libhard.h"
#define PORT 0x3f8
//
u8 in8(u32 port);
void out8(u32 port, u8 data);
//
u8* getoemid();
//
//void stdin_setboardseiral(void* node);
void stdout_setboardseiral(void* node);




static int read8250_one(u8* buf)
{
	int j = 0;
	if((in8(PORT + 5) & 1) != 0)
	{
		buf[0] = in8(PORT);
		j = 1;
	}
	return j;
}
int boardserial_input(u8* buf, int len)
{
	int j,ret;
	for(j=0;j<len;j++)
	{
		ret = read8250_one(buf+j);
		if(ret == 0)break;
	}
	return j;
}




static int write8250_one(u8 data)
{
	int j=0;
	while((in8(PORT + 5) & 0x20) == 0)
	{
		j++;
		if(j>0xffffff)return 0;
	}
	out8(PORT, data);
	return 1;
}
int boardserial_output(char* buf, int len)
{
	int j;
	for(j=0;j<len;j++)write8250_one(buf[j]);
	return j;
}




void freeboardserial()
{
}
void initboardserial()
{
	out8(PORT + 1, 0x00);//Disable all interrupts
	out8(PORT + 3, 0x80);//Enable DLAB (set baud rate divisor)
	out8(PORT + 0, 0x01);//1=115200, 3=38400
	out8(PORT + 1, 0x00);//(high byte)
	out8(PORT + 3, 0x03);//8 bits, no parity, one stop bit
	out8(PORT + 2, 0xC7);//Enable FIFO, clear them, with 14-byte threshold
	out8(PORT + 4, 0x0B);//IRQs enabled, RTS/DSR set

	boardserial_output("initboardserial success\n", 24);
}




static void boardserial_read(struct item* e1000,void* foot, p64 arg,int cmd, void* buf,int len)
{
    boardserial_input(buf,len);
}
static void boardserial_write(struct item* e1000,void* foot, p64 arg,int cmd, void* buf,int len)
{
    boardserial_output(buf,len);
}




void initboarddebug(struct item* dev)
{
	u8* oemid = getoemid();
	if(0 == oemid)return;

	if(0 == ncmp(oemid, "BOCHS ",6)){
		initboardserial();

        dev->onreader = (void*)boardserial_read;
        dev->onwriter = (void*)boardserial_write;
        stdout_setboardseiral(dev);
	}
}
void exitboarddebug(struct item* dev)
{
}