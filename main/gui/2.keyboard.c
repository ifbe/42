#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
char* whereispalette();




static void keyboardshow()
{
	int x,y;
	DWORD* screenbuf=(DWORD*)whereispalette();

	for(y=0;y<512;y++)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0;
		}
	}

	//
	for(y=64;y<512;y+=64)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}

	//
	for(x=64;x<1024;x++)
	{
		for(y=0;y<512;y++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}
}
static void keyboardmessage(QWORD type,QWORD value)
{
}
void keyboardinit(char* in)
{
	QWORD* this=(QWORD*)in;
	this[0]=0x776f646e6977;
	this[2]=0x64626b;
	this[6]=(0<<16)+0;   //startaddr
	this[7]=(768<<16)+1024; //endaddr
	this[8]=(QWORD)keyboardshow;
	this[9]=(QWORD)keyboardmessage;
}
