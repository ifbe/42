#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
char* whereispalette();




void keyboardinit()
{
	
}
void keyboardshow()
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
void keyboardmessage()
{
	
}
