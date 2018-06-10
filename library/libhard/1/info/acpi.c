#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
#define __S5__ hex32('_','S','5','_')
#define __APIC__ hex32('A','P','I','C')
#define __DSDT__ hex32('D','S','D','T')
#define __FACP__ hex32('F','A','C','P')
#define __HPET__ hex32('H','P','E','T')
#define __MADT__ hex32('M','A','D','T')
#define __MCFG__ hex32('M','C','F','G')
#define __rsdptr__ hex64('R','S','D',' ','P','T','R',' ')
void out16(u16 port, u16 data);
void printmemory(void*, int);
void say(void*, ...);




static u16 port;
static u16 data;
void acpi__APIC__(u64 p)
{
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	printmemory(buf, len);
}
void acpi__DSDT__(u64 p)
{
	int j;
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	//printmemory(buf, len);

	for(j=0;j<len-4;j++)
	{
		if(*(u32*)(buf+j) == __S5__)
		{
			data = *(u8*)(buf+j+8);
			data <<= 10;
			say("data=%08x\n", data);
			//out16(port, data|0x2000);
		}
	}
}
void acpi__FACP__(u64 p)
{
	u64 dsdt;
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	printmemory(buf, len);

	port = *(u16*)(buf+0x40);
	say("port=%08x\n", port);

	dsdt = *(u32*)(p+0x28);
	acpi__DSDT__(dsdt);
}
void acpi__HPET__(u64 p)
{
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	printmemory(buf, len);
}
void acpi__MADT__(u64 p)
{
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	printmemory(buf, len);
}
void acpi__MCFG__(u64 p)
{
	u8* buf = (u8*)p;
	int len = *(u32*)(buf+4);
	printmemory(buf, len);

	say("pciebase=%08x\n", *(u32*)(p+0x2c));
}
void acpithis(u64 addr)
{
	u32 name = *(u32*)addr;
	say("%.*s@%x\n", 4, &name, addr);

	if(name == __APIC__)acpi__APIC__(addr);
	else if(name == __DSDT__)acpi__DSDT__(addr);
	else if(name == __FACP__)acpi__FACP__(addr);
	else if(name == __HPET__)acpi__HPET__(addr);
	else if(name == __MADT__)acpi__MADT__(addr);
	else if(name == __MCFG__)acpi__MCFG__(addr);
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
