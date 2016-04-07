#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void say(char*,...);




void printmemory(BYTE* addr,int size)
{
//debug用，打印从addr开始的总共size个字节
	int i,j;

	//111111111111111111111111
	if(size<0x10)
	{
		for(i=0;i<size;i++)say("%.2x ",addr[i]);
		say("\n");
		return;
	}

	//2222222222222222222222222
	say("[----addr----]");
	for(i=0;i<=0xf;i++)
	{
		say("%.2x ",i);
	}
	say("[----anscii----]\n");

	//33333333333333333333333333
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
	say("[0,4):killing debug\n");
}
