#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int startfile(u8*);
int stopfile(u8*);
int readfile(u8*,u8*,u64,u64);
int writefile(u8*,u8*,u64,u64);
//
void printmemory(u64 start,u64 count);
void say(void*, ...);




//
static u8* guys;
static u8* fshome;
static u8* dirhome;
static u8* datahome;




int executable_read(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].read(mem, file, offset, count);
}
int executable_write(u8* mem, u8* file, u64 offset, u64 count)
{
	//return guys[which].write(mem, file, offset, count);
}




void executable_create(void* softaddr, u64* p)
{
	guys = softaddr;
	fshome = softaddr + 0x100000;
	dirhome = softaddr + 0x200000;
	datahome = softaddr + 0x300000;
}
void executable_delete()
{
}
