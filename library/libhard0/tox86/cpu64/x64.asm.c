#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




void haltwaitforint()
{
	asm("hlt");
}
void* syscall(u64 iCall, void* arg)		//only work with dsdc89
{
    void *retval;
    asm volatile( "syscall" : "=a"(retval) : "D"(iCall), "S"(arg) : "memory" );
    return retval;
}
void cpuid(u32 abcd[4])
{
	asm volatile("cpuid"
		:"=a"(abcd[0]),"=b"(abcd[1]),"=c"(abcd[2]),"=d"(abcd[3])
		:"a"(abcd[0])
	);
}
u64 rdtsc()
{
	u64 ret;
	asm volatile("rdtsc" : "=A"(ret) );
	return ret;
}
u64 rdpmc()
{
	return 0;
}
void gettss()
{
	asm volatile("str %ax");
}
void settss()
{
}
void setgdt(void* buf, u16 len)
{
	struct {
		u16 len;
		void* buf;
	} __attribute__((packed)) GDTR = {len, buf};

	asm("lgdt %0"
		:
		: "m"(GDTR)
	);
}
void getgdt(u8* buf)
{
	asm("sgdt %0"
		:
		: "m"(*buf)
	);
}
void setidt(void* buf, u16 len)
{
	struct {
		u16 len;
		void* buf;
	} __attribute__((packed)) IDTR = {len, buf};

	asm("lidt %0"
		:
		: "m"(IDTR)
	);
}
void getidt(u8* buf)
{
	asm("sidt %0"
		:
		: "m"(*buf)
	);
}
void wrmsr(u32 addr, u64 data)
{
	asm volatile("wrmsr"
		:
		: "c" (addr), "A" (data)
	);
}
u64 rdmsr(u32 addr)
{
	u64 data;
	asm volatile("rdmsr"
		: "=A" (data)
		: "c" (addr)
	);
	return data;
}
/*
u32 readcr(int id)
{
	u32 val;
	if(id == 0)asm volatile("mov %%cr0, %0" : "=r"(val) );
	//else if(id == 1)asm volatile("movl %%cr1, %0" : "=r"(val) );
	else if(id == 2)asm volatile("mov %%cr2, %0" : "=r"(val) );
	else if(id == 3)asm volatile("mov %%cr3, %0" : "=r"(val) );
	else if(id == 4)asm volatile("mov %%cr4, %0" : "=r"(val) );
	else if(id == 5)asm volatile("mov %%cr5, %0" : "=r"(val) );
	else if(id == 6)asm volatile("mov %%cr6, %0" : "=r"(val) );
	else if(id == 7)asm volatile("mov %%cr7, %0" : "=r"(val) );
	else if(id == 8)asm volatile("mov %%cr8, %0" : "=r"(val) );
	return val;
}
void writecr(int id, u32 val)
{
	if(id == 0)asm volatile("mov %0, %%cr0" : "=r"(val) );
	//else if(id == 1)asm volatile("movl %0, %%cr1" : "=r"(val) );
	else if(id == 2)asm volatile("mov %0, %%cr2" : "=r"(val) );
	else if(id == 3)asm volatile("mov %0, %%cr3" : "=r"(val) );
	else if(id == 4)asm volatile("mov %0, %%cr4" : "=r"(val) );
	else if(id == 5)asm volatile("mov %0, %%cr5" : "=r"(val) );
	else if(id == 6)asm volatile("mov %0, %%cr6" : "=r"(val) );
	else if(id == 7)asm volatile("mov %0, %%cr7" : "=r"(val) );
	else if(id == 8)asm volatile("mov %0, %%cr8" : "=r"(val) );
}
*/
