#include <efi.h>
#include <efilib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
void gethandleandtable(void* H, void* T);
void say(void*, ...);




static u64 ev[4];
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* T;




void deleteserial()
{
}
void createserial()
{
	gethandleandtable(&H, &T);
}
int lowlevel_input(void* buf)
{
	int ret;
	while(1)
	{
		ret = T->ConIn->ReadKeyStroke(T->ConIn, buf);
		if(ret == EFI_SUCCESS)return 1;
	}
}
void lowlevel_output(char* buf, int len)
{
	int j;
	unsigned short temp[2] = {0,0};
	for(j=0;j<len;j++)
	{
		if(buf[j] == '\n')
		{
			T->ConOut->OutputString(T->ConOut, L"\r\n");
		}
		else
		{
			temp[0] = buf[j];
			T->ConOut->OutputString(T->ConOut, temp);
		}
	}
}
void* pollenv()
{
	int ret = T->ConIn->ReadKeyStroke(T->ConIn, (void*)ev);
	if(ret != EFI_SUCCESS)return 0;

	if(ev[0] <= 0x10000)
	{
		if(ev[0] == 0x17)
		{
			ev[0] = 0x1b;
			ev[1] = hex32('c','h','a','r');
			return ev;
		}
		else if((ev[0]>=1)&&(ev[0]<=4))
		{
			if(ev[0] == 1)ev[0] = 0x48;
			else if(ev[0] == 2)ev[0] = 0x50;
			else if(ev[0] == 3)ev[0] = 0x4d;
			else if(ev[0] == 4)ev[0] = 0x4b;
			ev[1] = hex32('k','b','d',0);
			return ev;
		}
		else if((ev[0]>=0xb)&&(ev[0]<=0x16))
		{
			ev[0] = ev[0]-0xb+0xf1;
			ev[1] = hex32('k','b','d',0);
			return ev;
		}
		else
		{
			say("%x\n",ev[0]);
			return 0;
		}
	}

	ev[0] >>= 16;
	ev[1] = hex32('c','h','a','r');
	return ev;
}
void* waitenv()
{
}
