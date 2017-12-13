#include <efi.h>
#include <efilib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))




static u64 ev[4];
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* S;
void printinform(void* a, void* b)
{
	H = a;
	S = b;
}




void deleteserial()
{
}
void createserial()
{
}
int lowlevel_input(void* buf)
{
	int ret;
	while(1)
	{
		ret = S->ConIn->ReadKeyStroke(S->ConIn, buf);
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
			S->ConOut->OutputString(S->ConOut, L"\r\n");
		}
		else
		{
			temp[0] = buf[j];
			S->ConOut->OutputString(S->ConOut, temp);
		}
	}
}
void* pollenv()
{
	int ret = S->ConIn->ReadKeyStroke(S->ConIn, (void*)ev);
	if(ret != EFI_SUCCESS)return 0;
	if(ev[0] <= 0x10000)return 0;

	ev[0] >>= 16;
	ev[1] = hex32('c','h','a','r');
	return ev;
}
void* waitenv()
{
}
