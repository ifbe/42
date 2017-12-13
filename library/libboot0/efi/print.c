#include <efi.h>
#include <efilib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* S;
void printinform(void* a, void* b)
{
	H = a;
	S = b;
}




int lowlevel_input(char* buf)
{
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
void deleteserial()
{
}
void createserial()
{
}
