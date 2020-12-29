#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define PERCPU 0x30000
#define GDTBUF (PERCPU+0x000)
#define TSSBUF (PERCPU+0x1000)
#define IDTBUF (PERCPU+0x2000)
#define STKBUF (PERCPU+0x10000)
void getgdt(void*);
u16 gettss();
//
void loadgdt();
void loadtss();
//
void printmemory(void*, int);
void say(void*, ...);




/*
type:
.bit7 = Present, if(1)valid selectors
.b6,5 = Privilege, 2 bits. if(0)ring0, if(3)ring3
.bit4 = Desctype, if(1)code or data, if(0)tss or ldt
.bit3 = Executable, If(1)code, If(0)data
.bit2 = Direction/Conforming
	Direction bit for data selectors: Tells the direction. 0 the segment grows up. 1 the segment grows down, ie. the offset has to be greater than the limit.
	Conforming bit for code selectors:
	If 1 code in this segment can be executed from an equal or lower privilege level. For example, code in ring 3 can far-jump to conforming code in a ring 2 segment. The privl-bits represent the highest privilege level that is allowed to execute the segment. For example, code in ring 0 cannot far-jump to a conforming code segment with privl==0x2, while code in ring 2 and 3 can. Note that the privilege level remains the same, ie. a far-jump form ring 3 to a privl==2-segment remains in ring 3 after the jump.
	If 0 code in this segment can only be executed from the ring set in privl.
.bit1 = Read/Write
	Readable bit for code selectors: Whether read access for this segment is allowed. Write access is never allowed for code segments.
	Writable bit for data selectors: Whether write access for this segment is allowed. Read access is always allowed for data segments.
.bit0 = Accessed, you set to 0. CPU set to 1 when the segment is accessed.
flag:
.bit3 = Gr: If(0)limit is in 1 B blocks, if(1)limit is in 4 KiB blocks
.bit2 = Sz: Size bit. if(Lbit)has to be 0, elif(0)16 bit protected mode, elif(1)32 bit protected mode
.bit1 = indicate x86_64 code descriptor. This bit is reserved for data segments
.bit0 = unused
*/
struct gdt{
	u16 limit_00_15  ;	//[00,15]
	u16  base_00_15  ;	//[16,31]
	u8   base_16_23  ;	//[32,39]
	u8         type  ;	//[40,47]
	u8  limit_16_19:4;	//[48,51]
	u8         flag:4;	//[52,55]
	u8   base_24_31  ;	//[56,63]
}__attribute__((packed));
struct gdt_64bit{
	u32 zero0  ;	//[00,31]
	u8  zero1  ;	//[32,39]
	u8   type  ;	//[40,47]
	u8  zero2:4;	//[48,51]
	u8   flag:4;	//[52,55]
	u8  zero3  ;	//[56,63]
}__attribute__((packed));
struct gdt_tss{
	u16 limit_00_15  ;	//[00,15]	//0x67
	u16  base_00_15  ;	//[16,31]
	u8   base_16_23  ;	//[32,39]
	u8         type  ;	//[40,47]	//0x89
	u8  limit_16_19:4;	//[48,51]
	u8         flag:4;	//[52,55]	//0x00
	u8   base_24_31  ;	//[56,63]
	u32  base_32_63  ;	//[64,95]
	u32        zero  ;	//[96,127]
}__attribute__((packed));
struct tss{
	u32 rsvd;	//[ 0, 3]
	u64 rsp0;	//[ 4, b]
	u64 rsp1;	//[0c,13]
	u64 rsp2;	//[14,1b]
	u64 ignd;	//[1c,23]
	u64 ist1;	//[24,2b]
	u64 ist2;	//[2c,33]
	u64 ist3;	//[34,3b]
	u64 ist4;	//[3c,43]
	u64 ist5;	//[44,4b]
	u64 ist6;	//[4c,53]
	u64 ist7;	//[54,5b]
	u32 haha;	//[5c,5f]
	u32 hehe;	//[60,63]
	u16 zero;	//[64,65]
	u16 iopb;	//[66,67]
}__attribute__((packed));




void fillgdt(u8* buf)
{
	int j;
	for(j=0;j<0x10000;j++)buf[j] = 0;

	struct tss* tss = (void*)(TSSBUF);
	tss->rsp0 = STKBUF;
	tss->iopb = 0x68;
	say("tss:\n");
	printmemory(tss, 0x68);

	struct gdt* gdt = (void*)buf;
	*(u64*)(buf+0x00) =                  0;		//must null
	*(u64*)(buf+0x08) =                  0;		//myown unused
	*(u64*)(buf+0x10) = 0x00209a0000000000;		//kernel code
	*(u64*)(buf+0x18) = 0x0000920000000000;		//kernel data
	*(u64*)(buf+0x20) = 0x0020fa0000000000;		//user code
	*(u64*)(buf+0x28) = 0x0000f20000000000;		//user data

	struct gdt_tss* gt = (void*)(buf+0x30);
	gt->limit_00_15 = 0x67;		//103;
	gt->base_00_15  = ((u64)tss)&0xffff;
	gt->base_16_23  = (((u64)tss)>>16)&0xff;
	gt->type        = 0x89;
	gt->limit_16_19 = 0;
	gt->flag        = 0;
	gt->base_24_31  = (((u64)tss)>>24)&0xff;
	gt->base_32_63  = ((u64)tss)>>32;
}
void initgdt()
{
	say("@initgdt\n");

	//old
	//say("oldtr@%x\n", gettss());
	say("oldgdt:\n");
	u8 map[16];
	getgdt(map);
	printmemory(map, 16);
	printmemory(*(u8**)(map+2), *(u16*)map + 1);

	//set
	u8* buf = (u8*)GDTBUF;
	fillgdt(buf);

	//new
	say("newgdt:\n");
	printmemory(buf, 0x40);

	//run
	loadgdt();
	loadtss();
}