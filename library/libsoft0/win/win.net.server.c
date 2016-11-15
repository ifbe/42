#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>




//
static int alive=0;
static HANDLE thread=0;
//
static char IPADDRESS[32];
static int PORT;
static char DIRECTORY[64];
static int CONFIG;




DWORD WINAPI newone(LPVOID pM)
{
	return 0;
}




int readserver(u64 fd, u8* addr, u64 offset, u64 count)
{
	return 0;
}
int writeserver(u64 fd, u8* addr, u64 offset, u64 count)
{
	return 0;
}
int listserver()
{
	return 0;
}
int changeserver()
{
	return 0;
}
int stopserver(u64 x)
{
	if(x > 0)
	{
		printf("[x]\n");
	}
	else
	{
		printf("stopserver\n");
	}
}
int startserver(char* addr, int port, char* dir, int opt)
{
	//
	alive = 1;
	thread = CreateThread(NULL, 0, newone, NULL, 0, NULL);

	//
	return 0;
}
int deleteserver()
{
	return 0;
}
int createserver()
{
	return 0;
}