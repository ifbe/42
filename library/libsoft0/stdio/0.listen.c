#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
char* listenbuf=0;
void initlisten(char* addr)
{
	listenbuf=addr;
}
