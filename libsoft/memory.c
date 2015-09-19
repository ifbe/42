#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


void diary(char* fmt,...);




void cleanmemory(QWORD addr,QWORD size)
{
	//清理
	BYTE* memory=(BYTE*)addr;
	int i=0;
	for(i=0;i<size;i++) memory[i]=0;
}




//debug用，打印从addr开始的总共size个字节
void printmemory(QWORD addr,QWORD size)
{
	BYTE* printaddr=(BYTE*)addr;
	int i,j;

	//1111111111111111111111
	diary("[----addr----]");
	for(i=0;i<=0xf;i++)
	{
		diary("%2x ",i);
	}
	diary("[----anscii----]\n");

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)diary("%-14llx",addr+j*16);
		else diary("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			diary("%.2x ",printaddr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			unsigned char ch=printaddr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			diary("%c",ch);
		}
		diary("\n",printaddr[16*j+15]);
	}
}
