#include<stdio.h>
#include<stdlib.h>
#define QWORD unsigned long long
unsigned char* readbuf;
unsigned char* dirbuf;
unsigned char* fsbuf;


__attribute__((constructor)) void initmemory()
{
        readbuf=(unsigned char*)malloc(0x100000);
        dirbuf=(unsigned char*)malloc(0x100000);
        fsbuf=(unsigned char*)malloc(0x100000);

	say("readbuf@%llx\ndirbuf@%llx\nfsbuf@%llx\n",
	(QWORD)readbuf,(QWORD)dirbuf,(QWORD)fsbuf);
}
__attribute__((destructor)) void destorymemory()
{
        free(readbuf);
        free(dirbuf);
        free(fsbuf);
}




void getaddrofbuffer(unsigned long long* p)
{
	*p=(unsigned long long)readbuf;
}
void getaddrofdir(unsigned long long* p)
{
	*p=(unsigned long long)dirbuf;
}
void getaddroffs(unsigned long long* p)
{
	*p=(unsigned long long)fsbuf;
}
