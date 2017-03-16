#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void printmemory(void*, int);
void say(void*, ...);




int check_elf(char* addr)
{
	unsigned int temp=*(unsigned int*)addr;
	if(temp==0x464c457f)return 0x666c65;
	return 0;
}
int parse_elf(char* addr)
{
	say("elf\n");
	return 0;
}
