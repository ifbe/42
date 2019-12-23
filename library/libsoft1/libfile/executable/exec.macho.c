#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void say(void*, ...);
struct head
{
	u32 magic;
	u32 cputype;
	u32 cpusubtype;
	u32 filetype;
	u32 cmdnum;
	u32 cmdsz;
	u32 flag;
};




u32 check_macho(u8* addr)
{
	u32 temp;
	if(addr == 0)return 0;

	temp = *(u32*)addr;
	if( (temp==0xfeedface) | (temp==0xfeedfacf) )
	{
		//feedface, feedfacf
		return temp;
	}
	if(temp==0xcafebabe)
	{
		//cafebabe
		return temp;
	}

	return 0;
}
int parse_macho(u8* addr)
{
	struct head* head;
	say("macho\n");

	head = (void*)addr;
	say(
		"cputype=%x\n"
		"cpusubtype=%x\n"
		"filetype=%x\n"
		"\n",
		head->cputype,
		head->cpusubtype,
		head->filetype
	);

	say("entrypoint@%x\n", *(u32*)(addr+0x600));
	return 0;
}
