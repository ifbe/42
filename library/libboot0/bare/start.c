#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void initpage();
void initgdt();
void initidt();
void initacpi();
//
void init8259();
void initapic();
void initioapic();
//
void init8042();
void initrtc();
void initpci();
//
void createserial();
void main(int, char**);




void start()
{
	main(0, 0);
}




int arg2utf8(u8* src, u8* dst)
{
	int j = 0;
	while(src[j] >= 0x20)
	{
		dst[j] = src[j];
		j++;
	}

	dst[j] = 0;
	return j;
}




void* pollenv()
{
	return 0;
}
void* waitenv()
{
	return 0;
}




void death()
{
}
void* birth()
{
	int j;
	u8* mem = (u8*)0x1000000;
	createserial();

	for(j=0;j<0x1000000;j++)mem[j] = 0;
	return mem;
}



/*
void baseenvcreate(void* addr)
{
	//say("[4,8):createed driver\n");
	initpage();
	initgdt();
	initidt();
	initacpi();

	init8259();
	//initapic();
	//initioapic();

	init8042();
	initrtc();
	initpci();

	asm("sti");
}
void baseenvdelete()
{
	//say("[4,8):deleteing driver\n");
}
*/
