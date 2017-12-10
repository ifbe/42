#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __rsdptr__ hex64('R','S','D',' ','P','T','R',' ')
void say(void*, ...);
void printmemory(void*, int);




void acpithis(u64 addr)
{
	char* this = (char*)addr;
	say("%.*s@%x\n", 4, this, addr);
}
void acpirsdt(void* p)
{
	int j;
	u64 temp;
	u8* addr; 

	temp = *(u32*)(p+0x10);
	addr = (u8*)temp;
	say("rsdt@%x\n", temp);
	//printmemory(addr, addr[4]);

	temp = addr[4];
	for(j=0x24;j<temp;j+=4)
	{
		acpithis(*(u32*)(addr+j));
	}
}
void acpixsdt(void* p)
{
	int j;
	u64 temp;
	u8* addr; 

	temp = *(u64*)(p+0x18);
	addr = (u8*)temp;
	say("xsdt@%x\n", temp);
	//printmemory(addr, addr[4]);

	temp = addr[4];
	for(j=0x24;j<temp;j+=8)
	{
		acpithis(*(u32*)(addr+j));
	}
}
void initacpi()
{
	int j;
	void* p = (char*)0xe0000;
	for(j=0;j<0x1fff8;j++)
	{
		if(*(u64*)(p+j) == __rsdptr__)
		{
			say("RSD PTR @%x\n", p+j);
			if(*(u8*)(p+j+15) == 0)
			{
				acpirsdt(p+j);
			}
			else
			{
				acpixsdt(p+j);
			}
		}
	}
}
