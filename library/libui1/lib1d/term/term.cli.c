#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void arteryread();
void arterywrite(void*);
//
int buf2arg(u8* buf,int max,int* argc,u8** argv);
int buf2type(u8* buf,int max,u64* type,u8** name);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void say(void*, ...);




static char* input = 0;
static char* output = 0;




void cli_create(void* addr)
{
	input = addr + 0x400000;
	output = addr + 0x500000;
}
void cli_delete()
{
}
void cli_start()
{
}
void cli_stop()
{
}
void cli_list()
{
}
void cli_choose()
{
}
void cli_read()
{
	say("[void]");
}
void cli_write(u64* p)
{
	int j;
	int* in = (void*)(input+0xffff0);
	if(p[1] != 0x72616863)return;

	arterywrite(p);

	j = *(u8*)p;
	if(j == 0x8)		//backspace
	{
		if(*in <= 0)return;
		(*in)--;
		input[*in] = 0;
	}
	else if(j == 0xa)	//enter
	{
		for(j=0;j<0x80;j++)input[j] = 0;
		*in = 0;
	}
	else
	{
		input[*in] = j;
		(*in)++;
	}
}