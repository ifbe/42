typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
//libuser1
void freeactor();
void initactor(void*);
//libuser0
void freearena();
void initarena(void*);
//libsoft1
void freeartery();
void initartery(void*);
//libsoft0
void freesystem();
void initsystem(void*);
//libhard1
void freedriver();
void initdriver(void*);
//libhard0
void freedevice();
void initdevice(void*);
//libboot1
void freepulser();
void initpulser(void*);
//libboot0
void freetheall();
void inittheall(void*);
//
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
void* death();
void* birth();
//
void prep();
void loop();
//
int openreadclose(void*,int,void*,int);
int openwriteclose(void*,int,void*,int);
//i
int arg2utf8(void*, void*);
int termwrite(void*, int);
//o
void printmemory(void*, int);
void say(void*, ...);




void* beforedawn()
{
	//allocate
	void* addr = birth();

	//libboot
	initstdin( addr+0x000000);
	initstdout(addr+0x100000);
	initstdev( addr+0x200000);
	initstdrel(addr+0x300000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x400000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0x800000);

	//libuser
	initarena(addr+0xc00000);
	initactor(addr+0xc00000);

	return addr;
}
void afterdusk()
{
	//libuser
	freeactor();
	freearena();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freestdev();
	freestdrel();
	freestdout();
	freestdin();

	//cleanup
	death();
}
int main(int argc, char* argv[])
{
	//before
	u64 j,k;
	u8* addr = beforedawn();

	//cmdline
	termwrite("\n@42@\n", 1);
	for(j=1;j<argc;j++)
	{
		k = arg2utf8(argv[j], addr);
		say("%.*s\n", k, addr);

		if(termwrite(addr, k) < 0)goto byebye;
	}

	//load cfg
	prep();

	//main loop
	loop();

	//after mainloop: backup world
	//if(0 == dstctx)dstctx =
	//if(0 == storectx(dstctx))dump();
	openwriteclose("universe.bin",0,addr,0x1000000);

byebye:
	//after
	afterdusk();
	return 0;
}
