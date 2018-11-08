typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
//libuser1
void freeactor();
void initactor(void*);
int actorread_all(void*);
int actorwrite_ev(void*);
//libuser0
void freearena();
void initarena(void*);
int arenaread_all(void*);
int arenawrite_ev(void*);
//libsoft1
void freeartery();
void initartery(void*);
int arteryread_all(void*);
int arterywrite_ev(void*);
//libsoft0
void freesystem();
void initsystem(void*);
int systemread_all(void*);
int systemwrite_ev(void*);
//libhard1
void freedriver();
void initdriver(void*);
int driverread_all(void*);
int driverwrite_ev(void*);
//libhard0
void freedevice();
void initdevice(void*);
int deviceread_all(void*);
int devicewrite_ev(void*);
//libboot1
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//libboot0
void* death();
void* birth();
void loop();
//
int termread();
int termwrite(void*, int);
//
int argv2line(void*, void*);
int openwriteclose(void*,int,void*,int);
//
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
	void* dbg;
	void* win;
	u8* addr = beforedawn();

	//cmdline
	k = 0;
	for(j=1;j<argc;j++){k += argv2line(argv[j], addr+k);}
	termwrite(addr, k);

	//loop
	loop();

	//after
	openwriteclose("universe.bin",0,addr,0x1000000);
	afterdusk();
	return 0;
}




/*
#include <efi.h>
#include <efilib.h>
EFI_STATUS efi_hack(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table);
EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table)
{
	efi_hack(handle, table);

	main(0, 0);

	return EFI_SUCCESS;
}
void atexit(){}
*/




#ifdef __ANDROID__
#include <stdio.h>
#include <stdlib.h>
void app_dummy();
void setapp(void*);
void android_main(void* app)
{
	app_dummy();

	setapp(app);
	main(0, 0);

	exit(0);
}
#endif
