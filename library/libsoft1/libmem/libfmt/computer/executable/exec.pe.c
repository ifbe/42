#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printmemory(char* addr,u64 size);
void say(char* fmt,...);
struct mz
{
	u16 magic;		//'M','Z'
	u16 cblp;		//文件最后页字节数
	u16 fcp;		//文件页数
	u16 crlc;		//重定义元素个数
	u16 hdrcp;		//头部尺寸
	u16 min;		//最小附加段
	u16 max;		//最大附加段
	u16 ss;
	u16 sp;
	u16 cksum;		//校验和
	u16 ip;
	u16 cs;
	u16 farlc;		//重分配表地址
	u16 ovno;		//覆盖号
	u16 res[4];		//保留字
	u16 oemid;
	u16 oeminfo;
	u16 res2[10];	//保留字
	u32 addr;		//pe段的地址
};
struct pe
{
	u32 magic;
	u16 machine;
	u16 numsec;
	u32 timedate;
	u32 symtab;
	u32 symnum;
	u16 opthdr;
	u16 character;
};
struct opthdr
{
	u16 magic;
	u8 majorlinker;
	u8 minorlinker;
	u32 codesize;
	u32 datasize;
	u32 bsssize;
	u32 entry;
	u32 codebase;
};
struct opt32
{
	u32 database;
	u32 imagebase;
	u32 sectionalign;
	u32 filealign;
	u16 majoropsys;
	u16 minoropsys;
	u16 majorversion;
	u16 minorversion;
	u16 majorsubsys;
	u16 minorsubsys;
	u16 win32version;
	u32 imagesize;
	u32 headersize;
	u32 checksum;
	u16 subsys;
	u16 dllcharacter;
	u32 stackreserve;
	u32 stackcommit;
	u32 heapreserve;
	u32 heapcommit;
	u32 loaderflag;
	u32 numrvasize;
};
struct opt64
{
	u64 imagebase;
	u32 sectionalign;
	u32 filealign;
	u16 majoropsys;
	u16 minoropsys;
	u16 majorversion;
	u16 minorversion;
	u16 majorsubsys;
	u16 minorsubsys;
	u16 win32version;
	u32 imagesize;
	u32 headersize;
	u32 checksum;
	u16 subsys;
	u16 dllcharacter;
	u64 stackreserve;
	u64 stackcommit;
	u64 heapreserve;
	u64 heapcommit;
	u32 loaderflag;
	u32 numrvasize;
};
struct dir
{
	u32 exportaddr;
	u32 exportsize;
	u32 importaddr;
	u32 importsize;
	u32 resourceaddr;
	u32 resourcesize;
	u32 exceptionaddr;
	u32 exceptionsize;
	u32 certificateaddr;
	u32 certificatesize;
	u32 relocationaddr;
	u32 relocationsize;
	u32 debugaddr;
	u32 debugsize;
	u32 architectureaddr;
	u32 architecturesize;
	u32 globalptraddr;
	u32 globalptrsize;
	u32 tlstableaddr;
	u32 tlstablesize;
	u32 loadconfigaddr;
	u32 loadconfigsize;
	u32 boundimportaddr;
	u32 boundimportsize;
	u32 iataddr;
	u32 iatsize;
	u32 delayimportaddr;
	u32 delayimportsize;
	u64 clrruntime;
	u64 mustzero;
};




int check_pe(u8* addr)
{
	u32 temp;

	//[0,1]:'M','Z'
	temp=*(u16*)addr;
	if(temp!=0x5a4d)return 0;

	//[0x3c,0x3f]:	=0x40,=0xf0,=0x?
	temp=*(u32*)(addr+0x3c);
	if(temp>=0xffc)return 0;

	temp=*(u32*)(addr+temp);
	if(temp!=0x4550)return 0;

	return 64;
}
int parse_pe(u8* addr)
{
	int j,k;
	struct mz* mz;
	struct pe* pe;
	struct opthdr* opthdr;
	struct opt32* opt32;
	struct opt64* opt64;
	struct dir* dir;
	say("pe\n");

	//mz head
	mz = (void*)addr;
	j = mz->addr;
	say("[0,%x]\n", j-1);

	//pe head
	pe = (void*)(addr+j);
	say(
		"machine=%x\n"
		"symtab=%x\n"
		"symnum=%x\n"
		"\n",
		pe->machine,
		pe->symtab,
		pe->symnum
	);

	//opt head
	opthdr = (void*)(addr+j+24);
	say(
		"codesize=%x\n"
		"datasize=%x\n"
		"bsssize=%x\n"
		"entry=%x\n"
		"codebase=%x\n"
		"\n",
		opthdr->codesize,
		opthdr->datasize,
		opthdr->bsssize,
		opthdr->entry,
		opthdr->codebase
	);

	//opt body
	if(opthdr->magic == 0x10b)
	{
		opt32 = (void*)(addr + j + 24 + 24);
		say(
			"imagebase=%x\n"
			"imagesize=%x\n"
			"headersize=%x\n"
			"stackreserve=%x\n"
			"stackcommit=%x\n"
			"stackreserve=%x\n"
			"stackcommit=%x\n"
			"\n",
			opt32->imagebase,
			opt32->imagesize,
			opt32->headersize,
			opt64->stackreserve,
			opt64->stackcommit,
			opt64->heapreserve,
			opt64->heapcommit
		);

		dir = (void*)(addr + j + 0x78);
	}
	else if(opthdr->magic == 0x20b)
	{
		opt64 = (void*)(addr + j + 24 + 24);
		say(
			"imagebase=%llx\n"
			"imagesize=%llx\n"
			"headersize=%llx\n"
			"stackreserve=%llx\n"
			"stackcommit=%llx\n"
			"stackreserve=%llx\n"
			"stackcommit=%llx\n"
			"\n",
			opt64->imagebase,
			opt64->imagesize,
			opt64->headersize,
			opt64->stackreserve,
			opt64->stackcommit,
			opt64->heapreserve,
			opt64->heapcommit
		);

		dir = (void*)(addr + j + 0x88);
	}

	say(
		"export@%llx$%llx\n"
		"import@%llx$%llx\n"
		"resource@%llx$%llx\n"
		"exception@%llx$%llx\n"
		"certificate@%llx$%llx\n"
		"relocation@%llx$%llx\n"
		"debug@%llx$%llx\n"
		"architecture@%llx$%llx\n"
		"globalptr@%llx$%llx\n"
		"tlstable@%llx$%llx\n"
		"loadconfig@%llx$%llx\n"
		"boundimport@%llx$%llx\n"
		"iat@%llx$%llx\n"
		"delayimport@%llx$%llx\n"
		"\n",
		dir->exportaddr, dir->exportsize,
		dir->importaddr, dir->importsize,
		dir->resourceaddr, dir->resourcesize,
		dir->exceptionaddr, dir->exceptionsize,
		dir->certificateaddr, dir->certificatesize,
		dir->relocationaddr, dir->relocationsize,
		dir->debugaddr, dir->debugsize,
		dir->architectureaddr, dir->architecturesize,
		dir->globalptraddr, dir->globalptrsize,
		dir->tlstableaddr, dir->tlstablesize,
		dir->loadconfigaddr, dir->loadconfigsize,
		dir->boundimportaddr, dir->boundimportsize,
		dir->iataddr, dir->iatsize,
		dir->delayimportaddr, dir->delayimportsize
	);

	return 0;
}
