#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<signal.h>
#include<termios.h>
#include<sys/ioctl.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
char* ptsname(int);
int grantpt(int);
int unlockpt(int);
u64 startthread(void*, void*);
void stopthread();
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




struct uartinfo
{
	char* buf;
	int len;
	int enq;
	int deq;
};
static struct uartinfo info;
static u64 thread;
static int master;




void systemshell_process(char* p)
{
	int ret;
	int slave = open(p, O_RDWR);
	if(slave == 0)
	{
		printf("error@open:%d\n",errno);
		return;
	}
	ret = ioctl(slave, TIOCSCTTY, NULL);

	setsid();
	dup2(slave, 0);
	dup2(slave, 1);
	dup2(slave, 2);
	execl("/bin/bash", "/bin/bash", NULL);
}
void* systemshell_thread(void* p)
{
	int ret,max;
	while(1)
	{
		max = 0x100000 - (info.enq);
		if(max > 256)max = 256;

		ret = read(
			master,
			(info.buf)+(info.enq),
			256
		);
		if(ret < 0)break;
		if(ret == 0)continue;

		info.enq = (info.enq+ret)%0x100000;
		eventwrite(
			(u64)&info,
			hex32('u','a','r','t'),
			0,
			0
		);
	}
	return 0;
}




int systemshell_read(char* buf, int len)
{
	return 0;
}
int systemshell_write(char* buf, int len)
{
	int ret;
	ret = write(master, buf, len);
	return ret;
}
int systemshell_list(char* p)
{
	int ret = system("ls /dev/pts/");
	return 0;
}
int systemshell_choose(char* p, int speed)
{
	int ret;
	char* name;

	master = open("/dev/ptmx", O_RDWR);
	if(master <= 0)
	{
		printf("error@open:%d\n",errno);
		return -1;
	}

	ret = grantpt(master);
	if(ret < 0)
	{
		printf("error@grantpt:%d\n",errno);
		return -2;
	}

	ret = unlockpt(master);
	if(ret < 0)
	{
		printf("error@unlockpt:%d\n",errno);
		return -3;
	}

	name = ptsname(master);
	if(name == 0)
	{
		printf("error@ptsname:%d\n",errno);
		return -4;
	}
	printf("%.*s\n", 16, name);

	ret = fork();
	if(ret < 0)return -5;
	else if(ret == 0)systemshell_process(name);
	else
	{
		signal(SIGCHLD, SIG_IGN);
		if(info.buf == 0)
		{
			info.enq = 0;
			info.deq = 0;
			info.len = 0x100000;
			info.buf = (void*)malloc(info.len);
		}
		startthread(systemshell_thread, 0);
		write(master, "unset PROMPT_COMMAND\n", 21);
	}
	return 0;
}




int systemshell_stop()
{
	return 0;
}
int systemshell_start(char* p)
{
	return 0;
}
void systemshell_create()
{
}
void systemshell_delete()
{
}
