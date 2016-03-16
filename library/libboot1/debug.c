#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void say(char*,...);




void printmemory(BYTE* addr,int size)
{
//debug用，打印从addr开始的总共size个字节
	int i,j;

	//打印的很少就不显示这些了
	if(size>0x10)
	{
		say("[----addr----]");
		for(i=0;i<=0xf;i++)
		{
			say("%2x ",i);
		}
		say("[----anscii----]\n");
	}

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)say( "@%-13llx" , (QWORD)(addr+j*16) );
		else say("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			say("%.2x ",addr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			BYTE ch=addr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			say("%c",ch);
		}
		say("\n");
	}
}




void debuginit(char* type,char* addr)
{
}
void debugkill()
{
}