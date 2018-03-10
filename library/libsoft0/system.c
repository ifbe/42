#include "system.h"
//random
int createrandom(void*);
int deleterandom();
//signal
int createsignal(void*);
int deletesignal();
//epoll,iocp,kqueue
int createwatcher(void*);
int deletewatcher();
//socket
int createsocket(void*);
int deletesocket();
//shell
int createshell(void*);
int deleteshell();
//uart
int createuart(void*);
int deleteuart();
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct object* obj;




int systemdelete()
{
	return 0;
}
int systemcreate()
{
	return 0;
}
int systemstop()
{
	return 0;
}
int systemstart(char* name, int flag)
{
	return 0;
}
int systemread(int fd, char* buf, int off, int len)
{
	return 0;
}
int systemwrite(int fd, char* buf, int off, int len)
{
	return 0;
}
int systemlist(u8* buf)
{
	int j,k=0;
	void* addr;
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].sock)continue;

		k++;
		addr = (void*)(&obj[j]);
		say("[%03x]: %.8s,%.8s\n", j, addr, addr+8);
	}

	if(0 == k)say("empth system\n");
	return 0;
}
int systemchoose(u8* buf)
{
	say("@system: %s\n", buf);
	return 0;
}




void freesystem()
{
	//say("[8,c):freeing system\n");

	deleteuart();
	deleteshell();
	deletesocket();
	deletewatcher();
	deletesignal();
	deleterandom();
}
void initsystem(u8* addr)
{
	int j;
	for(j=0;j<0x400000;j++)addr[j]=0;
	obj = (void*)addr;

	createrandom(addr);
	createsignal(addr);
	createwatcher(addr);
	createsocket(addr);
	createshell(addr);
	createuart(addr);

	//say("[8,c):inited system\n");
}
