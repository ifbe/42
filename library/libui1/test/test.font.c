#define QWORD unsigned long long
#define DWORD unsigned int
//
void printascii(int x, int y, int size, char ch, DWORD fgcolor, DWORD bgcolor);
void printdecimal(int x, int y, int size, int data, DWORD fg, DWORD bg);
void backgroundcolor();
//
void say(char*,...);





void font_list()
{
}
void font_change()
{
}
void font_read()
{
	int x,y;
	backgroundcolor(0);

	for(y=2;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			printascii(
				x*16,
				y*16,
				1,
				x+y*16,
				0xffffffff,
				0
			);
		}
	}
}
void font_write()
{
}
void font_start(QWORD size,void* addr)
{
}
void font_stop()
{
}
void font_init(char*base,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x746e6f66;

	this[10]=(QWORD)font_start;
	this[11]=(QWORD)font_stop;
	this[12]=(QWORD)font_list;
	this[13]=(QWORD)font_change;
	this[14]=(QWORD)font_read;
	this[15]=(QWORD)font_write;
}
void font_kill()
{
}
