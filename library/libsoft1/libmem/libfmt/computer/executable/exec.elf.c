#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printmemory(void*, int);
void say(void*, ...);
struct head
{
	u8 magic[4];	//0x7f, 'E', 'L', 'F'
	u8 cpubit;
	u8 endian;
	u8 ver1;
	u8 abi;
	u64 unused;
	u16 type;
	u16 instruction;
	u32 ver2;
};
struct elf32
{
	u32 programentry;
	u32 ptabaddr;
	u32 stabaddr;
	u32 flag;	//x86=3, mips=8, ppc=0x14, arm=0x28, x64=0x3e, a64=0xb7
	u16 headersize;
	u16 ptabsz;
	u16 ptabnum;
	u16 stabsz;
	u16 stabnum;
	u16 index;
};
struct elf64
{
	u64 programentry;
	u64 ptabaddr;
	u64 stabaddr;
	u32 flag;
	u16 headersize;
	u16 ptabsz;
	u16 ptabnum;
	u16 stabsz;
	u16 stabnum;
	u16 index;
};
struct ptab32
{
	u32 type;
	u32 offset;
	u32 vaddr;
	u32 undef;
	u32 segszinfile;
	u32 segszinmem;
	u32 flag;
	u32 align;
};
struct ptab64
{
	u32 type;
	u32 flag;
	u64 offset;
	u64 vaddr;
	u64 undef;
	u64 segszinfile;
	u64 segszinmem;
	u64 align;
};




int check_elf(u8* addr)
{
	u32 temp=*(u32*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
int parse_elf(u8* addr)
{
	int j,k;
	struct head* head;
	struct elf32* e32;
	struct elf64* e64;
	struct ptab32* p32;
	struct ptab64* p64;
	say("elf\n");

	//
	head = (void*)addr;
	say(
		"endian=%x\n"
		"abi=%x\n"
		"type=%x\n"
		"instruction=%x\n"
		"\n",
		head->endian,
		head->abi,
		head->type,
		head->instruction
	);
	if(head->cpubit == 1)
	{
		e32 = (void*)(addr+0x18);
		say(
			"entryaddr=%x\n"
			"ptabaddr=%x\n"
			"ptabnum=%x\n"
			"stabaddr=%x\n"
			"stabnum=%x\n"
			"\n",
			e32->programentry,
			e32->ptabaddr,
			e32->ptabnum,
			e32->stabaddr,
			e32->stabnum
		);

		p32 = (void*)(addr + (e32->ptabaddr));
		k = e32->ptabnum;
		if(k>16)k=16;
		for(j=0;j<k;j++)
		{
			say(
				"offset=%08x	"
				"vaddr=%08x	"
				"segszinfile=%08x	"
				"segszinmem=%08x\n",
				p32->offset,
				p32->vaddr,
				p32->segszinfile,
				p32->segszinmem
			);
			p32++;
		}
	}
	else if(head->cpubit == 2)
	{
		e64 = (void*)(addr+0x18);
		say(
			"entryaddr=%llx\n"
			"ptabaddr=%llx\n"
			"ptabnum=%llx\n"
			"stabaddr=%llx\n"
			"stabnum=%llx\n"
			"\n",
			e64->programentry,
			e64->ptabaddr,
			e64->ptabnum,
			e64->stabaddr,
			e64->stabnum
		);

		p64 = (void*)(addr + (e64->ptabaddr));
		k = e64->ptabnum;
		if(k>16)k=16;
		for(j=0;j<k;j++)
		{
			say(
				"offset=%08llx	"
				"vaddr=%08llx	"
				"segszinfile=%08llx	"
				"segszinmem=%08llx\n",
				p64->offset,
				p64->vaddr,
				p64->segszinfile,
				p64->segszinmem
			);
			p64++;
		}
	}

	return 0;
}
